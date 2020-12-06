/**
 * @file hit.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Abstract object that can be hit by a ray
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "bounding_box.hpp"

// Forward decl
template <typename T>
class material;

/**
 * @brief A structure to store the record of the ray
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
struct hit_rec {
  point3<T> p;
  vec3<T> n;
  std::shared_ptr<material<T>> mat;
  T t, u, v;
  bool front;

  // Determine whether our ray is aligned with the normal
  // ->This tells us whether we are inside or outside our object
  /**
   * @brief Determine whether our ray is aligned with the normal
   *
   * @param r Ray to compute
   * @param n_out Corrected normal whether inside or out of object
   */
  void set_face(const ray<T>& r, const vec3<T>& n_out) {
    front = dot(r.direction(), n_out) < 0;
    n = front ? n_out : -n_out;
  }
};

// Our base class for objects
/**
 * @brief Base class for hittable objects
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class hit {
 public:
  //  Return whether an object is hit or not
  /**
   * @brief Returns whether a ray intersects an object
   *
   * @return true True if the ray hits the object
   * @return false False if the ray does not hit the object
   */
  virtual bool is_hit(const ray<T>&,
                      const T&,
                      const T&,
                      hit_rec<T>&,
                      unsigned int*) const = 0;
  /**
   * @brief Returns if we are in an object's bounding box
   *
   * @param out Bounding box of object
   * @return true True if we are in the object's bounding box
   * @return false False if we are outside of the object's bounding box
   */
  virtual bool bound_box(const T&, const T&, BB<T>& out) const = 0;
};
