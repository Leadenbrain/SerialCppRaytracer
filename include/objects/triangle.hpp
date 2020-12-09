#pragma once

#include <algorithm>
#include "hit.hpp"

// Rectangle on the xy plane
/**
 * @brief Rectangular plane with normal along z
 *
 * @tparam T Datatype to be used
 */
template <typename T>
class triangle : public hit<T> {
 public:
  /**
   * @brief Construct an uninitialized triangle object
   *
   */
  triangle() {}
  /**
   * @brief Construct a new triangle object
   *
   * @param v0 Left boundary
   * @param v1 Right boundary
   * @param v2 Top boundary
   * @param k Depth along z
   * @param m Material of object
   */
  triangle(const point3<T>& v0,
           const point3<T>& v1,
           const point3<T>& v2,
           const T& k,
           std::shared_ptr<material<T>> m)
      : mat(m), v0_(v0), v1_(v1), v2_(v2), k_(k) {}

  /**
   * @brief Returns whether a ray intersects the triangle
   *
   * @return true True if the plane is hit
   * @return false False if the plane is not hit
   */
  bool is_hit(const ray<T>&, const T&, const T&, hit_rec<T>&) const override;

  /**
   * @brief Returns whether we are in the bounding box
   *
   * @param out The bounding box of the object
   * @return true Always returns true
   * @return false Never returns false
   */
  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = BB<T>(point3<T>(std::min<T>({v0_.getX(), v1_.getX(), v2_.getX()}),
                          std::min<T>({v0_.getY(), v1_.getY(), v2_.getY()}),
                          std::min<T>({v0_.getZ(), v1_.getZ(), v2_.getZ()})),
                point3<T>(std::max<T>({v0_.getX(), v1_.getX(), v2_.getX()}),
                          std::max<T>({v0_.getY(), v1_.getY(), v2_.getY()}),
                          std::max<T>({v0_.getZ(), v1_.getZ(), v2_.getZ()})));
    return true;
  }

 private:
  /**
   * @brief Material of the object
   *
   */
  std::shared_ptr<material<T>> mat;
  /**
   * @brief Vertices of triangle
   *
   */
  point3<T> v0_, v1_, v2_;
  T k_;
};

/**
 * @brief Returns whether a ray intersects the triangle:
 * https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
 *
 * @tparam T Datatype to be used (e.g float, double)
 * @param r Ray to compute
 * @param t_min Initial shutter time
 * @param t_max Final shutter time
 * @param rec Hit record of the ray
 * @return true True if the plane is hit
 * @return false False if the plane is not hit
 */
template <typename T>
bool triangle<T>::is_hit(const ray<T>& r,
                         const T& t_min,
                         const T& t_max,
                         hit_rec<T>& rec) const {
  T u, v, t;

  vec3<T> e1, e2, h, s, q;
  T a, f;

  e1 = v1_ - v0_;
  e2 = v2_ - v0_;
  h = cross(r.direction(), e2);
  a = dot(e1, h);

  if (fabs(a) < 0.000001)
    return false;
  f = 1.0 / a;
  s = r.origin() - v0_;
  u = f * dot(s, h);
  if (u < 0.0 || u > 1.0)
    return false;

  q = cross(s, e1);
  v = f * dot(r.direction(), q);
  if (v < 0.0 || u + v > 1.0)
    return false;

  t = f * dot(e2, q);
  if (t < t_min || t > t_max)
    return false;
  rec.u = u;
  rec.v = v;
  rec.t = t;
  vec3<T> n_out = unit_v(cross(e1, e2));
  rec.set_face(r, n_out);
  rec.mat = mat;
  rec.p = r.at(t);

  return true;
}