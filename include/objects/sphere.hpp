/**
 * @file sphere.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Sphere object to be rendered
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "hit.hpp"

// Subclass of hit: sphere
/**
 * @brief Sphere object to be rendered
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class sphere : public hit<T> {
 public:
  /**
   * @brief Construct an unitialized sphere object
   *
   */
  sphere() {}
  /**
   * @brief Construct a new sphere object
   *
   * @param cen Center of sphere in x,y,z
   * @param r Radius of sphere
   * @param m Material of sphere
   */
  sphere(const point3<T>& cen, const T& r, std::shared_ptr<material<T>> m)
      : mat(m), c_(cen), r_(r) {}

  /**
   * @brief Return the radius of the sphere
   *
   * @return T Radius of sphere
   */
  T radius() const { return r_; }

  /**
   * @brief Return the center of the sphere
   *
   * @return point3<T> Center of the sphere
   */
  point3<T> center() const { return c_; }

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
   * @param out The bounding box of the object
   * @return true Always returns true
   * @return false Never returns false
   */
  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = BB<T>(c_ - vec3<T>(r_, r_, r_), c_ + vec3<T>(r_, r_, r_));
    return true;
  }

 private:
  /**
   * @brief Material of the sphere
   *
   */
  std::shared_ptr<material<T>> mat;
  /**
   * @brief Center of the sphere
   *
   */
  point3<T> c_;
  /**
   * @brief Radius of the sphere
   *
   */
  T r_;

  /**
   * @brief Get the uv mapping of the sphere from a point
   *
   * @param p Point
   * @param u Mapping of coordinate zero to u
   * @param v Mapping of coordinate one to v
   */
  static void get_sph_uv(const point3<T>& p, T& u, T& v) {
    T t = std::acos(-p.getY());
    T phi = std::atan2(-p.getZ(), p.getX()) * M_PI;

    u = phi / (static_cast<T>(2) * M_PI);
    v = t / M_PI;
  }
};

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
bool sphere<T>::is_hit(const ray<T>& r,
                       const T& t_min,
                       const T& t_max,
                       hit_rec<T>& rec,
                       unsigned int*) const {
  // Ray from origin to center of sphere
  vec3<T> oc = r.origin() - c_;

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
  vec3<T> n_out = (rec.p - c_) / r_;
  rec.set_face(r, n_out);
  rec.mat = mat;
  get_sph_uv(n_out, rec.u, rec.v);
  return true;
}
