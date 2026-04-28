import ctypes
from pathlib import Path

from PIL import Image


ROOT_DIR = Path(__file__).resolve().parent.parent
LIB_PATH = ROOT_DIR / "libfilter.so"


def load_filter_library():
    if not LIB_PATH.exists():
        raise FileNotFoundError(
            "libfilter.so не найдена"
        )

    library = ctypes.CDLL(str(LIB_PATH))
    library.gaussian_blur.argtypes = [
        ctypes.POINTER(ctypes.c_ubyte),
        ctypes.POINTER(ctypes.c_ubyte),
        ctypes.c_int,
        ctypes.c_int,
    ]
    library.gaussian_blur.restype = None
    return library


def blur_image(image_path):
    library = load_filter_library()

    image = Image.open(image_path).convert("RGB")
    width, height = image.size

    input_data = image.tobytes()
    output_data = bytearray(len(input_data))

    input_buffer = (ctypes.c_ubyte * len(input_data)).from_buffer_copy(input_data)
    output_buffer = (ctypes.c_ubyte * len(output_data)).from_buffer(output_data)

    library.gaussian_blur(input_buffer, output_buffer, width, height)

    return Image.frombytes("RGB", (width, height), bytes(output_data))


def main():
    image_path = input("Введите путь к изображению: ").strip()

    if not image_path:
      print("Путь к изображению не указан")
      return

    try:
        result = blur_image(image_path)
    except Exception as error:
        print("Ошибка:", error)
        return

    result.show()


if __name__ == "__main__":
    main()
