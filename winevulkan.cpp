#include "pch.h"
#include <Windows.h>

#include <chrono>
#include <fstream>


std::ofstream g_logfile;
void __cdecl log(const char* fmt, ...);

template<size_t N>
bool writeMem(uintptr_t dest, const uint8_t (&source) [N]) {
    DWORD curProtection;
    if (!VirtualProtect((void*)dest, N, PAGE_EXECUTE_READWRITE, &curProtection)) {
        log("ERROR: VirtualProtect failed to override protection, error code: %i. aborting...", GetLastError());
        return false;
    }

    memcpy((void*)dest, source, N);

    DWORD temp;
    if (!VirtualProtect((void*)dest, N, curProtection, &temp)) {
        log("ERROR: VirtualProtect failed to restore protection, error code: %i. aborting...", GetLastError());
        return false;
    }

    return true;
}

void patch_nvgpucomp32() {
    HMODULE hm = GetModuleHandle(L"nvgpucomp32.dll");
    if (hm == 0)
    {
        fprintf(stderr, "GetModuleHandle failed, error = %d\n", GetLastError());
        return;
    }
    log("nvgpucomp32.dll hModule %p", hm);
    uintptr_t base = (uintptr_t)hm;

    // TODO make this configurable!
    writeMem(base + 0x205C4D2, { 0x90, 0x90, 0x90 });
    writeMem(base + 0x2056A5B, { 0x90, 0x90, 0x90 });
    writeMem(base + 0x205618A, { 0xC2, 0x08, 0x00 });
    writeMem(base + 0x20561B3, { 0xC2, 0x08, 0x00 });
}

#define VKAPI_PTR __stdcall
typedef void* VkInstance;

HMODULE g_vulkan1Dll = NULL;
void* (VKAPI_PTR* g_vkGetInstanceProcAddr)(VkInstance, const char*) = nullptr;
void* (VKAPI_PTR* g_vkGetInstanceProcAddr_hook)(VkInstance, const char*) = nullptr;

void* VKAPI_PTR vkGetInstanceProcAddr_hook(VkInstance instance, const char* proc)
{
    log("vkGetInstanceProcAddr(%p, %s)", instance, proc);
    void* ret = g_vkGetInstanceProcAddr(instance, proc);
    if (!strcmp("vkCreateDevice", proc)) {
        patch_nvgpucomp32();
        // remove hook
        g_vkGetInstanceProcAddr_hook = g_vkGetInstanceProcAddr;
    }
    return ret;
}

void init() {

#ifdef _DEBUG
    g_logfile.open("nvgpucomp32_patcher_log.txt", std::ios::app);
#endif

    g_vulkan1Dll = LoadLibraryA("vulkan-1.dll");

    if (NULL == g_vulkan1Dll) {
        log("LoadLibraryA failed, error = %d\n", GetLastError());
        return;
    }

    g_vkGetInstanceProcAddr = (void* (VKAPI_PTR*)(VkInstance, const char*))
        GetProcAddress(g_vulkan1Dll, "vkGetInstanceProcAddr");

    if (nullptr == g_vkGetInstanceProcAddr) {
        log("GetProcAddress(vkGetInstanceProcAddr) not found");
        FreeLibrary(g_vulkan1Dll);
        return;
    }

    g_vkGetInstanceProcAddr_hook = vkGetInstanceProcAddr_hook;

    log("initialized");
}


extern "C" __declspec (dllexport)
void* VKAPI_PTR vkGetInstanceProcAddr(VkInstance instance, const char* proc)
{
    return g_vkGetInstanceProcAddr_hook(instance, proc);
}


#ifdef _DEBUG
void __cdecl log(const char* fmt, ...)
{
    if (g_logfile.is_open())
    {
        if (!fmt) { return; }

        va_list va_alist;

        auto now = std::chrono::system_clock::now();
        auto sec = std::chrono::time_point_cast<std::chrono::seconds>(now);
        auto rest = now - sec;
        auto mili = std::chrono::duration_cast<std::chrono::milliseconds>(rest);

        char timestamp_buf[256]{ 0 };
        time_t t = std::chrono::system_clock::to_time_t(now);
        struct tm lt;
        localtime_s(&lt, &t);
        snprintf(timestamp_buf, 256, "%02d/%02d/%02d %02d:%02d:%02d.%03d", lt.tm_mon + 1, lt.tm_mday, lt.tm_year % 100, lt.tm_hour, lt.tm_min, lt.tm_sec, (int)mili.count());

        char logbuf[256] = { 0 };

        va_start(va_alist, fmt);
        _vsnprintf_s(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), 256, fmt, va_alist);
        va_end(va_alist);

        g_logfile << "[" << timestamp_buf << "] " << logbuf << std::endl;
    }
}
#else
void __cdecl log(const char* fmt, ...) {}
#endif // DEBUG

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        init();
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
