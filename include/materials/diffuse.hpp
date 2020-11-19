// Subclass of material for diffuse objects
// This corresponds to objects which randomly scatter light on impact
// This is likely the majority of objects
#ifndef INCLUDE_MATERIALS_DIFFUSE_HPP_
#define INCLUDE_MATERIALS_DIFFUSE_HPP_

#include "material.hpp"
#include "textures/solid.hpp"

// Make a public subclass of material
template <typename T>
class diffuse : public material<T> {
 public:
  // Assign the color on initializing
  explicit diffuse(const color<T> c)
      : diff_col(std::make_shared<solid<T>>(c)) {}
  explicit diffuse(std::shared_ptr<uvTex<T>> c) : diff_col(c) {}

  // Override our scatter func
  bool scatter(const ray<T>& r,
               const hit_rec<T>& rec,
               color<T>& att,
               ray<T>& scat) const override {
    // We want to randomly scatter
    vec3<T> scat_dir = rec.n + random_unit_v<T>();

    // Get redundant scatters
    if (scat_dir.near_null())
      scat_dir = rec.n;

    scat = ray<T>(rec.p, scat_dir, r.time());
    // Attenuate by the colour of object
    // att = diff_col;
    att = diff_col->val(rec.u, rec.v, rec.p);
    return true;
  }

 private:
  std::shared_ptr<uvTex<T>> diff_col;
};

#endif  // INCLUDE_MATERIALS_DIFFUSE_HPP_
