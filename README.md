# nvgpucomp32 patch

Nvidia display driver update 576.02 causes all 32-bit games that use DXVK to CTD, this patches the bug in `nvgpucomp32.dll` which is the source of the crash. The patch only works with the latest 576.28 driver. I'll try to keep it updated until nvidia fixes their shit.

# How to install

- make sure you are on the latest driver 576.28
- download the patched DLL from [Releases](https://github.com/marekzajac97/nvgpucomp32_patch/releases)
- enable [DLL redirection](https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-redirection), run the below command in command prompt as admin:

`reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options" /v DevOverrideEnable /t REG_DWORD /d 1`

- restart the PC
- create a folder in the same place your game executable is located and name it the same as the executable with `.local` suffix added. So if your game is `YourGame.exe` make a folder `YourGame.exe.local` and place the patched `nvgpucomp32.dll` in there.
