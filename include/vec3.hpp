/**
 * @file vec3.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Custom 3D vector class for points, colours and rays
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#include <iostream>

#include "utilities.hpp"

/**
 * @brief 3D Vector class for points, colors and rays
 *
 * @tparam T Datatype to use (e.g float, double)
 */
template <typename T>
class vec3 {
 public:
  /**
   * @brief Construct an uninitialized vec3 object at 0,0,0
   *
   */
  vec3() : e{0, 0, 0} {}
  /**
   * @brief Construct a new vec3 object with identical entries
   *
   * @param e Value to be put in all vector components
   */
  explicit vec3(T e) : e{e, e, e} {}
  /**
   * @brief Construct a new vec3 object using three values
   *
   * @param e0 First component value
   * @param e1 Second component value
   * @param e2 Third component value
   */
  vec3(T e0, T e1, T e2) : e{e0, e1, e2} {}

  /**
   * @brief Returns the X component
   *
   * @return T X component
   */
  T getX() const { return e[0]; }
  /**
   * @brief Returns the y component
   *
   * @return T Y component
   */
  T getY() const { return e[1]; }
  /**
   * @brief Returns the Z component
   *
   * @return T Z component
   */
  T getZ() const { return e[2]; }

  /**
   * @brief Inversion operator
   *
   * @return vec3 Inverted vector
   */
  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  /**
   * @brief Returns value at i
   *
   * @param i Index
   * @return T Value at component i
   */
  T operator[](int i) const { return e[i]; }
  /**
   * @brief Returns value at i
   *
   * @param i Index
   * @return T Value at component i
   */
  T& operator[](int i) { return e[i]; }

