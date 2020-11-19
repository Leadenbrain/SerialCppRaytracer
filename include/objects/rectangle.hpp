// TODO: Make rectangle base class and sub-class for planes?
#ifndef INCLUDE_OBJECTS_RECTANGLE_HPP_
#define INCLUDE_OBJECTS_RECTANGLE_HPP_

#include "bounding_box.hpp"
#include "hit.hpp"
#include "render/ray.hpp"
#include "vec3.hpp"

// Rectangle on the xy plane
template <typename T>
class xy_rectangle : public hit<T> {
 public:
  xy_rectangle() {}
  xy_rectangle(const T& x0,
               const T& x1,
               const T& y0,
               const T& y1,
               const T& k,
               std::shared_ptr<material<T>> m)
      : mat(m), x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k) {}

  bool is_hit(const ray<T>&, const T&, const T&, hit_rec<T>&) const override;

  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = BB<T>(point3<T>(x0_, y0_, k_ - 0.0001),
                point3<T>(x1_, y1_, k_ + 0.0001));
    return true;
  }

 private:
  std::shared_ptr<material<T>> mat;
  T x0_, x1_;
  T y0_, y1_;
  T k_;
};

template <typename T>
bool xy_rectangle<T>::is_hit(const ray<T>& r,
                             const T& t_min,
                             const T& t_max,
                             hit_rec<T>& rec) const {
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

// Rectangle on the xz plane
template <typename T>
class xz_rectangle : public hit<T> {
 public:
  xz_rectangle() {}
  xz_rectangle(const T& x0,
               const T& x1,
               const T& z0,
               const T& z1,
               const T& k,
               std::shared_ptr<material<T>> m)
      : mat(m), x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k){};

  bool is_hit(const ray<T>&, const T&, const T&, hit_rec<T>&) const override;

  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = BB<T>(point3<T>(x0_, z0_, k_ - 0.0001),
                point3<T>(x1_, z1_, k_ + 0.0001));
    return true;
  }

 private:
  std::shared_ptr<material<T>> mat;
  T x0_, x1_;
  T z0_, z1_;
  T k_;
};

template <typename T>
bool xz_rectangle<T>::is_hit(const ray<T>& r,
                             const T& t_min,
                             const T& t_max,
                             hit_rec<T>& rec) const {
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

// Rectangle on yz plane
template <typename T>
class yz_rectangle : public hit<T> {
 public:
  yz_rectangle() {}
  yz_rectangle(const T& y0,
               const T& y1,
               const T& z0,
               const T& z1,
               const T& k,
               std::shared_ptr<material<T>> m)
      : mat(m), y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k){};

  bool is_hit(const ray<T>&, const T&, const T&, hit_rec<T>&) const override;

  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = BB<T>(point3<T>(y0_, z0_, k_ - 0.0001),
                point3<T>(y1_, z1_, k_ + 0.0001));
    return true;
  }

 private:
  std::shared_ptr<material<T>> mat;
  T y0_, y1_;
  T z0_, z1_;
  T k_;
};

template <typename T>
bool yz_rectangle<T>::is_hit(const ray<T>& r,
                             const T& t_min,
                             const T& t_max,
                             hit_rec<T>& rec) const {
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

// template <typename T>
// bool xy_rectangle<T>::bound_box(const T& t0, const T& t1, BB<T>& out) const

#endif  // INCLUDE_OBJECTS_RECTANGLE_HPP_
