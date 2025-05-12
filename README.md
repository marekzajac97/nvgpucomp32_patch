# nvgpucomp32 patch

Nvidia display driver update 576.02 causes all 32-bit games that use DXVK to CTD, this patches the bug in `nvgpucomp32.dll` which is the source of the crash. I'll try to keep it up to date with driver releases until nvidia fixes their shit.

# How to install

- download the patched DLL from [Releases](https://github.com/marekzajac97/nvgpucomp32_patch/releases) that is compatible with your installed driver version.
- enable [DLL redirection](https://learn.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-redirection), run the below command in command prompt as admin:

```sh
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options" /v DevOverrideEnable /t REG_DWORD /d 1
```

- restart the PC for changes to take effect
- create a folder in the same place your game executable is located and name it the same as the executable with the `.local` suffix added. So if your game is `YourGame.exe` make a folder `YourGame.exe.local` and place the patched `nvgpucomp32.dll` in there.

NOTE: The patch is only needed for the first launch of the game, after that you can delete `.local` the folder and disable DLL redirection if that causes issues, it should work until the driver is reinstalled.
