#ifndef ddperfermence_h_
#define ddperfermence_h_
#include "ddperfermence_gpu_ntd3dkmt.h"

#include "g_def.h"
#include "ddtimer.h"

typedef NTSTATUS(APIENTRY* PD3DKMTQueryStatistics)(_In_ CONST D3DKMT_QUERYSTATISTICS*);
BEG_NSP_DDM

class ddgpu_perfermence
{
public:
    ddgpu_perfermence();
    ~ddgpu_perfermence();
    struct gpu_useage_desc
    {
        LONGLONG runningTime = 0; // 程序在gpu上总共运行的时间 100ns
        LONGLONG timePoint = 0;   // 当前记录时候的时间戳 100ns

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
    PD3DKMTQueryStatistics m_D3DKMTQueryStatistics = NULL;
};

END_NSP_DDM
#endif
