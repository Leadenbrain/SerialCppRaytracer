// An object for storing our camera data
// There isn't too much going on here, just setting up the screen and camera
// location/configuration
// TODO: these parameters chould be in some sort of .yaml or something
#ifndef INCLUDE_RENDER_CAMERA_HPP_
#define INCLUDE_RENDER_CAMERA_HPP_

#include "ray.hpp"
#include "utilities.hpp"
#include "vec3.hpp"

template <typename T>
class camera {
 public:
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

  ray<T> getRay(const T& s, const T& t) const {
    vec3<T> rd = r_ * random_disk_hat<T>();
    vec3<T> off = u * rd.getX() + v * rd.getY();

    return ray<T>(o_ + off, llc_ + s * hor_ + t * ver_ - o_ - off,
                  random_double(t0_, t1_));
    // return ray<T>(o_, llc_ + s * hor_ + t * ver_ - o_);
  }

 private:
  point3<T> o_;
  point3<T> llc_;
  vec3<T> hor_;
  vec3<T> ver_;
  vec3<T> u, v, w;
  T r_, t0_, t1_;
};

#endif  // INCLUDE_RENDER_CAMERA_HPP_
