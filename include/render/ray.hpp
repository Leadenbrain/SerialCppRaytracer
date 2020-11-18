// Our ray object, this allows us to render
// They get sent from our camera into the world and will interact with objects
// As they move through our shutter time, they will pick up color data we
// collect at the end
#ifndef _INCLUDED_RAY_HPP
#define _INCLUDED_RAY_HPP

#include "vec3.hpp"

template <typename T>
class ray {
 public:
  ray() {}
  ray(const point3<T>& o, const vec3<T>& dir) : o_(o), dir_(dir), time_(0) {}
  ray(const point3<T>& o, const vec3<T>& dir, T time)
      : o_(o), dir_(dir), time_(time) {}

  point3<T> origin() const { return o_; }
  vec3<T> direction() const { return dir_; }
  T time() const { return time_; }

  point3<T> at(T t) const { return o_ + t * dir_; }

 private:
  point3<T> o_;
  vec3<T> dir_;
  T time_;
};

#endif