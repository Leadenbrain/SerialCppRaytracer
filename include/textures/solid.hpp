/**
 * @file solid.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Solid one color texture
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "texture.hpp"

/**
 * @brief Solid one color subclass of uvTex
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class solid : public uvTex<T> {
 public:
  /**
   * @brief Construct an uninitialized solid object
   *
   */
  solid() {}
  /**
   * @brief Construct a new solid object from a color
   *
   * @param c
   */
  explicit solid(const color<T>& c) : col_(c) {}
  /**
   * @brief Construct a new solid object from r,g,b values
   *
   * @param r Red value (b/w 0-255)
   * @param g Green value (b/w 0-255)
   * @param b Blue value (b/w 0-255)
   */
  solid(const T& r, const T& g, const T& b) : solid(color<T>(r, g, b)) {}
  /**
   * @brief Return the color of the texture
   *
   * @return color<T> Color of texture
   */
  virtual color<T> val(const T&, const T&, const vec3<T>&) const override {
    return col_;
  }

 private:
  /**
   * @brief Color of texture
   *
   */
  color<T> col_;
};
