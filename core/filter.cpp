#include "filter.h"

#include <algorithm>
#include <cstring>

void gaussian_blur(
  unsigned char *input,
  unsigned char *output,
  int width,
  int height
)
{
  const int channels = 3;

  if (input == nullptr || output == nullptr || width <= 0 || height <= 0) {
    return;
  }

  const int size = width * height * channels;
  if (width < 3 || height < 3) {
    if (input != output) {
      std::memcpy(output, input, static_cast<size_t>(size));
    }
    return;
  }

  if (input != output) {
    std::memcpy(output, input, static_cast<size_t>(size));
  }

  const int stride = width * channels;

  for (int y = 1; y < height - 1; ++y) {
    const int row_begin = y * stride + channels;
    const int row_end = y * stride + (width - 1) * channels;

    for (int i = row_begin; i < row_end; ++i) {
      const int sum =
        input[i - stride - channels] +
        2 * input[i - stride] +
        input[i - stride + channels] +
        2 * input[i - channels] +
        4 * input[i] +
        2 * input[i + channels] +
        input[i + stride - channels] +
        2 * input[i + stride] +
        input[i + stride + channels];

      output[i] = static_cast<unsigned char>(sum / 16);
    }
  }
}
