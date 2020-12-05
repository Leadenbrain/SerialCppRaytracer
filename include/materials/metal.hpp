/**
 * @file metal.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Reflective surface, with additional fuzzy scattering
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "material.hpp"

// Make a public subclass of material
/**
 * @brief Reflective metal subclass of material
 *
 * @tparam T Datatype to be used (double, float)
 */
template <typename T>
class metal : public material<T> {
 public:
  /**
   * @brief Construct a new metal object from a color, with diffuse fuzziness
   *
   * @param c Color of metal
   * @param f Diffuse percentage (0-1)
   */
  metal(const color<T>& c, const T f) : metal_col(c), fuzz_(f < 1 ? f : 1) {}
  // Override our base function
  /**
   * @brief Overidden scatter function.
   * @details Determine reflection of normal around symmetric axis. Appplies
   * fuzzy diffuse scattering
   *
   * @param r Ray to compute
   * @param rec Hit record of ray
   * @param att Attenuation
   * @param scat Scattering color
   * @return true This object always scatters (always occurs)
   * @return false This object always scatters (never occurs)
   */
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
  // TODO: This should be a texture
  /**
   * @brief Color of metal
   *
   */
  color<T> metal_col;
  /**
   * @brief Diffuse percentage to fuzz scattering (0-1)
   *
   */
  T fuzz_;
};
