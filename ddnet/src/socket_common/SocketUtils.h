
#ifndef __SOCKET_UTILS_H_
#define __SOCKET_UTILS_H_

#include "typedef/DogString.h"

#include <vector>

class SocketUtils
{
public:
    /** 返回程序所运行的机器的主机名字
    @param [out] name 返回你程 序所运行的机器的主机名字
    @return 是否成功
    */
    static bool GetHostName(std::string& name);

    /** 通过机器的主机名字返回机器的IP地址
    @param [int] name 域名
    @param [out] ips 一组ip
    */
    static void GetIpByHostName(const std::string& name, std::vector<DogStringA >& ips);
};

#endif //__SOCKET_UTILS_H_
