#include "ddperfermence.h"
#include "ddprocess.h"

BEG_NSP_DDM
ddgpu_perfermence::ddgpu_perfermence()
{
    (void)init();
}

ddgpu_perfermence::~ddgpu_perfermence()
{
    if (m_Gdi32DllHandle != NULL) {
        ::FreeLibrary(m_Gdi32DllHandle);
        m_Gdi32DllHandle = NULL;
    }
}

// reference resources : process hacker gpumon.c
bool ddgpu_perfermence::get_gpu_useagedesc(const LUID& gpuAdapterLuid, HANDLE hProcess, ddgpu_perfermence::gpu_useage_desc& desc)
{
    if (m_D3DKMTQueryStatistics == NULL && !init()) {
        return false;
    }

    // GPU 模块个数(任务管理器中可以看到 3D COPY VIDEO ENCODE VIDEO DECODE 等模块)
    int gpuNodeCount = 0;
    {
        D3DKMT_QUERYSTATISTICS stats;
        memset(&stats, 0, sizeof(D3DKMT_QUERYSTATISTICS));
        stats.Type = D3DKMT_QUERYSTATISTICS_ADAPTER;
        stats.AdapterLuid = gpuAdapterLuid;
        if (m_D3DKMTQueryStatistics(&stats) < 0) {
            return false;
        }
        gpuNodeCount = stats.QueryResult.AdapterInformation.NodeCount;
    }

    desc.timePoint = ddtimer::now() / 100;
    for (int i = 0; i < gpuNodeCount; ++i) {
        D3DKMT_QUERYSTATISTICS stats;
        ::memset(&stats, 0, sizeof(D3DKMT_QUERYSTATISTICS));
        stats.Type = D3DKMT_QUERYSTATISTICS_PROCESS_NODE;
        stats.AdapterLuid = gpuAdapterLuid;
        stats.ProcessHandle = hProcess;
        stats.QueryProcessNode.NodeId = i;
        if (m_D3DKMTQueryStatistics(&stats) < 0) {
            continue;
        }

        desc.runningTime += stats.QueryResult.ProcessNodeInformation.RunningTime.QuadPart;
    }

    return true;
}

bool ddgpu_perfermence::get_gpu_useagedesc_byname(const LUID& gpuAdapterLuid, const std::wstring& processName, gpu_useage_desc& gpuUseageDesc)
{
    ddprocess process;
    if (!process.init(processName)) {
        return false;
    }
    return get_gpu_useagedesc(gpuAdapterLuid, process.get_handle(), gpuUseageDesc);
}

bool ddgpu_perfermence::init()
{
    m_Gdi32DllHandle = ::LoadLibrary(L"gdi32.dll");
    if (m_Gdi32DllHandle == NULL) {
        return false;
    }

    m_D3DKMTQueryStatistics = (PD3DKMTQueryStatistics)GetProcAddress(m_Gdi32DllHandle, "D3DKMTQueryStatistics");
    if (m_D3DKMTQueryStatistics == NULL) {
        return false;
    }

    return true;
}

END_NSP_DDM
