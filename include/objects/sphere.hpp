// A sphere we can render
#ifndef INCLUDED_SPHERE_HPP
#define INCLUDED_SPHERE_HPP

#include "hit.hpp"
#include "vec3.hpp"

// Subclass of hit: sphere
template <typename T>
class sphere : public hit<T> {
 public:
  sphere() {}
  // Assign center and radius of sphere
  sphere(const point3<T>& cen, const T& r, std::shared_ptr<material<T>> m)
      : mat(m), c_(cen), r_(r) {}

  // Get the radius value
  T radius() const { return r_; }
  // Get the center
  point3<T> center() const { return c_; }

  // We want to override the base function
  virtual bool is_hit(const ray<T>&,
                      const T&,
                      const T&,
                      hit_rec<T>&) const override;

  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = BB<T>(c_ - vec3<T>(r_, r_, r_), c_ + vec3<T>(r_, r_, r_));
    return true;
  }

  // We will store the center and radius privately
 private:
  std::shared_ptr<material<T>> mat;
  point3<T> c_;
  T r_;

  static void get_sph_uv(const point3<T>& p, T& u, T& v) {
    T t = std::acos(-p.getY());
    T phi = std::atan2(-p.getZ(), p.getX()) * M_PI;

    u = phi / (static_cast<T>(2) * M_PI);
    v = t / M_PI;
  }
};

// Take in our ray, time frime
template <typename T>
bool sphere<T>::is_hit(const ray<T>& r,
                       const T& t_min,
                       const T& t_max,
                       hit_rec<T>& rec) const {
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

#endif
