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
        LONGLONG mem_usage = 0;            // ˽���ڴ�ʹ�� (b)
        LONGLONG mem_share_usage = 0;      // �����ڴ�ʹ�� (b)
        LONGLONG mem_commit_usage = 0;     // �ύ�ڴ�ʹ�� (b)

        static inline double get_usage(const gpu_useage_desc& pre, const gpu_useage_desc& next)
        {
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

END_NSP_DDM
#endif
