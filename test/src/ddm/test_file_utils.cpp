
#include "test_case_factory.h"
#include "file/file_writer.h"
#include "file/file_reader.h"
#include "file/dir_utils.h"
#include "file/ini_file.h"
#include "str/str_utils.h"
#include <iostream>

BEG_NSP_DDM

TEST(test_file_utils, uft16_uft8)
{
    // D:\\workspaces\\C++_workspaces\\MyLibrary1\\bin\\Debug_Win32\\ 

    {
        if (!dir_uitls::is_path_exist(L".\\2.txt")) {
            dir_uitls::create_file(L".\\2.txt");
        }
        sp_file_writer file = create_UCS2_file_writer(L".\\2.txt");
        if (file != nullptr) {
            ddstrw w = L"ÄãºÃ£¬ÎÄ¼þ\r\n";
            file->write_buffw(w.c_str(), (u32)w.length());
            file->write_buffw(w.c_str(), (u32)w.length());
            file->write_buffw(w.c_str(), (u32)w.length());
            file->write_buffw(w.c_str(), (u32)w.length());
            file->write_buffw(w.c_str(), (u32)w.length());
            file->write_buffw(w.c_str(), (u32)w.length());
            file->write_buffw(w.c_str(), (u32)w.length());
            file->write_buffw(w.c_str(), (u32)w.length());
        }

        {
            sp_file_reader file1 = create_UCS2_file_reader(L"2.txt");
            if (file1 != nullptr) {
                ddstrw w;
                while (file1->getlinew(w))
                {
                    ddstra ansi;
                    str_utils::uft16_ansi(w, ansi);
                    std::cout << ansi << std::endl;
                    w.resize(0);
                };
            }
        }
    }
}

TEST(test_file_utils, create_dir_ex)
{
    dir_uitls::create_dir_ex(_DDT("E:\\aa\\bb\\cc"));
    dir_uitls::create_dir_ex(_DDT("E:\\aa\\bb1\\cc1\\"));
    dir_uitls::create_dir_ex(_DDT("E:/aa/bb2\\cc2\\"));
    dir_uitls::create_dir_ex(_DDT("E:/aa/bb2/cc2/"));
}

TEST(test_file_utils, enum_dir)
{
    ddstr dirPath = L"E:\\VM";
    std::vector<ddstr> pathAllVec;
    dir_uitls::enum_dir(dirPath, pathAllVec, nullptr);

    std::vector<ddstr> pathAllDir;
    dir_uitls::enum_dir(dirPath, pathAllDir, [](const ddstr&, bool isDir) {
        if (isDir) {
            return true;
        }

        return false;
    });

    std::vector<ddstr> pathAllFile;
    dir_uitls::enum_dir(dirPath, pathAllFile, [](const ddstr&, bool isDir) {
        if (isDir) {
            return false;
        }

        return true;
    });

    std::vector<ddstr> pathAllTxt;
    dir_uitls::enum_dir(dirPath, pathAllTxt, [](const ddstr& path, bool isDir) {
        if (isDir) {
            return false;
        }

        if (path.length() >= 4 &&
            path[path.length() - 4] == L'.' &&
            path[path.length() - 3] == L't' &&
            path[path.length() - 2] == L'x' &&
            path[path.length() - 1] == L't') {
            return true;
        }

        return false;
    });
}

TEST(test_file_utils, ini_file)
{
    auto sp_ini_file = ini_file::create_obj(L"E:\\aa\\1.ini", true);

    if (!sp_ini_file) {
        return;
    }

    sp_ini_file->add_key(L"Sec1", L"key1", L"1");
    sp_ini_file->add_key(L"Sec1", L"key2", L"1");
    sp_ini_file->add_key(L"Sec1", L"key3", L"1");
    sp_ini_file->add_key(L"Sec1", L"key4", L"1");

    sp_ini_file->add_key(L"Sec2", L"key1", L"1");
    sp_ini_file->add_key(L"Sec2", L"key2", L"1");
    sp_ini_file->add_key(L"Sec2", L"key3", L"1");
    sp_ini_file->add_key(L"Sec2", L"key4", L"1");

    sp_ini_file->del_key(L"Sec1", L"key1");
    sp_ini_file->del_section(L"sec2");
    sp_ini_file->chang_key(L"sec1", L"key2", L"2");

    std::vector<ddstr> ss = sp_ini_file->get(L"sec1", L"key2");
    std::vector<ddstr> ss2 = sp_ini_file->get(L"sec1", 0);
    std::vector<ddstr> ss1 = sp_ini_file->get(0, 0);
}
END_NSP_DDM
