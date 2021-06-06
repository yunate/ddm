
#ifndef __SOCKET_UTILS_H_
#define __SOCKET_UTILS_H_

#include "typedef/DogString.h"

#include <vector>

class SocketUtils
{
public:
    /** ���س��������еĻ�������������
    @param [out] name ������� �������еĻ�������������
    @return �Ƿ�ɹ�
    */
    static bool GetHostName(std::string& name);

    /** ͨ���������������ַ��ػ�����IP��ַ
    @param [int] name ����
    @param [out] ips һ��ip
    */
    static void GetIpByHostName(const std::string& name, std::vector<DogStringA >& ips);
};

#endif //__SOCKET_UTILS_H_
