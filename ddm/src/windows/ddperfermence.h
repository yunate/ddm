// reference resources : process hacker gpumon.c

#ifndef ddperfermence_h_
#define ddperfermence_h_

#include "g_def.h"

#include <windows.h>
BEG_NSP_DDM

class ddgpu_perfermence
{
public:
    ddgpu_perfermence();
    ~ddgpu_perfermence();
    struct gpu_useage_desc
    {
        LONGLONG timePoint = 0;         // ��ǰ��¼ʱ���ʱ��� (100ns)
        LONGLONG runningTime = 0;       // ������gpu���ܹ����е�ʱ�� (100ns)
        LONGLONG mem_usage = 0;            // ˽���ڴ�ʹ�� ��(b)
        LONGLONG mem_share_usage = 0;      // �����ڴ�ʹ�� (b)
        LONGLONG mem_commit_usage = 0;     // �ύ�ڴ�ʹ�� (b)

        static inline double get_usage(const gpu_useage_desc& pre, const gpu_useage_desc& next)
        {
            if (next.timePoint == pre.timePoint) {
                return 0.0f;
            }
            return ((double)(next.runningTime - pre.runningTime)) / ((double)(next.timePoint - pre.timePoint));
        }
    };

    bool get_gpu_useagedesc(const LUID& gpuAdapterLuid, HANDLE hProcess, gpu_useage_desc& gpuUseageDesc);
    bool get_gpu_useagedesc_byname(const LUID& gpuAdapterLuid, const std::wstring& processName, gpu_useage_desc& gpuUseageDesc);

protected:
    bool init();

protected:
    HMODULE m_Gdi32DllHandle = NULL;
    void* m_D3DKMTQueryStatistics = NULL;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ddcpu_perfermence
{
public:
    struct cpu_useage_desc
    {
        u64 timePoint = 0;            // ��ǰ��¼ʱ���ʱ��� (100ns)
        u64 runningTime = 0;          // ������cpu���ܹ����е�ʱ��(�ں�+�û�) (100ns)
        u64 readBytes = 0;            // �����ܹ������ֽ���
        u64 writeBytes = 0;           // �����ܹ�д���ֽ���
        u64 mem_usage = 0;            // �ڴ�ʹ�� (b)
        u64 mem_private_usage = 0;    // ˽���ڴ�ʹ�õġ�(b)

        static inline double get_cpu_usage(const cpu_useage_desc& pre, const cpu_useage_desc& next)
        {
            if (next.timePoint == pre.timePoint) {
                return 0.0f;
            }
            return ((double)(next.runningTime - pre.runningTime)) / ((double)(next.timePoint - pre.timePoint));
        }

        static inline double get_io_read_usage(const cpu_useage_desc& pre, const cpu_useage_desc& next)
        {
            if (next.timePoint == pre.timePoint) {
                return 0.0f;
            }
            return ((double)(next.readBytes - pre.readBytes)) * 10000000 / ((double)(next.timePoint - pre.timePoint));
        }

        static inline double get_io_write_usage(const cpu_useage_desc& pre, const cpu_useage_desc& next)
        {
            if (next.timePoint == pre.timePoint) {
                return 0.0f;
            }
            return ((double)(next.writeBytes - pre.writeBytes)) * 10000000 / ((double)(next.timePoint - pre.timePoint));
        }
    };

    static bool get_cpu_useage_desc(HANDLE hProcess, cpu_useage_desc& cpuUsageDesc);
    static bool get_cpu_useage_desc_byname(const std::wstring& processName, cpu_useage_desc& cpuUsageDesc);
};

END_NSP_DDM
#endif
