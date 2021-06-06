#include "win_msg_loop.h"

BEG_NSP_DDM
bool NSP_DDM::win_msg_loop::stop(u32)
{
    if (::PostThreadMessage(m_loopId, WM_QUIT, NULL, NULL)) {
        m_stop = true;
    }
    return m_stop;
}

void win_msg_loop::loop()
{
    m_loopId = ::GetCurrentThreadId();
    while (!m_stop)
    {
        BOOL hr = ::GetMessage(&m_msg, NULL, 0, 0);
        if (hr == 0) {
            m_stop = true; // WM_QUIT, exit message loop
            continue;
        } else if (hr == -1) {
            continue;
        }

        if (filter_msg(&m_msg))
        {
            ::TranslateMessage(&m_msg);
            ::DispatchMessage(&m_msg);
        }
	}
}

bool win_msg_loop::filter_msg(MSG* msg)
{
    (msg);
    return true;
}

END_NSP_DDM