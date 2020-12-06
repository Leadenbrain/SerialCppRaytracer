/**
 * @file glass.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief A glass object that refracts/reflects using Snell's Law
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "material.hpp"

/**
 * @brief Glass subclass of material
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class glass : public material<T> {
 public:
  /**
   * @brief Construct a new glass object
   *
   * @param index Index of refraction
   */
  explicit glass(const T& index) : eta_(index) {}
  // Override our scatter func
  /**
   * @brief Overidden scatter function.
   * @details Determines whether there is total internal reflection or
   * refraction using Schlick approximation.
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
               ray<T>& scat,
               unsigned int* seed) const override {
    // Get the effective eta
    T refr_rat = rec.front ? (1.0 / eta_) : eta_;

    vec3<T> unit_d = unit_v(r.direction());

    // From Snell's Law we have a total internal reflection
    // Since sin_t < 1, if eta_eff*sin_t > 1.0, we are reflecting
    T cos_t = std::fmin(dot<T>(-unit_d, rec.n), 1.0);
    T sin_t = std::sqrt(1.0 - cos_t * cos_t);

    bool refl = refr_rat * sin_t > 1.0;
    vec3<T> direction;

    // If internal reflect, else refract
    if (refl || (schlick(cos_t, refr_rat) > random_double(seed)))
      direction = reflect<T>(unit_d, rec.n);
    else
      direction = refract<T>(unit_d, rec.n, refr_rat);

    scat = ray<T>(rec.p, direction, r.time());
    att = color<T>(1.0, 1.0, 1.0);
    return true;
  }

 private:
  /**
   * @brief Index of refraction
   *
   */
  T eta_;

  /**
   * @brief Schlick approximation for total internal reflection
   *
   * @param cos Cos of theta
   * @param eta Index of refraction
   * @return T Approximated refractive index
   */
  static T schlick(const T& cos, const T& eta) {
    T eta0 = (1 - eta) / (1 + eta);
    eta0 *= eta0;
    return eta0 + (1 - eta0) * std::pow((1 - cos), 5);
  }
};
