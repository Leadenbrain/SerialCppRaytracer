// Base class for our materials
// Need abstraction for behaviour of rays on a mat

#ifndef INCLUDE_MATERIALS_MATERIAL_HPP_
#define INCLUDE_MATERIALS_MATERIAL_HPP_

#include "render/ray.hpp"
#include "utilities.hpp"
#include "vec3.hpp"

// Forward decl
template <typename T>
struct hit_rec;

// Material base class, only forward decl scatter function
template <typename T>
class material {
 public:
  // Function for the scattering behaviouur of light
  virtual bool scatter(const ray<T>&,
                       const hit_rec<T>&,
                       color<T>&,
                       ray<T>&) const = 0;

  // Function for emission behaviour of light
  virtual color<T> emit(const T&, const T&, const point3<T>&) const {
    return color<T>(0, 0, 0);
  }
};

#endif  // INCLUDE_MATERIALS_MATERIAL_HPP_
