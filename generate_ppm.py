width = 256
height = 256

with open("sample.ppm", "wb") as f:
    f.write(f"P6\n{width} {height}\n255\n".encode())
    for y in range(height):
        for x in range(width):
            r = x % 256
            g = y % 256
            b = 128
            f.write(bytes([r, g, b]))
