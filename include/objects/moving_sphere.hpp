// A moving_sphere we can render
#ifndef INCLUDED_MOVING_SPHERE_HPP
#define INCLUDED_MOVING_SPHERE_HPP

#include "bounding_box.hpp"
#include "hit.hpp"
#include "vec3.hpp"

// Subclass of hit, moving_sphere
template <typename T>
class moving_sphere : public hit<T> {
 public:
  moving_sphere() {}
  // Assign center and radius of moving_sphere
  moving_sphere(const point3<T>& cen0,
                const point3<T>& cen1,
                const T& t0,
                const T& t1,
                const T& r,
                std::shared_ptr<material<T>> m)
      : mat(m), c0_(cen0), c1_(cen1), t0_(t0), t1_(t1), r_(r) {}

  // Get the radius value
  T radius() const { return r_; }
  // Get the center
  point3<T> center(const T&) const;

  // We want to override the base function
  virtual bool is_hit(const ray<T>&,
                      const T&,
                      const T&,
                      hit_rec<T>&) const override;

  bool bound_box(const T& t0, const T& t1, BB<T>& out) const override {
    BB<T> b0(center(t0) - vec3<T>(r_, r_, r_),
             center(t0) + vec3<T>(r_, r_, r_));
    BB<T> b1(center(t1) - vec3<T>(r_, r_, r_),
             center(t1) + vec3<T>(r_, r_, r_));
    out = surround_box(b0, b1);
    return true;
  }

  // We will store the center and radius privately
 private:
  std::shared_ptr<material<T>> mat;
  point3<T> c0_, c1_;
  T t0_, t1_;
  T r_;
};

// We need to know where the center is if it moves linearly
template <typename T>
point3<T> moving_sphere<T>::center(const T& t) const {
  return c0_ + ((t - t0_) / (t1_ - t0_)) * (c1_ - c0_);
}

// Take in our ray, time frime
template <typename T>
bool moving_sphere<T>::is_hit(const ray<T>& r,
                              const T& t_min,
                              const T& t_max,
                              hit_rec<T>& rec) const {
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

#endif
