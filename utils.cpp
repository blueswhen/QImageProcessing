// Copyright 2014-4 sxniu

#include "include/utils.h"
#include <stdio.h>

namespace utils {

bool SetArroundPixelsPosition(int* arround_pixels_pos, int center_pos_x,
                              int center_pos_y, int number_of_pixels,
                              int width, int height) {
  if (center_pos_x == 0 ||
      center_pos_x == width - 1 ||
      center_pos_y == 0 ||
      center_pos_y == height - 1) {
    printf("%s", "error: the center pixel is on the board of image\n");
    return false;
  }

  if (number_of_pixels == 8) {
    arround_pixels_pos[0] = center_pos_y * width + center_pos_x + 1;
    arround_pixels_pos[1] = (center_pos_y + 1) * width + center_pos_x + 1;
    arround_pixels_pos[2] = (center_pos_y + 1) * width + center_pos_x;
    arround_pixels_pos[3] = (center_pos_y + 1) * width + center_pos_x - 1;
    arround_pixels_pos[4] = center_pos_y * width + center_pos_x - 1;
    arround_pixels_pos[5] = (center_pos_y - 1) * width + center_pos_x - 1;
    arround_pixels_pos[6] = (center_pos_y - 1) * width + center_pos_x;
    arround_pixels_pos[7] = (center_pos_y - 1) * width + center_pos_x + 1;
  } else {
    printf("%s", "error: this number of pixels is not supported\n");
    return false;
  }
  return true;
}

}
