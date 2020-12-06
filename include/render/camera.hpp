/**
 * @file camera.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Camera object to render scene from
 * @details An object for storing our camera data. Contains the initialization
 * of the camera and its configuration in the world.
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

// TODO: these parameters chould be in some sort of .yaml or something
#pragma once

#include "ray.hpp"

/**
 * @brief The camera class
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class camera {
 public:
  /**
   * @brief Construct a new camera object
   *
   * @param from Point camera is looking from
   * @param to Point camera is looking to
   * @param vup Direction vector of vertical up
   * @param fov Field of view
   * @param ar Aspect ratio
   * @param ap Aperture size
   * @param focus Focal length
   * @param t0 Initial shutter time
   * @param t1 Final shutter time
   */
  camera(const point3<T>& from,
         const point3<T>& to,
         const vec3<T>& vup,
         const T& fov,
         const T& ar,
         const T& ap,
         const T& focus,
         const T& t0 = 0,
         const T& t1 = 0) {
    T t = deg_to_rad(fov);
    T h = std::tan(t / static_cast<T>(2));
    T vp_height = static_cast<T>(2.0) * h;
    T vp_width = ar * vp_height;
    // T focus = 1.0;

    w = unit_v<T>(from - to);
    u = unit_v<T>(cross<T>(vup, w));
    v = cross<T>(w, u);

    t0_ = t0;
    t1_ = t1;

    o_ = from;
    hor_ = focus * vp_width * u;
    ver_ = focus * vp_height * v;
    llc_ = o_ - hor_ / static_cast<T>(2.0) - ver_ / static_cast<T>(2.0) -
           focus * w;

    r_ = ap / static_cast<T>(2);
  }

  /**
   * @brief Get the Ray object
   *
   * @param s u value of pixel
   * @param t v value of pixel
   * @return ray<T> Ray at (u,v)
   */
  ray<T> getRay(const T& s, const T& t, unsigned int* seed) const {
    vec3<T> rd = r_ * random_disk_hat<T>(seed);
    vec3<T> off = u * rd.getX() + v * rd.getY();

    return ray<T>(o_ + off, llc_ + s * hor_ + t * ver_ - o_ - off,
                  random_double(t0_, t1_, seed));
    // return ray<T>(o_, llc_ + s * hor_ + t * ver_ - o_);
  }

 private:
  /**
   * @brief Origin of camera ray
   *
   */
  point3<T> o_;
  /**
   * @brief Lower left corner of camera view
   *
   */
  point3<T> llc_;
  /**
   * @brief Horizontal view
   *
   */
  vec3<T> hor_;
  /**
   * @brief Vertival view
   *
   */
  vec3<T> ver_;
  /**
   * @brief (u,v,w) values -> Texture mapped x,y,z
   *
   */
  vec3<T> u, v, w;
  /**
   * @brief Aperture radius, initial shutter time, final shutter time
   *
   */
  T r_, t0_, t1_;
};
