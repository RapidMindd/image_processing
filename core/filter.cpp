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

  const int kernel[3][3] = {
    {1, 2, 1},
    {2, 4, 2},
    {1, 2, 1}
  };

  for (int y = 1; y < height - 1; ++y) {
    for (int x = 1; x < width - 1; ++x) {
      for (int c = 0; c < channels; ++c) {
        int sum = 0;

        for (int dy = -1; dy <= 1; ++dy) {
          for (int dx = -1; dx <= 1; ++dx) {
            const int source_index = ((y + dy) * width + (x + dx)) * channels + c;
            sum += input[source_index] * kernel[dy + 1][dx + 1];
          }
        }

        const int target_index = (y * width + x) * channels + c;
        output[target_index] = static_cast<unsigned char>(sum / 16);
      }
    }
  }
}
