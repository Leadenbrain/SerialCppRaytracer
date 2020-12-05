/**
 * @file texture.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Bass class for textures
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

// Base class for textures
#pragma once

#include "vec3.hpp"

/**
 * @brief Base class for texture objects
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
// g++ gave me errors with texture as class name
class uvTex {
 public:
  /**
   * @brief Return the color value of a texture map (u,v) at a point (x,y,z)
   *
   * @param u u value of texture map
   * @param v v value of texture map
   * @param p Point to get color for
   * @return color<T> Color of mapped point
   */
  virtual color<T> val(const T& u, const T& v, const point3<T>& p) const = 0;
};
