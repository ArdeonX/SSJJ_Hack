
#include "GloBals.h"
void Entry();

void Begin() {

    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Entry, NULL, NULL, NULL);
    return;
}
BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        RemovePeHeader(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Begin, NULL, NULL, NULL);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

