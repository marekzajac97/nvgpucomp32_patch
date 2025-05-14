# nvgpucomp32 patch

Nvidia display driver update 576.02 causes all 32-bit games that use DXVK to CTD on 50xx series GPUs. This patch fixes the bug in `nvgpucomp32.dll`, which is the source of the crash. I'll try to keep it up to date with driver releases until nvidia fixes their shit.

# How to install

- download the patched DLL from [Releases](https://github.com/marekzajac97/nvgpucomp32_patch/releases) that is compatible with your installed driver version.
- enable [DLL redirection](https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-redirection), run the below command in the command prompt as admin:

```sh
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options" /v DevOverrideEnable /t REG_DWORD /d 1
```

- restart the PC for changes to take effect
- create a folder in the same place your game executable is located and name it the same as the executable with the `.local` suffix added. So if your game is `YourGame.exe` make a folder `YourGame.exe.local` and place the patched `nvgpucomp32.dll` inside there.

# Troubleshoot

The game still refuses to launch with the patch installed? Check if it launches without DXVK installed. If it doesn't, it's likely that DLL redirection is causing issues.

Enabling it changes the order of searching for DLLs and checks the game's installation directory first, but some DLLs there may be outdated or broken! If any such DLLs are already installed system-wide in `C:\Windows\SysWOW64` (e.g. `dinput8.dll`, `msvcrt.dll`, etc), remove them from the game's directory (or rename them).

NOTE: The patch is only needed for the game's first launch. After that, you can delete the `.local` folder and disable DLL redirection if that causes problems. The game should work until the driver is reinstalled.
