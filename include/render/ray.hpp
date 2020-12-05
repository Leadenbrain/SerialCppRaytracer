/**
 * @file ray.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief The ray object for our tracing algorithm.
 * @details This class is the object ejected from pixels. It merely acts as a
 * mathematical half-ray: tracking the origin and direction of the ray.
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#include "vec3.hpp"

/**
 * @brief A template class for a ray object
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class ray {
 public:
  /**
   * @brief Construct an uninitialized ray object
   *
   */
  ray() {}
  /**
   * @brief Construct a new ray object, defaulting time to 0
   *
   * @param o Point of the ray origin
   * @param dir Direction vector of the ray
   */
  ray(const point3<T>& o, const vec3<T>& dir) : o_(o), dir_(dir), time_(0) {}
  /**
   * @brief Construct a new ray object
   *
   * @param o Point of the ray origin
   * @param dir Direction vector of the ray
   * @param time Time value to compute ray at
   */
  ray(const point3<T>& o, const vec3<T>& dir, T time)
      : o_(o), dir_(dir), time_(time) {}

  /**
   * @brief Return the origin point
   *
   * @return point3<T> Origin of ray
   */
  point3<T> origin() const { return o_; }
  /**
   * @brief Return the direction vector of the ray
   *
   * @return vec3<T> Direction vector of ray
   */
  vec3<T> direction() const { return dir_; }
  /**
   * @brief Return the time the ray is at
   *
   * @return T Time value of ray
   */
  T time() const { return time_; }

  /**
   * @brief Determine end of ray
   *
   * @param t Time to trace ray to
   * @return point3<T> Ray position after time t
   */
  point3<T> at(T t) const { return o_ + t * dir_; }

 private:
  /**
   * @brief Origin of ray
   *
   */
  point3<T> o_;
  /**
   * @brief Direction vector of ray
   *
   */
  vec3<T> dir_;
  /**
   * @brief Time value of ray
   *
   */
  T time_;
};
