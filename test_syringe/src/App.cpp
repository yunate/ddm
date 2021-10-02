#include "App.h"

void App::on_dll_attach()
{
    ::MessageBox(0, L"dll attach", L"title", 0);
}

void App::on_dll_deach()
{
    ::MessageBox(0, L"dll deach", L"title", 0);
}
