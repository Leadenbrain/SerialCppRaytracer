/**
 * @file diffuse.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Object for diffuse (random) scattering. Subclass of material
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "material.hpp"
#include "textures/solid.hpp"

// Make a public subclass of material
/**
 * @brief Object for diffuse reflection of light; subclass of material
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class diffuse : public material<T> {
 public:
  /**
   * @brief Construct a new diffuse object using a color
   *
   * @param c Color of object
   */
  explicit diffuse(const color<T>& c)
      : diff_col(std::make_shared<solid<T>>(c)) {}
  /**
   * @brief Construct a new diffuse object using a texture
   *
   * @param c Texture of object
   */
  explicit diffuse(std::shared_ptr<uvTex<T>> c) : diff_col(c) {}

  // Override our scatter func
  /**
   * @brief Overidden scatter function.
   * @details Scatters the object into a random direction
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
    // We want to randomly scatter
    vec3<T> scat_dir = rec.n + random_unit_v<T>(seed);

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