  /**
   * @brief Addition and assignment operator
   *
   * @param v Vector to add
   * @return vec3& Summed vector
   */
  vec3& operator+=(const vec3& v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  /**
   * @brief Mutiplication and assignment operator
   *
   * @param t Value to multiple by
   * @return vec3& Scaled vector
   */
  vec3& operator*=(const T& t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  /**
   * @brief Division and assignment operator
   *
   * @param t Value to divide by
   * @return vec3& Scaled vector
   */
  vec3& operator/=(const T& t) { return *this *= 1 / t; }

  /**
   * @brief Returns the square of the norm
   *
   * @return T Square of the norm
   */
  T norm_sqr() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
  /**
   * @brief Reutrns the norm of the vector
   *
   * @return T Norm of the vector
   */
  T norm() const { return std::sqrt(norm_sqr()); }
  /**
   * @brief Returns whether we are approximately near zero
   *
   * @return true True if all indices < s
   * @return false False if any index > s
   */
  bool near_null() const {
    const T s = 1e-8;
    return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) &&
           (std::fabs(e[2]) < s);
  }

  /**
   * @brief Return a random vector
   *
   * @return vec3 Random vector
   */
  static vec3 random() {
    return vec3(random_double(), random_double(), random_double());
  }

  /**
   * @brief Returns a vector with components of random value between two input
   *
   * @param t1 Lower bound for random component
   * @param t2 Upper bound for random component
   * @return vec3 Vector with random entries as components
   */
  static vec3 random(const T& t1, const T& t2) {
    return vec3(random_double(t1, t2), random_double(t1, t2),
                random_double(t1, t2));
  }

 private:
  /**
   * @brief Array containing three component values
   *
   */
  T e[3];
};

/**
 * @brief Output value to stream
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param out ostream to print to
 * @param v Vector to print
 * @return std::ostream& Printed value
 */
template <typename T>
inline std::ostream& operator<<(std::ostream& out, const vec3<T>& v) {
  return out << v.getX() << " " << v.getY() << " " << v.getZ();
}

/**
 * @brief Binary addition operator
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param u First vector
 * @param v Second vector
 * @return vec3<T> Summed Vector
 */
template <typename T>
inline vec3<T> operator+(const vec3<T>& u, const vec3<T>& v) {
  return vec3<T>(u.getX() + v.getX(), u.getY() + v.getY(), u.getZ() + v.getZ());
}

/**
 * @brief Binary subtraction operator
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param u First vector
 * @param v Second vector
 * @return vec3<T> Subtracted vector
 */
template <typename T>
inline vec3<T> operator-(const vec3<T>& u, const vec3<T>& v) {
  return vec3<T>(u.getX() - v.getX(), u.getY() - v.getY(), u.getZ() - v.getZ());
}

/**
 * @brief Binary multiplication operator
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param u First vector
 * @param v Second vector
 * @return vec3<T> Multiplied vector
 */
template <typename T>
inline vec3<T> operator*(const vec3<T>& u, const vec3<T>& v) {
  return vec3<T>(u.getX() * v.getX(), u.getY() * v.getY(), u.getZ() * v.getZ());
}

/**
 * @brief Binary multiplication by scalar
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param t Scalar value
 * @param v Vector
 * @return vec3<T> Scaled vector
 */
template <typename T>
inline vec3<T> operator*(const T& t, const vec3<T>& v) {
  return vec3<T>(v.getX() * t, v.getY() * t, v.getZ() * t);
}

/**
 * @brief Binary multiplication by scalar
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param v Vector
 * @param t Scalar value
 * @return vec3<T> Scaled vector
 */
template <typename T>
inline vec3<T> operator*(const vec3<T>& v, const T& t) {
  return t * v;
}

/**
 * @brief Binary division operator by scalar
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param v Vector
 * @param t Scalar value
 * @return vec3<T> Scaled vector
 */
template <typename T>
inline vec3<T> operator/(const vec3<T>& v, const T& t) {
  return (1 / t) * v;
}

/**
 * @brief Perform the inner product
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param u First vector
 * @param v Second vector
 * @return T Inner product of two vectors
 */
template <typename T>
inline T dot(const vec3<T>& u, const vec3<T>& v) {
  return u.getX() * v.getX() + u.getY() * v.getY() + u.getZ() * v.getZ();
}

/**
 * @brief Perform the cross product
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param u First vector
 * @param v Second vector
 * @return vec3<T> Cross product of two vectors
 */
template <typename T>
inline vec3<T> cross(const vec3<T>& u, const vec3<T>& v) {
  return vec3<T>(u.getY() * v.getZ() - u.getZ() * v.getY(),
                 u.getZ() * v.getX() - u.getX() * v.getZ(),
                 u.getX() * v.getY() - u.getY() * v.getX());
}

/**
 * @brief Returns the unit vector
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param v Vector
 * @return vec3<T> Unit vector
 */
template <typename T>
inline vec3<T> unit_v(const vec3<T>& v) {
  return v / v.norm();
}

/**
 * @brief Reflect vector around a normal
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param v Vector ro reflect
 * @param n Normal to reflect around
 * @return vec3<T> Reflected vector
 */
template <typename T>
inline vec3<T> reflect(const vec3<T>& v, const vec3<T>& n) {
  return v - static_cast<T>(2) * dot<T>(v, n) * n;
}

/**
 * @brief Refract vector with a given index of refraction
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param uv Initial uv mapped ray
 * @param n Normal of refraction
 * @param etai_over_etat Effective index of refraction
 * @return vec3<T> Refracted ray
 */
template <typename T>
inline vec3<T> refract(const vec3<T>& uv, const vec3<T>& n, T etai_over_etat) {
  T cos_theta = std::fmin(dot<T>(-uv, n), 1.0);
  vec3<T> r_out_tang = etai_over_etat * (uv + cos_theta * n);
  vec3<T> r_out_parr =
      static_cast<T>(-std::sqrt(std::fabs(1.0 - r_out_tang.norm_sqr()))) * n;
  return r_out_parr + r_out_tang;
}

/**
 * @brief Return a random unit vector in a disk
 *
 * @tparam T Datatype to use (e.g float, double)
 * @return vec3<T> Random unit vector in a disk
 */
template <typename T>
inline vec3<T> random_disk_hat() {
  while (true) {
    vec3<T> p =
        vec3<T>(random_double(static_cast<T>(-1.0), static_cast<T>(1.0)),
                random_double(static_cast<T>(-1.0), static_cast<T>(1.0)), 0);
    if (p.norm_sqr() >= 1)
      continue;
    return p;
  }
}

/**
 * @brief Return a random vector in a sphere
 *
 * @tparam T Datatype to use (e.g float, double)
 * @return vec3<T> Random vector in a sphere
 */
template <typename T>
inline vec3<T> random_sphere() {
  while (true) {
    vec3<T> p = vec3<T>::random(static_cast<T>(-1.0), static_cast<T>(1.0));
    if (p.norm_sqr() >= 1)
      continue;
    return p;
  }
}

/**
 * @brief Return a random unit vector in spherical coordinates
 *
 * @tparam T Datatype to use (e.g float, double)
 * @return vec3<T> Random unit vector in spherical coordinates
 */
template <typename T>
inline vec3<T> random_unit_v() {
  return unit_v<T>(random_sphere<T>());
}

/**
 * @brief Return a random vector in hemi-sphere
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param n Normal of hemisphere
 * @return vec3<T> Random vector in hemisphere
 */
template <typename T>
inline vec3<T> random_half_sphere(const vec3<T>& n) {
  vec3<T> in_sphere = random_sphere<T>();
  if (dot<T>(in_sphere, n) > 0.0)
    return in_sphere;
  else
    return -in_sphere;
}

// These convenient aliases will make tracking data easier
template <typename T>
using point3 = vec3<T>;

template <typename T>
using color = vec3<T>;
