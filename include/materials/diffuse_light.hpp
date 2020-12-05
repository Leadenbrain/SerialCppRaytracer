/**
 * @file diffuse_light.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Emissive diffuse light material
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
 * @brief Emissive diffuse light subclass of material
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class diffuse_light : public material<T> {
 public:
  /**
   * @brief Construct an uninitialized diffuse light object
   *
   */
  diffuse_light() {}
  /**
   * @brief Construct a new diffuse light object from a color
   *
   * @param c Color of light
   */
  explicit diffuse_light(const color<T>& c)
      : c_(std::make_shared<solid<T>>(c)) {}
  /**
   * @brief Construct a new diffuse light object from a texture
   *
   * @param c Texture of light
   */
  explicit diffuse_light(std::shared_ptr<uvTex<T>> c) : c_(c) {}

  // We won't be scattering this time
  /**
   * @brief Scattering function of diffuse light -> Object doesn't scatter
   *
   * @return true This will never happen
   * @return false This will always happen
   */
  bool scatter(const ray<T>&,
               const hit_rec<T>&,
               color<T>&,
               ray<T>&) const override {
    return false;
  }

  /**
   * @brief Return the emissive value of the light
   *
   * @param u u value of texture map
   * @param v v value of texture map
   * @param p Point to determine color of
   * @return color<T> Color of texture mapped point
   */
  color<T> emit(const T& u, const T& v, const point3<T>& p) const override {
    return c_->val(u, v, p);
  }

  //  private:
  /**
   * @brief Texture of diffuse light
   *
   */
  std::shared_ptr<uvTex<T>> c_;
};
