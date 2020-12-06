/**
 * @file rectangle.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Rectangular plane object to be rendered
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

// TODO: Make rectangle base class and sub-class for planes?
#pragma once

#include "hit.hpp"

// Rectangle on the xy plane
/**
 * @brief Rectangular plane with normal along z
 *
 * @tparam T Datatype to be used
 */
template <typename T>
class xy_rectangle : public hit<T> {
 public:
  /**
   * @brief Construct an uninitialized xy rectangle object
   *
   */
  xy_rectangle() {}
  /**
   * @brief Construct a new xy rectangle object
   *
   * @param x0 Left boundary
   * @param x1 Right boundary
   * @param y0 Bottom boundary
   * @param y1 Top boundary
   * @param k Depth along z
   * @param m Material of object
   */
  xy_rectangle(const T& x0,
               const T& x1,
               const T& y0,
               const T& y1,
               const T& k,
               std::shared_ptr<material<T>> m)
      : mat(m), x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k) {}

  /**
   * @brief Returns whether a ray intersects the plane
   *
   * @return true True if the plane is hit
   * @return false False if the plane is not hit
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
    out = BB<T>(point3<T>(x0_, y0_, k_ - 0.0001),
                point3<T>(x1_, y1_, k_ + 0.0001));
    return true;
  }

 private:
  /**
   * @brief Material of the object
   *
   */
  std::shared_ptr<material<T>> mat;
  /**
   * @brief X boundaries of plane
   *
   */
  T x0_, x1_;
  /**
   * @brief Y boundaries of plane
   *
   */
  T y0_, y1_;
  /**
   * @brief Depth along z axis of plane
   *
   */
  T k_;
};

/**
 * @brief Returns whether a ray intersects the plane
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
bool xy_rectangle<T>::is_hit(const ray<T>& r,
                             const T& t_min,
                             const T& t_max,
                             hit_rec<T>& rec,
                             unsigned int*) const {
  T t = (k_ - r.origin().getZ()) / r.direction().getZ();
  if (t < t_min || t > t_max)
    return false;
  T x = r.origin().getX() + t * r.direction().getX();
  T y = r.origin().getY() + t * r.direction().getY();

  if (x < x0_ || x > x1_ || y < y0_ || y > y1_)
    return false;

  rec.u = (x - x0_) / (x1_ - x0_);
  rec.v = (y - y0_) / (y1_ - y0_);
  rec.t = t;
  vec3<T> n_out = vec3<T>(0, 0, 1);
  rec.set_face(r, n_out);
  rec.mat = mat;
  rec.p = r.at(t);

  return true;
}

/**
 * @brief Rectangular plane with normal along y
 *
 * @tparam T Datatype to be used
 */
template <typename T>
class xz_rectangle : public hit<T> {
 public:
  /**
   * @brief Construct an uninitialized xz rectangle object
   *
   */
  xz_rectangle() {}
  /**
   * @brief Construct a new xy rectangle object
   *
   * @param x0 Left boundary
   * @param x1 Right boundary
   * @param z0 Bottom boundary
   * @param z1 Top boundary
   * @param k Depth along z
   * @param m Material of object
   */
  xz_rectangle(const T& x0,
               const T& x1,
               const T& z0,
               const T& z1,
               const T& k,
               std::shared_ptr<material<T>> m)
      : mat(m), x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k){};

  /**
   * @brief Returns whether a ray intersects the plane
   *
   * @return true True if the plane is hit
   * @return false False if the plane is not hit
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
    out = BB<T>(point3<T>(x0_, k_ - 0.0001, z0_),
                point3<T>(x1_, k_ + 0.0001, z1_));
    return true;
  }

 private:
  /**
   * @brief Material of the object
   *
   */
  std::shared_ptr<material<T>> mat;
  /**
   * @brief X boundaries of plane
   *
   */
  T x0_, x1_;
  /**
   * @brief Z boundaries of plane
   *
   */
  T z0_, z1_;
  /**
   * @brief Depth along y axis of plane
   *
   */
  T k_;
};

/**
 * @brief Returns whether a ray intersects the plane
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
bool xz_rectangle<T>::is_hit(const ray<T>& r,
                             const T& t_min,
                             const T& t_max,
                             hit_rec<T>& rec,
                             unsigned int*) const {
  T t = (k_ - r.origin().getY()) / r.direction().getY();
  if (t < t_min || t > t_max)
    return false;
  T x = r.origin().getX() + t * r.direction().getX();
  T z = r.origin().getZ() + t * r.direction().getZ();

  if (x < x0_ || x > x1_ || z < z0_ || z > z1_)
    return false;

  rec.u = (x - x0_) / (x1_ - x0_);
  rec.v = (z - z0_) / (z1_ - z0_);
  rec.t = t;
  vec3<T> n_out = vec3<T>(0, 1, 0);
  rec.set_face(r, n_out);
  rec.mat = mat;
  rec.p = r.at(t);

  return true;
}

/**
 * @brief Rectangular plane with normal along x
 *
 * @tparam T Datatype to be used
 */
template <typename T>
class yz_rectangle : public hit<T> {
 public:
  /**
   * @brief Construct an uninitialized yz rectangle object
   *
   */
  yz_rectangle() {}
  /**
   * @brief Construct a new xy rectangle object
   *
   * @param y0 Left boundary
   * @param y1 Right boundary
   * @param z0 Bottom boundary
   * @param z1 Top boundary
   * @param k Depth along z
   * @param m Material of object
   */
  yz_rectangle(const T& y0,
               const T& y1,
               const T& z0,
               const T& z1,
               const T& k,
               std::shared_ptr<material<T>> m)
      : mat(m), y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k){};

  /**
   * @brief Returns whether a ray intersects the plane
   *
   * @return true True if the plane is hit
   * @return false False if the plane is not hit
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
    out = BB<T>(point3<T>(k_ - 0.0001, y0_, z0_),
                point3<T>(k_ + 0.0001, y1_, z1_));
    return true;
  }

 private:
  /**
   * @brief Material of the object
   *
   */
  std::shared_ptr<material<T>> mat;
  /**
   * @brief Y boundaries of plane
   *
   */
  T y0_, y1_;
  /**
   * @brief Z boundaries of plane
   *
   */
  T z0_, z1_;
  /**
   * @brief Depth along X axis of plane
   *
   */
  T k_;
};

/**
 * @brief Returns whether a ray intersects the plane
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
bool yz_rectangle<T>::is_hit(const ray<T>& r,
                             const T& t_min,
                             const T& t_max,
                             hit_rec<T>& rec,
                             unsigned int*) const {
  T t = (k_ - r.origin().getX()) / r.direction().getX();
  if (t < t_min || t > t_max)
    return false;
  T y = r.origin().getY() + t * r.direction().getY();
  T z = r.origin().getZ() + t * r.direction().getZ();

  if (y < y0_ || y > y1_ || z < z0_ || z > z1_)
    return false;

  rec.u = (y - y0_) / (y1_ - y0_);
  rec.v = (z - z0_) / (z1_ - z0_);
  rec.t = t;
  vec3<T> n_out = vec3<T>(1, 0, 0);
  rec.set_face(r, n_out);
  rec.mat = mat;
  rec.p = r.at(t);

  return true;
}
