#ifndef dir_utils_h_
#define dir_utils_h_ 1

#include "g_def.h"

#include <functional>
#include <string>
#include <vector>

BEG_NSP_DDM

class dir_utils
{
public:
    /** �Ƿ����ļ���
    @param [in] path ·��
    */
    static bool is_dir(const ddstr& path);

    /** �ж��ļ����ļ����Ƿ����
    @param [in] path ·��
    */
    static bool is_path_exist(const ddstr& filePath);

    /** �����ļ�
    @param [in] filePath �ļ�·��
    */
    static bool create_file(const ddstr& filePath);

    /** ɾ���ļ���
    @param [in] dfilePath �ļ�·��
    */
    static bool delete_file(const ddstr& dfilePath);

    /** ɾ���ļ���
    @param [in] dirPath �ļ���·��
    */
    static bool delete_dir(const ddstr& dirPath);

    /** �����ļ���
    @param [in] dirPath �ļ���·��
    */
    static bool create_dir(const ddstr& dirPath);

    /** �����ļ���(Ƕ�״���)
    @param [in] dirPath �ļ���·��
    */
    static bool create_dir_ex(const ddstr& dirPath);

    /** ö��Ŀ¼ �������
    @param [in] dirPath Ŀ¼·��
    @param [in] callBack �ص���������ÿһ��·�����д���
                @param [in] path ·��
                @param [in] isDir �Ƿ���Ŀ¼
    */
    static void enum_dir(const ddstr& dirPath, std::function<void(const ddstr & path, bool isDir)> callBack);

    /** ö��Ŀ¼ �������
    @param [in] dirPath Ŀ¼·��
    @param [in] out ���·��vector
    @param [in] filter ��ÿһpath���й��ˣ�����true��ŵ�out����У����򶪵���filterΪ��ʱ��������true
                @param [in] path ·��
                @param [in] isDir �Ƿ���Ŀ¼
                @return true ��path�ŵ�out�����false ����
    */
    static void enum_dir(const ddstr& dirPath, std::vector<ddstr>& out, std::function<bool(const ddstr& path, bool isDir)> filter);
};

END_NSP_DDM
#endif // dir_utils_h_