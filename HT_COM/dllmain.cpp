// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "LOG.h"
#include "INTERFACE.h"
#include "REGISTER.h"

HMODULE hmodule;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hmodule = hModule;
        Logger::Write("DLL_PROCESS_ATTACH");
        break;
    case DLL_THREAD_ATTACH:
        Logger::Write("DLL_THREAD_ATTACH");
        break;
    case DLL_THREAD_DETACH:
        Logger::Write("DLL_THREAD_DETACH");
        break;
    case DLL_PROCESS_DETACH:
        Logger::Write("DLL_PROCESS_DETACH");
        break;
    }
    return TRUE;
}

///////////

STDAPI DllInstall(BOOL b, PCWSTR s) {
    Logger::Write((b) ? "DllInstall" : "DllUninstall");
    return S_OK;
}
STDAPI DllRegisterServer() {
    Logger::Write("DllRegisterServer ", hmodule);
    HRESULT rc = RegisterServer(hmodule, CLSID_CA, FNAME, VINDX, PRGID);
    Logger::Write("DllRegisterServer");
    return S_OK;
}
STDAPI DllUnregisterServer() {
    Logger::Write("DllUnRegisterServer ", hmodule);
    HRESULT rc = UnregisterServer(CLSID_CA, VINDX, PRGID);
    Logger::Write("DllUnRegisterServer");
    return S_OK;
}

