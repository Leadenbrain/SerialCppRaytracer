/**
 * @file material.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Base class for materials
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#include "render/ray.hpp"
// Forward decl
template <typename T>
struct hit_rec;

// Material base class, only forward decl scatter function
/**
 * @brief Material base class.
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class material {
 public:
  /**
   * @brief Boolean for whether our object scatters or absorbs
   *
   * @return true If true, the object scatters
   * @return false If false, the object absorbs
   */
  virtual bool scatter(const ray<T>&,
                       const hit_rec<T>&,
                       color<T>&,
                       ray<T>&,
                       unsigned int*) const = 0;

  // Function for emission behaviour of light
  /**
   * @brief Return the emissive color of the material
   *
   * @return color<T> Emissive color of material, default to none.
   */
  virtual color<T> emit(const T&, const T&, const point3<T>&) const {
    return color<T>(0, 0, 0);
  }
};
