// Copyright 2014-4 sxniu
#ifndef QIMAGEPROCESSING_INCLUDE_UTILS_H_
#define QIMAGEPROCESSING_INCLUDE_UTILS_H_

namespace utils {

bool SetArroundPixelsPosition(int* arround_pixels_pos, int center_pos_x,
                              int center_pos_y, int number_of_pixels,
                              int width, int height);

}  // namespace utils

#endif  // QIMAGEPROCESSING_INCLUDE_UTILS_H_
