// Simple texture subclass for a solid color
#ifndef INCLUDE_TEXTURES_SOLID_HPP_
#define INCLUDE_TEXTURES_SOLID_HPP_

#include "texture.hpp"
#include "vec3.hpp"

template <typename T>
class solid : public uvTex<T> {
 public:
  solid() {}
  explicit solid(const color<T>& c) : col_(c) {}

  solid(const T& r, const T& g, const T& b) : solid(color<T>(r, g, b)) {}
  virtual color<T> val(const T&, const T&, const vec3<T>&) const override {
    return col_;
  }

 private:
  color<T> col_;
};

#endif  // INCLUDE_TEXTURES_SOLID_HPP_
