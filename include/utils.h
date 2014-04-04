// Copyright 2014-4 sxniu
#ifndef QIMAGEPROCESSING_INCLUDE_UTILS_H_
#define QIMAGEPROCESSING_INCLUDE_UTILS_H_

#define EIGHT_ARROUND_POSITION(center_x, center_y, width, height) \
{ \
  center_y * width + center_x + 1, \
  (center_y + 1) * width + center_x + 1, \
  (center_y + 1) * width + center_x, \
  (center_y + 1) * width + center_x - 1, \
  center_y * width + center_x - 1, \
  (center_y - 1) * width + center_x - 1, \
  (center_y - 1) * width + center_x, \
  (center_y - 1) * width + center_x + 1, \
}

namespace utils {
}  // namespace utils

#endif  // QIMAGEPROCESSING_INCLUDE_UTILS_H_
