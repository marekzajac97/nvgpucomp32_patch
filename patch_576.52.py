def patch(f, offset, data):
    f.seek(offset)
    f.write(data)

with open('nvgpucomp32.dll', "r+b") as f:
    patch(f, 0x205B752, b'\x90\x90\x90')
    patch(f, 0x2055CCB, b'\x90\x90\x90')
    patch(f, 0x20553FA, b'\xc2\x08\x00')
    patch(f, 0x2055423, b'\xc2\x08\x00')
