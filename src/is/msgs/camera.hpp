#ifndef __IS_MSG_CAMERA_HPP__
#define __IS_MSG_CAMERA_HPP__

#include "../packer.hpp"

namespace is {
namespace msg {
namespace camera {

struct OggPacket {
  bool new_context;
  std::vector<unsigned char> data;
  int64_t packet_n;
  int64_t granule_pos;
  IS_DEFINE_MSG(new_context, data, packet_n, granule_pos);
};

struct CompressedImage {
  std::string format;               // Image format: ".png", ".jpg"
  std::vector<unsigned char> data;  // Image binary data
  IS_DEFINE_MSG(format, data);
};

struct RegionOfInterest {
  unsigned int x_offset;  // Leftmost pixel of the ROI
  unsigned int y_offset;  // Topmost pixel of the ROI
  unsigned int height;    // Height of ROI
  unsigned int width;     // Width of ROI
  IS_DEFINE_MSG(x_offset, y_offset, height, width);
};

struct Resolution {
  unsigned int height;
  unsigned int width;
  IS_DEFINE_MSG(height, width);
};

struct ImageType {
  std::string value;
  IS_DEFINE_MSG(value);
};

namespace image_type {
const ImageType rgb{"rgb"};
const ImageType gray{"gray"};
}

}  // ::camera
}  // ::msg
}  // ::is

#endif  // __IS_MSG_CAMERA_HPP__