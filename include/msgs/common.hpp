#ifndef __IS_MSG_COMMON_HPP__
#define __IS_MSG_COMMON_HPP__

#include <chrono>
#include <string>
#include <vector>
#include <is/packer.hpp>

namespace is {
namespace msg {
namespace common {

enum class Status { OK, FAILED };

struct Delay {
  int milliseconds;

  MSGPACK_DEFINE_ARRAY(milliseconds);
};

struct TimeStamp {
  int64_t time_point;

  TimeStamp() { time_point = std::chrono::system_clock::now().time_since_epoch().count(); }

  MSGPACK_DEFINE_ARRAY(time_point);
};

struct SamplingRate {
  double rate;     // [Hz]
  int64_t period;  // [ms]

  SamplingRate() {}

  SamplingRate(double r) {
    rate = r;
    period = 0;
  }

  SamplingRate(int64_t p) {
    period = p;
    rate = 0.0;
  }

  MSGPACK_DEFINE_ARRAY(rate, period);
};

struct EntityList {
  std::vector<std::string> list;

  EntityList() {}

  EntityList(std::vector<std::string> l) {
    list = l;
  }

  MSGPACK_DEFINE_ARRAY(list);
};

struct Point2d {
  double x, y;

  MSGPACK_DEFINE_ARRAY(x, y);
};

struct Pattern {
  std::vector<Point2d> points;
  std::string frame;

  MSGPACK_DEFINE_ARRAY(points, frame);
};

struct FrameChangeRequest {
  std::vector<Pattern> patterns;
  double z;
  
  MSGPACK_DEFINE_ARRAY(patterns, z);
};

struct Point3d {
  double x, y, z;

  MSGPACK_DEFINE_ARRAY(x, y, z);
};

}  // ::common
}  // ::msg
}  // ::is

// Enum packing must be done on global namespace
MSGPACK_ADD_ENUM(is::msg::common::Status);

#endif  // __IS_MSG_COMMON_HPP__
