#include "../core/filter.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

int read_int_arg(char **argv, int argc, int index, int default_value)
{
  if (index >= argc) {
    return default_value;
  }

  const int value = std::atoi(argv[index]);
  return value > 0 ? value : default_value;
}

void fill_test_image(std::vector<unsigned char> &image)
{
  for (size_t i = 0; i < image.size(); ++i) {
    image[i] = static_cast<unsigned char>((i * 37 + 17) % 256);
  }
}

unsigned long long checksum(const std::vector<unsigned char> &image)
{
  unsigned long long sum = 0;

  for (size_t i = 0; i < image.size(); ++i) {
    sum += image[i];
  }

  return sum;
}

int main(int argc, char **argv)
{
  const int width = read_int_arg(argv, argc, 1, 1024);
  const int height = read_int_arg(argv, argc, 2, 1024);
  const int iterations = read_int_arg(argv, argc, 3, 10);
  const int complexity = read_int_arg(argv, argc, 4, 35);
  const int channels = 3;

  std::vector<unsigned char> input(width * height * channels);
  std::vector<unsigned char> output(width * height * channels);

  fill_test_image(input);

  const std::chrono::high_resolution_clock::time_point start =
    std::chrono::high_resolution_clock::now();

  for (int i = 0; i < iterations; ++i) {
    gaussian_blur(input.data(), output.data(), width, height);
  }

  const std::chrono::high_resolution_clock::time_point finish =
    std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> elapsed = finish - start;
  const double seconds = elapsed.count();
  const double perf = 1.0 / seconds;
  const int efficiency = static_cast<int>((perf / complexity) * 1000000.0);

  std::cout << "width: " << width << '\n';
  std::cout << "height: " << height << '\n';
  std::cout << "channels: " << channels << '\n';
  std::cout << "iterations: " << iterations << '\n';
  std::cout << "complexity: " << complexity << '\n';
  std::cout << "time_seconds: " << seconds << '\n';
  std::cout << "perf: " << perf << '\n';
  std::cout << "efficiency: " << efficiency << '\n';
  std::cout << "checksum: " << checksum(output) << '\n';

  return 0;
}
