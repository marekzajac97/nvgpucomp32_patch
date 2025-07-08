def patch(f, offset, data):
    f.seek(offset)
    f.write(data)

with open('nvgpucomp32.dll', "r+b") as f:
    patch(f, 0x205B8D2, b'\x90\x90\x90')
    patch(f, 0x2055E5B, b'\x90\x90\x90')
    patch(f, 0x205558A, b'\xc2\x08\x00')
    patch(f, 0x20555B3, b'\xc2\x08\x00')
