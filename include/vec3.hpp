// 3-Vector class, also point3 and color
// Pretty self-explanatory, nothing fishy here
#ifndef INCLUDED_VEC3_HPP
#define INCLUDED_VEC3_HPP

#include <cmath>
#include <iostream>

#include "utilities.hpp"

template <typename T>
class vec3 {
 public:
  vec3() : e{0, 0, 0} {}
  explicit vec3(T e) : e{e, e, e} {}
  vec3(T e0, T e1, T e2) : e{e0, e1, e2} {}
  //   vec3(const vec3& v) : e{v.e} {}

  T getX() const { return e[0]; }
  T getY() const { return e[1]; }
  T getZ() const { return e[2]; }

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  T operator[](int i) const { return e[i]; }
  T& operator[](int i) { return e[i]; }

  vec3& operator+=(const vec3& v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  // We'll only have to worry about scalar mult
  vec3& operator*=(const T& t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  // Same for division
  vec3& operator/=(const T& t) { return *this *= 1 / t; }

  // Square of the norm
  T norm_sqr() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
  // The norm
  T norm() const { return std::sqrt(norm_sqr()); }
  // We need to know if length is practically null
  bool near_null() const {
    const T s = 1e-8;
    return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) &&
           (std::fabs(e[2]) < s);
  }

  static vec3 random() {
    return vec3(random_double(), random_double(), random_double());
  }

  static vec3 random(const T& t1, const T& t2) {
    return vec3(random_double(t1, t2), random_double(t1, t2),
                random_double(t1, t2));
  }

 private:
  T e[3];
};

// We'll overload the rest of the operators here
template <typename T>
inline std::ostream& operator<<(std::ostream& out, const vec3<T>& v) {
  return out << v.getX() << " " << v.getY() << " " << v.getZ();
}

template <typename T>
inline vec3<T> operator+(const vec3<T>& u, const vec3<T>& v) {
  return vec3<T>(u.getX() + v.getX(), u.getY() + v.getY(), u.getZ() + v.getZ());
}

template <typename T>
inline vec3<T> operator-(const vec3<T>& u, const vec3<T>& v) {
  return vec3<T>(u.getX() - v.getX(), u.getY() - v.getY(), u.getZ() - v.getZ());
}

template <typename T>
inline vec3<T> operator*(const vec3<T>& u, const vec3<T>& v) {
  return vec3<T>(u.getX() * v.getX(), u.getY() * v.getY(), u.getZ() * v.getZ());
}

template <typename T>
inline vec3<T> operator*(const T& t, const vec3<T>& v) {
  return vec3<T>(v.getX() * t, v.getY() * t, v.getZ() * t);
}

template <typename T>
inline vec3<T> operator*(const vec3<T>& v, const T& t) {
  return t * v;
}

template <typename T>
inline vec3<T> operator/(const vec3<T>& v, const T& t) {
  return (1 / t) * v;
}

// Perform dot product
template <typename T>
inline T dot(const vec3<T>& u, const vec3<T> v) {
  return u.getX() * v.getX() + u.getY() * v.getY() + u.getZ() * v.getZ();
}

// Perofrm cross product
template <typename T>
inline vec3<T> cross(const vec3<T>& u, const vec3<T>& v) {
  return vec3<T>(u.getY() * v.getZ() - u.getZ() * v.getY(),
                 u.getZ() * v.getX() - u.getX() * v.getZ(),
                 u.getX() * v.getY() - u.getY() * v.getX());
}

// Return the unit vector
template <typename T>
inline vec3<T> unit_v(const vec3<T>& v) {
  return v / v.norm();
}

// Reflections
template <typename T>
inline vec3<T> reflect(const vec3<T>& v, const vec3<T>& n) {
  return v - static_cast<T>(2) * dot<T>(v, n) * n;
}

template <typename T>
inline vec3<T> refract(const vec3<T>& uv, const vec3<T>& n, T etai_over_etat) {
  T cos_theta = std::fmin(dot<T>(-uv, n), 1.0);
  vec3<T> r_out_tang = etai_over_etat * (uv + cos_theta * n);
  vec3<T> r_out_parr =
      static_cast<T>(-std::sqrt(std::fabs(1.0 - r_out_tang.norm_sqr()))) * n;
  return r_out_parr + r_out_tang;
}

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

template <typename T>
inline vec3<T> random_sphere() {
  while (true) {
    vec3<T> p = vec3<T>::random(static_cast<T>(-1.0), static_cast<T>(1.0));
    if (p.norm_sqr() >= 1)
      continue;
    return p;
  }
}

template <typename T>
inline vec3<T> random_unit_v() {
  return unit_v<T>(random_sphere<T>());
}

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

#endif
