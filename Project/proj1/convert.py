from PIL import Image

image_path = "./inp.jpg"
output_path = "./studentOutputs/out.ppm"

img = Image.open(image_path).convert('RGB')
height, width = img.size

pixels = map(lambda t: f"{t[0]:>4}{t[1]:>4}{t[2]:>4}", img.getdata())
# print(next(pixels))

with open(output_path, "w", encoding="utf-8") as f:
    print("P3", file=f)
    print(width, height, file=f)
    print("255", file=f)
    for row in range(height):
        print("    ".join([next(pixels) for col in range(width)]), file=f)
