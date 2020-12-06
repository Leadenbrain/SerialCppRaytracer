/**
 * @file moving_sphere.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Sphere object moving, for motion blur
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "hit.hpp"

// Subclass of hit, moving_sphere
/**
 * @brief Moving sphere object, to render a sphere with motion blur
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class moving_sphere : public hit<T> {
 public:
  /**
   * @brief Construct an uninitialized moving sphere object
   *
   */
  moving_sphere() {}
  /**
   * @brief Construct a new moving sphere object
   *
   * @param cen0 Center of sphere at initial shutter time
   * @param cen1 Center of sphere at final shutter time
   * @param t0 Initial shutter time
   * @param t1 Final shutter time
   * @param r Radius of sphere
   * @param m Material of sphere
   */
  moving_sphere(const point3<T>& cen0,
                const point3<T>& cen1,
                const T& t0,
                const T& t1,
                const T& r,
                std::shared_ptr<material<T>> m)
      : mat(m), c0_(cen0), c1_(cen1), t0_(t0), t1_(t1), r_(r) {}

  /**
   * @brief Returns the radius of the sphere
   *
   * @return T Radius of the sphere
   */
  T radius() const { return r_; }

  /**
   * @brief Returns the center of the sphere at a given time using linear
   * interpolation
   *
   * @return point3<T> Center of the sphere at time t
   */
  point3<T> center(const T&) const;

  /**
   * @brief Returns whether a ray intersects the sphere
   *
   * @return true True if the sphere is hit
   * @return false False if the sphere is not hit
   */
  bool is_hit(const ray<T>&,
              const T&,
              const T&,
              hit_rec<T>&,
              unsigned int*) const override;

  /**
   * @brief Returns whether we are in the bounding box
   *
   * @param t0 Initial shutter time
   * @param t1 Final shutter time
   * @param out The bounding box of the object
   * @return true Always returns true
   * @return false Never returns false
   */
  bool bound_box(const T& t0, const T& t1, BB<T>& out) const override {
    BB<T> b0(center(t0) - vec3<T>(r_, r_, r_),
             center(t0) + vec3<T>(r_, r_, r_));
    BB<T> b1(center(t1) - vec3<T>(r_, r_, r_),
             center(t1) + vec3<T>(r_, r_, r_));
    out = surround_box(b0, b1);
    return true;
  }

 private:
  /**
   * @brief Material of the sphere
   *
   */
  std::shared_ptr<material<T>> mat;
  /**
   * @brief Center of the sphere at initial and final shutter time
   *
   */
  point3<T> c0_, c1_;
  /**
   * @brief Initial and final shutter time
   *
   */
  T t0_, t1_;
  /**
   * @brief Radius of the sphere
   *
   */
  T r_;
};

/**
 * @brief Returns the center of the sphere at a given time using linear
 * interpolation
 *
 * @tparam T Datatype to be used (e.g float, double)
 * @param t Time to compute center
 * @return point3<T> Center of the sphere at a given time
 */
template <typename T>
point3<T> moving_sphere<T>::center(const T& t) const {
  return c0_ + ((t - t0_) / (t1_ - t0_)) * (c1_ - c0_);
}

/**
 * @brief Returns whether a ray intersects the sphere
 *
 * @tparam T Datatype to be used (e.g float, double)
 * @param r Ray to compute
 * @param t_min Initial shutter time
 * @param t_max Final shutter time
 * @param rec Hit record of the ray
 * @return true True if the sphere is hit
 * @return false False if the sphere is not hit
 */
template <typename T>
bool moving_sphere<T>::is_hit(const ray<T>& r,
                              const T& t_min,
                              const T& t_max,
                              hit_rec<T>& rec,
                              unsigned int*) const {
  // Ray from origin to center of moving_sphere
  vec3<T> oc = r.origin() - center(r.time());

  // We'll solve (P-C)\dot(P-C) = r^2
  // Clever trick here: we only need to do half
  // -> the two sides of the square will give same answer anyways
  T a = r.direction().norm_sqr();
  T b = dot(oc, r.direction());
  T c = oc.norm_sqr() - r_ * r_;

  T discriminant = b * b - a * c;
  if (discriminant < 0)
    return false;
  T sqrtd(std::sqrt(discriminant));

  // Find best soln in our time
  T soln = (-b - sqrtd) / a;
  if (soln < t_min || t_max < soln) {
    soln = (-b + sqrtd) / a;
    if (soln < t_min || t_max < soln)
      return false;
  }

  rec.t = soln;
  rec.p = r.at(rec.t);
  vec3<T> n_out = (rec.p - center(r.time())) / r_;
  rec.set_face(r, n_out);
  rec.mat = mat;
  return true;
}
