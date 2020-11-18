// A metal subclass of material
// This object will reflect light (like a mirror)
#ifndef _INCLUDED_METAL_HPP
#define _INCLUDED_METAL_HPP

#include "material.hpp"

// Make a public subclass of material
template <typename T>
class metal : public material<T> {
 public:
  // Assign the color on initializing, if fuzz value sent in < 1, use it
  metal(const color<T>& a, const T f) : metal_col(a), fuzz_(f < 1 ? f : 1) {}
  // Override our base function
  bool scatter(const ray<T>& r,
               const hit_rec<T>& rec,
               color<T>& att,
               ray<T>& scat) const override {
    // Reflect the ray around normal
    vec3<T> ref = reflect<T>(unit_v<T>(r.direction()), rec.n);
    // Add a fuzz-factor to our metal
    scat = ray<T>(rec.p, ref + fuzz_ * random_sphere<T>()), r.time();
    att = metal_col;
    return true;
  }

 private:
  color<T> metal_col;
  T fuzz_;
};

#endif
