#include "ddperfermence.h"
#include "ddprocess.h"
#include "ddtimer.h"
#include "windows/ddwindows_version.h"

#include "ddperfermence_gpu_ntd3dkmt.h"

typedef NTSTATUS(APIENTRY* PD3DKMTQueryStatistics)(_In_ CONST D3DKMT_QUERYSTATISTICS*);

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

#define WINDOWS_10_RS4 ddversion(10, 0, 1803, 17025)
static void get_gpu_memlimit(PD3DKMTQueryStatistics D3DKMTQueryStatistics, int gpuSegmentCount, const LUID& gpuAdapterLuid, std::vector<bool>& isSharedBitSet, ULONG64& sharedLimit, ULONG64& privateLimit)
{
    ddversion windowsVersion = DDGetWindowsVersion();
    isSharedBitSet.resize(gpuSegmentCount, false);
    for (int i = 0; i < gpuSegmentCount; i++) {
        D3DKMT_QUERYSTATISTICS stats;
        ::memset(&stats, 0, sizeof(D3DKMT_QUERYSTATISTICS));
        stats.Type = D3DKMT_QUERYSTATISTICS_SEGMENT;
        stats.AdapterLuid = gpuAdapterLuid;
        stats.QuerySegment.SegmentId = i;

        if (D3DKMTQueryStatistics(&stats) < 0) {
            continue;
        }

        ULONG64 commitLimit;
        bool isShared = false;
        if (windowsVersion >= DDVERSION_MIN_WIN8) {
            commitLimit = stats.QueryResult.SegmentInformation.CommitLimit;
            isShared = (stats.QueryResult.SegmentInformation.Aperture > 0);
        } else {
            commitLimit = stats.QueryResult.SegmentInformationV1.CommitLimit;
            isShared = (stats.QueryResult.SegmentInformationV1.Aperture > 0);
        }

        if (windowsVersion < WINDOWS_10_RS4) {
            if (isShared) {
                sharedLimit += commitLimit;
            } else {
                privateLimit += commitLimit;
            }
        }

        if (isShared) {
            isSharedBitSet[i] = true;
        }
    }
}

static bool get_gpu_mem_useagedesc(PD3DKMTQueryStatistics D3DKMTQueryStatistics, int gpuSegmentCount, const LUID& gpuAdapterLuid, HANDLE hProcess, ddgpu_perfermence::gpu_useage_desc& gpuUseageDesc)
{
    std::vector<bool> bitset(gpuSegmentCount, false);
    ULONG64 sharedLimit = 0;
    ULONG64 privateLimit = 0;
    get_gpu_memlimit(D3DKMTQueryStatistics, gpuSegmentCount, gpuAdapterLuid, bitset, sharedLimit, privateLimit);

    for (int i = 0; i < gpuSegmentCount; i++)
    {
        D3DKMT_QUERYSTATISTICS stats;
        ::memset(&stats, 0, sizeof(D3DKMT_QUERYSTATISTICS));
        stats.Type = D3DKMT_QUERYSTATISTICS_PROCESS_SEGMENT;
        stats.AdapterLuid = gpuAdapterLuid;
        stats.ProcessHandle = hProcess;
        stats.QueryProcessSegment.SegmentId = i;
        if (D3DKMTQueryStatistics(&stats) < 0) {
            continue;
        }

        ULONG64 bytesCommitted = stats.QueryResult.ProcessSegmentInformation.BytesCommitted;
        if (bitset[i]) {
            gpuUseageDesc.mem_share_usage += bytesCommitted;
        } else {
            gpuUseageDesc.mem_usage += bytesCommitted;
        }
    }

    if (gpuUseageDesc.mem_usage != 0) {
        D3DKMT_QUERYSTATISTICS stats;
        ::memset(&stats, 0, sizeof(D3DKMT_QUERYSTATISTICS));
        stats.Type = D3DKMT_QUERYSTATISTICS_PROCESS;
        stats.AdapterLuid = gpuAdapterLuid;
        stats.ProcessHandle = hProcess;

        if (D3DKMTQueryStatistics(&stats) >= 0) {
            gpuUseageDesc.mem_commit_usage += stats.QueryResult.ProcessInformation.SystemMemory.BytesAllocated;
        }
    }
    return true;
}

static bool get_gpu_gpu_useagedesc(PD3DKMTQueryStatistics D3DKMTQueryStatistics, int gpuNodeCount, const LUID& gpuAdapterLuid, HANDLE hProcess, ddgpu_perfermence::gpu_useage_desc& gpuUseageDesc)
{
    for (int i = 0; i < gpuNodeCount; ++i) {
        D3DKMT_QUERYSTATISTICS stats;
        ::memset(&stats, 0, sizeof(D3DKMT_QUERYSTATISTICS));
        stats.Type = D3DKMT_QUERYSTATISTICS_PROCESS_NODE;
        stats.AdapterLuid = gpuAdapterLuid;
        stats.ProcessHandle = hProcess;
        stats.QueryProcessNode.NodeId = i;
        if (D3DKMTQueryStatistics(&stats) < 0) {
            continue;
        }

        gpuUseageDesc.runningTime += stats.QueryResult.ProcessNodeInformation.RunningTime.QuadPart;
    }
    gpuUseageDesc.timePoint = ddtimer::now() / 100;
    return true;
}

bool ddgpu_perfermence::get_gpu_useagedesc(const LUID& gpuAdapterLuid, HANDLE hProcess, ddgpu_perfermence::gpu_useage_desc& gpuUseageDesc)
{
    if (m_D3DKMTQueryStatistics == NULL && !init()) {
        return false;
    }

    // GPU ģ�����(����������п��Կ��� 3D COPY VIDEO ENCODE VIDEO DECODE ��ģ��)
    int gpuNodeCount = 0;
    int gpuSegmentCount = 0;
    {
        D3DKMT_QUERYSTATISTICS stats;
        memset(&stats, 0, sizeof(D3DKMT_QUERYSTATISTICS));
        stats.Type = D3DKMT_QUERYSTATISTICS_ADAPTER;
        stats.AdapterLuid = gpuAdapterLuid;
        if (((PD3DKMTQueryStatistics)m_D3DKMTQueryStatistics)(&stats) < 0) {
            return false;
        }
        gpuNodeCount = stats.QueryResult.AdapterInformation.NodeCount;
        gpuSegmentCount = stats.QueryResult.AdapterInformation.NbSegments;
    }

    if (!get_gpu_mem_useagedesc((PD3DKMTQueryStatistics)m_D3DKMTQueryStatistics, gpuSegmentCount, gpuAdapterLuid, hProcess, gpuUseageDesc)) {
        return false;
    }

    if (!get_gpu_gpu_useagedesc((PD3DKMTQueryStatistics)m_D3DKMTQueryStatistics, gpuNodeCount, gpuAdapterLuid, hProcess, gpuUseageDesc)) {
        return false;
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