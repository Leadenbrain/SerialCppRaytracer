/**
 * @file z_rotation.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Class for rotation of object around the z axis
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#include "hit.hpp"

/**
 * @brief Object for rotation around the z axis
 *
 * @tparam T
 */
template <typename T>
class z_rotation : public hit<T> {
 public:
  /**
   * @brief Construct an uninitialized z rotation object
   *
   */
  z_rotation();
  /**
   * @brief Construct a new z rotation object
   *
   * @param p Object to rotate
   * @param t_deg Angle of rotation in degrees
   */
  z_rotation(std::shared_ptr<hit<T>> p, const T& t_deg);

  /**
   * @brief Returns whether a ray intersects the rotated objected
   *
   * @param r Ray to compute
   * @param t_min Initial shutter time
   * @param t_max Final shutter time
   * @param rec Hit record of the ray
   * @return true True if the rotated object is hit
   * @return false False if the rotated object is not hit
   */
  bool is_hit(const ray<T>& r,
              const T& t_min,
              const T& t_max,
              hit_rec<T>& out,
              unsigned int*) const override;

  /**
   * @brief Returns whether we are in the bounding box
   *
   * @param out The bounding box of the translated object
   * @return true Returns true point is bounding box
   * @return false Returns false if point is not in bounding box
   */
  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = bound_;
    return box_bool;
  }

 private:
  /**
   * @brief Object to rotate
   *
   */
  std::shared_ptr<hit<T>> p_;
  /**
   * @brief Sine and cosine of the angle
   *
   */
  T sin_t, cos_t;
  /**
   * @brief Boolean for being inside the bounding box
   *
   */
  bool box_bool;
  /**
   * @brief Bounding box of rotated object
   *
   */
  BB<T> bound_;
};

/**
 * @brief Construct a new z rotation<T>::z rotation object
 * @details Apply rotation transformation:
 * x' = cos(t)x - sin(t)y
 * y' = sin(t)x + cos(t)y
 * @tparam T Datatype to be used (e.g float, double)
 * @param p Object to rotate
 * @param t_deg Angle of rotation in degrees
 */
template <typename T>
z_rotation<T>::z_rotation(std::shared_ptr<hit<T>> p, const T& t_deg) : p_(p) {
  T t_rad = deg_to_rad(t_deg);
  sin_t = std::sin(t_rad);
  cos_t = std::cos(t_rad);
  box_bool = p_->bound_box(0, 1, bound_);

  point3<T> min(-inf<T>, -inf<T>, -inf<T>);
  point3<T> max(inf<T>, inf<T>, inf<T>);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        T x = i * bound_.max().getX();
        T y = j * bound_.max().getY();
        T z = k * bound_.max().getZ();

        T x_out = cos_t * x - sin_t * y;
        T y_out = sin_t * x + cos_t * y;

        vec3<T> out(x_out, y_out, z);

        for (int a = 0; a < 3; a++) {
          min[a] = fmin(min[a], out[a]);
          max[a] = fmax(max[a], out[a]);
        }
      }
    }
  }

  bound_ = BB<T>(min, max);
}

/**
 * @brief
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param r Ray to compute
 * @param t_min Initial shutter time
 * @param t_max Final shutter time
 * @param rec Hit record of the ray
 * @return true True if the rotated object is hit
 * @return false False if the rotated object is not hit
 */
template <typename T>
bool z_rotation<T>::is_hit(const ray<T>& r,
                           const T& t_min,
                           const T& t_max,
                           hit_rec<T>& rec,
                           unsigned int*) const {
  point3<T> o = r.origin();
  vec3<T> d = r.direction();

  // Want to say this is right, might have my visual rotated
  o[0] = cos_t * r.origin()[0] - sin_t * r.origin()[1];
  o[1] = sin_t * r.origin()[0] + cos_t * r.origin()[1];

  d[0] = cos_t * r.direction()[0] - sin_t * r.direction()[1];
  d[1] = sin_t * r.direction()[0] + cos_t * r.direction()[1];

  ray<T> rot_r(o, d, r.time());

  if (!p_->is_hit(rot_r, t_min, t_max, rec))
    return false;

  point3<T> p = rec.p;
  vec3<T> n = rec.n;

  p[0] = cos_t * rec.p[0] - sin_t * rec.p[1];
  p[1] = sin_t * rec.p[0] + cos_t * rec.p[1];

  n[0] = cos_t * rec.n[0] - sin_t * rec.n[1];
  n[1] = sin_t * rec.n[0] + cos_t * rec.n[1];

  rec.p = p;
  rec.set_face(rot_r, n);

  return true;
}
