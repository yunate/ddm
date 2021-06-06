
#include "dir_utils.h"
#ifdef DD_WINDOW
#include <queue>
#include <Windows.h>
BEG_NSP_DDM

bool dir_uitls::is_dir(const ddstr& path)
{
    DWORD ftyp = ::GetFileAttributes(path.c_str());

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
        // 这是一个文件夹
        return true;
    }

    return false;
}

bool dir_uitls::is_path_exist(const ddstr& filePath)
{
    DWORD dwAttrib = ::GetFileAttributes(filePath.c_str());
    return (INVALID_FILE_ATTRIBUTES != dwAttrib);
}

bool dir_uitls::create_file(const ddstr& filePath)
{
    DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
    DWORD dwShareMode = 0;
    LPSECURITY_ATTRIBUTES lpSecurityAttributes = 0;
    DWORD dwCreationDisposition = CREATE_NEW;
    DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE hTemplateFile = 0;
    HANDLE handle = CreateFile(filePath.c_str(),
                               dwDesiredAccess,
                               dwShareMode,
                               lpSecurityAttributes,
                               dwCreationDisposition,
                               dwFlagsAndAttributes,
                               hTemplateFile);

    if (handle == INVALID_HANDLE_VALUE) {
        return false;
    }

    ::CloseHandle(handle);
    return true;
}

bool dir_uitls::delete_file(const ddstr& dfilePath)
{
    return (::DeleteFile(dfilePath.c_str()) == TRUE);
}

bool dir_uitls::delete_dir(const ddstr& dirPath)
{
    if (!is_path_exist(dirPath)) {
        return false;
    }

    HANDLE hFile = nullptr;
    WIN32_FIND_DATA fileInfo;
    ::memset(&fileInfo, 0, sizeof(LPWIN32_FIND_DATA));

    ddstr wsTemp(dirPath);
    if (_DDT('\\') != wsTemp[wsTemp.length() - 1] &&
        _DDT('/') != wsTemp[wsTemp.length() - 1]) {
        wsTemp.append(_DDT("\\"));
    }

    ddstr rootPath(wsTemp);
    wsTemp.append(_DDT("*"));
    hFile = ::FindFirstFile(wsTemp.c_str(), &fileInfo);

    if (INVALID_HANDLE_VALUE == hFile) {
        return false;
    }

    do {
        //如果是当前目录或者是上级目录，就直接进入下一次循环 
        ddstr fileName = fileInfo.cFileName;
        if (fileName == _DDT(".") || fileName == _DDT("..")) {
            continue;
        }

        ddstr subPath = rootPath + fileName;
        if (is_dir(subPath)) {
            if (!delete_dir(subPath)) {
                return false;
            }
        } else {
            if (!delete_file(subPath)) {
                return false;
            }
        }
    } while (::FindNextFile(hFile, &fileInfo));

    ::FindClose(hFile);
    ::RemoveDirectory(rootPath.c_str());
    return true;
}

bool dir_uitls::create_dir(const ddstr& dirPath)
{
    if (is_path_exist(dirPath)) {
        return true;
    }

    return ::CreateDirectory(dirPath.c_str(), nullptr) == TRUE;
}

bool dir_uitls::create_dir_ex(const ddstr& dirPath)
{
    if (dirPath.empty()) {
        return false;
    }

    if (is_path_exist(dirPath)) {
        return true;
    }

    ddstr tmp = _DDT("");
    tmp.resize(dirPath.size(), 0);

    for (size_t i = 0; i < dirPath.length(); ++i) {
        if (dirPath[i] == _DDT('\\') || dirPath[i] == _DDT('/')) {
            if (!create_dir(tmp)) {
                return false;
            }
        }

        tmp[i] = dirPath[i];
    }

    if (tmp[tmp.size() - 1] != _DDT('\\') && tmp[tmp.size() - 1] != _DDT('/')) {
        if (!create_dir(tmp)) {
            return false;
        }
    }
    return true;
}

void dir_uitls::enum_dir(const ddstr& dirPath, std::function<void(const ddstr & path, bool isDir)> callBack)
{
    ddstr rootPath = dirPath;

    if (rootPath.length() < 2) {
        return;
    }

    if (rootPath[rootPath.length() - 1] != _DDT('/') &&
        rootPath[rootPath.length() - 1] != _DDT('\\')) {
        rootPath.append(1, _DDT('\\'));
    }

    if (callBack == nullptr || !is_path_exist(rootPath)) {
        return;
    }

    // 枚举路径
    std::queue<ddstr> dirPathQue;
    dirPathQue.push(rootPath);

    while (!dirPathQue.empty()) {
        std::wstring path = dirPathQue.front();
        std::wstring pathRoot = dirPathQue.front();
        dirPathQue.pop();
        WIN32_FIND_DATA fileInfo;
        ::memset(&fileInfo, 0, sizeof(LPWIN32_FIND_DATA));
        path.append(L"*");
        HANDLE hFile = nullptr;
        hFile = ::FindFirstFile(path.c_str(), &fileInfo);

        if (INVALID_HANDLE_VALUE == hFile) {
            continue;
        }

        do {
            //如果是当前目录或者是上级目录，就直接进入下一次循环  
            ddstr fileName = fileInfo.cFileName;
            if (fileName == _DDT(".") || fileName == _DDT("..")) {
                continue;
            }

            std::wstring subPath = pathRoot + fileName;

            if (is_dir(subPath)) {
                subPath += L"\\";
                dirPathQue.push(subPath);
                callBack(subPath, true);
            } else {
                callBack(subPath, false);
            }
        } while (::FindNextFile(hFile, &fileInfo));

        ::FindClose(hFile);
    }
}

void dir_uitls::enum_dir(const ddstr& dirPath, std::vector<ddstr>& out, std::function<bool(const ddstr&, bool)> filter)
{
    enum_dir(dirPath, [filter, &out](const ddstr& path, bool isDir) {
                if (filter == nullptr || filter(path, isDir)) {
                    out.push_back(path);
                    return;
                }
                return; 
             });
}

END_NSP_DDM
#endif // DD_WINDOW
