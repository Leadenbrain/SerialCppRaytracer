// A glass object that will refract using snell's law
#ifndef _INCLUDED_GLASS_HPP
#define _INCLUDED_GLASS_HPP

#include "material.hpp"

// Make a public subclass of material
template <typename T>
class glass : public material<T> {
 public:
  // Assign the index of refraction on initialization
  explicit glass(const T index) : eta_(index) {}
  // Override our scatter func
  bool scatter(const ray<T>& r,
               const hit_rec<T>& rec,
               color<T>& att,
               ray<T>& scat) const override {
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
    if (refl || (schlick(cos_t, refr_rat) > random_double()))
      direction = reflect<T>(unit_d, rec.n);
    else
      direction = refract<T>(unit_d, rec.n, refr_rat);

    scat = ray<T>(rec.p, direction, r.time());
    att = color<T>(1.0, 1.0, 1.0);
    return true;
  }

 private:
  T eta_;

  static T schlick(T cos, T eta) {
    T eta0 = (1 - eta) / (1 + eta);
    eta0 *= eta0;
    return eta0 + (1 - eta0) * std::pow((1 - cos), 5);
  }
};

#endif
