def patch(f, offset, data):
    f.seek(offset)
    f.write(data)

with open('nvgpucomp32.dll', "r+b") as f:
    patch(f, 0x205B862, b'\x90\x90\x90')
    patch(f, 0x2055DEB, b'\x90\x90\x90')
    patch(f, 0x205551A, b'\xc2\x08\x00')
    patch(f, 0x2055543, b'\xc2\x08\x00')
