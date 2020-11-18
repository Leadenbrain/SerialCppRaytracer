// Base class for textures
#ifndef INCLUDED_TEXTURE_HPP
#define INCLUDED_TEXTURE_HPP

#include "utilities.hpp"
#include "vec3.hpp"

template <typename T>
// g++ gave me errors with texture as class name
class uvTex {
 public:
  virtual color<T> val(const T& u, const T& v, const point3<T>& p) const = 0;
};

#endif
