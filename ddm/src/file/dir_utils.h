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
    /** 是否是文件夹
    @param [in] path 路径
    */
    static bool is_dir(const ddstr& path);

    /** 判断文件或文件夹是否存在
    @param [in] path 路径
    */
    static bool is_path_exist(const ddstr& filePath);

    /** 创建文件
    @param [in] filePath 文件路径
    */
    static bool create_file(const ddstr& filePath);

    /** 删除文件夹
    @param [in] dfilePath 文件路径
    */
    static bool delete_file(const ddstr& dfilePath);

    /** 删除文件夹
    @param [in] dirPath 文件夹路径
    */
    static bool delete_dir(const ddstr& dirPath);

    /** 创建文件夹
    @param [in] dirPath 文件夹路径
    */
    static bool create_dir(const ddstr& dirPath);

    /** 创建文件夹(嵌套创建)
    @param [in] dirPath 文件夹路径
    */
    static bool create_dir_ex(const ddstr& dirPath);

    /** 枚举目录 广度优先
    @param [in] dirPath 目录路径
    @param [in] callBack 回调函数，对每一个路径进行处理
                @param [in] path 路径
                @param [in] isDir 是否是目录
    */
    static void enum_dir(const ddstr& dirPath, std::function<void(const ddstr & path, bool isDir)> callBack);

    /** 枚举目录 广度优先
    @param [in] dirPath 目录路径
    @param [in] out 输出路径vector
    @param [in] filter 对每一path进行过滤，返回true这放到out结果中，否则丢掉。filter为空时候当作返回true
                @param [in] path 路径
                @param [in] isDir 是否是目录
                @return true 将path放到out结果，false 丢掉
    */
    static void enum_dir(const ddstr& dirPath, std::vector<ddstr>& out, std::function<bool(const ddstr& path, bool isDir)> filter);
};

END_NSP_DDM
#endif // dir_utils_h_