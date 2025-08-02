# With the release of the driver version 580.88 the issue has been resolved by Nvidia, this fix will no longer be maintained.

# nvgpucomp32 patch

Nvidia display driver update 576.02 causes all 32-bit games that use DXVK to CTD on 50xx series GPUs. This patch fixes the bug in `nvgpucomp32.dll`, which is the source of the crash. I'll try to keep it up to date with driver releases until nvidia fixes their shit.

# How to install

- download `winevulkan.dll` from [Releases](https://github.com/marekzajac97/nvgpucomp32_patch/releases) that is compatible with your installed driver version and put it inside the game's installation directory.
