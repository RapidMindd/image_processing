#ifndef FILTER_H
#define FILTER_H

extern "C" void gaussian_blur(
  unsigned char *input,
  unsigned char *output,
  int width,
  int height
);

#endif
