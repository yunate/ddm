#ifndef app_h_
#define app_h_

#include "ddinclude.h"
using namespace NSP_DDM;

class App : private nocopyable
{
protected:
    App() = default;
    ~App() = default;

public:
    void on_dll_attach();
    void on_dll_deach();
};

#define APP (singleton<App>::get_instance())
#endif
