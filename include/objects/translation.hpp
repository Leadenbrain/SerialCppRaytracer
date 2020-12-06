/**
 * @file translation.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Class for translation of a hittable object
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "hit.hpp"

/**
 * @brief Object for translation of a hittable object
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class translate : public hit<T> {
 public:
  /**
   * @brief Construct an unitialized translate object
   *
   */
  translate() {}
  /**
   * @brief Construct a new translate object
   *
   * @param p Object to translate
   * @param disp Translation vector
   */
  translate(std::shared_ptr<hit<T>> p, const vec3<T>& disp)
      : p_(p), disp_(disp) {}

  /**
   * @brief Returns whether a ray intersects the translated objected
   *
   * @param r Ray to compute
   * @param t_min Initial shutter time
   * @param t_max Final shutter time
   * @param rec Hit record of the ray
   * @return true True if the translated object is hit
   * @return false False if the translated object is not hit
   */
  bool is_hit(const ray<T>& r,
              const T& t_min,
              const T& t_max,
              hit_rec<T>& rec,
              unsigned int*) const override {
    ray<T> trans_r(r.origin() - disp_, r.direction(), r.time());
    if (!p_->is_hit(trans_r, t_min, t_max, rec))
      return false;

    rec.p += disp_;
    rec.set_face(trans_r, rec.n);

    return true;
  }

  /**
   * @brief Returns whether we are in the bounding box
   *
   * @param out The bounding box of the translated object
   * @return true Returns true point is bounding box
   * @return false Returns false if point is not in bounding box
   */
  bool bound_box(const T& t0, const T& t1, BB<T>& out) const override {
    if (!p_->bound_box(t0, t1, out))
      return false;

    out = BB<T>(out.min() + disp_, out.max() + disp_);
    return true;
  }

 private:
  /**
   * @brief Object to translate
   *
   */
  std::shared_ptr<hit<T>> p_;
  /**
   * @brief Translation vector
   *
   */
  vec3<T> disp_;
};
