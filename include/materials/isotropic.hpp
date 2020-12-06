/**
 * @file isotropic.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Isotropic fog material
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#include "material.hpp"
#include "textures/solid.hpp"

/**
 * @brief Fog object for isotropic scattering; subclass of material
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class isotropic : public material<T> {
 public:
  /**
   * @brief Construct a new isotropic object from a texture
   *
   */
  explicit isotropic(std::shared_ptr<uvTex<T>>(c)) : c_(c) {}
  /**
   * @brief Construct a new isotropic object from a color
   *
   * @param c Color of object
   */
  explicit isotropic(const color<T>& c) : c_(std::make_shared<solid<T>>(c)) {}

  /**
   * @brief Overidden scatter function.
   * @details Scatters randomly in a sphere and attenuates by fog color
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
    scat = ray<T>(rec.p, random_sphere<T>(seed), r.time());
    att = c_->val(rec.u, rec.v, rec.p);
    return true;
  }

 private:
  /**
   * @brief Texture of fog
   *
   */
  std::shared_ptr<uvTex<T>> c_;
};
