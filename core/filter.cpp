#include "filter.h"
#include <algorithm>
#include <cstring>
#include <cstdlib>

constexpr size_t ALIGNMENT = 32;

extern "C" void gaussian_blur(
  unsigned char * __restrict__ input,
  unsigned char * __restrict__ output,
  int width,
  int height
)
{
    if (input == nullptr || output == nullptr || width <= 0 || height <= 0) {
        return;
    }

    const int channels = 3;
    const size_t size = static_cast<size_t>(width) * height * channels;

    unsigned char* aligned_in = static_cast<unsigned char*>(std::aligned_alloc(ALIGNMENT, size));
    if (!aligned_in) return;

    std::memcpy(aligned_in, input, size);

    if (width < 3 || height < 3) {
        if (input != output) std::memcpy(output, input, size);
        return;
    }

    unsigned char* __restrict__ p_in = (unsigned char*)__builtin_assume_aligned(aligned_in, ALIGNMENT);
    unsigned char* __restrict__ p_out = (unsigned char*)__builtin_assume_aligned(output, ALIGNMENT);

    const int stride = width * channels;

    #pragma omp parallel for
    for (int y = 1; y < height - 1; ++y) {
        const int row_offset = y * stride;

        #pragma omp simd
        for (int i = channels; i < stride - channels; ++i) {
            const int idx = row_offset + i;
            const int sum =
                p_in[idx - stride - channels] + 2 * p_in[idx - stride]
                + p_in[idx - stride + channels] + 2 * p_in[idx - channels]
                + 4 * p_in[idx] + 2 * p_in[idx + channels]
                + p_in[idx + stride - channels] + 2 * p_in[idx + stride]
                + p_in[idx + stride + channels];

            p_out[idx] = static_cast<unsigned char>(sum >> 4);
        }
    }
}
