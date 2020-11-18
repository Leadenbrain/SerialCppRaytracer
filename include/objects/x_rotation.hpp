// Class for making a rotated instace of an object around x-axis
// Rather than actually moving the objects, we'll just move the rays
// Pretty clever idea, still not sure how I feel about it compared to moving the
// objects themselves in the scene
// ->likely scales much better
#ifndef INCLUDED_X_ROTATION_HPP
#define INCLUDED_X_ROTATION_HPP

#include "hit.hpp"

template <typename T>
class x_rotation : public hit<T> {
 public:
  x_rotation();
  x_rotation(std::shared_ptr<hit<T>> p, const T& t_deg);

  virtual bool is_hit(const ray<T>& r,
                      const T& t_min,
                      const T& t_max,
                      hit_rec<T>& out) const override;

  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = bound_;
    return box_bool;
  }

 private:
  std::shared_ptr<hit<T>> p_;
  T sin_t, cos_t;
  bool box_bool;
  BB<T> bound_;
};

// Apply rotation transformation
// y' = cos(t)y - sin(t)z
// z' = sin(t)y + cost(t)z
template <typename T>
x_rotation<T>::x_rotation(std::shared_ptr<hit<T>> p, const T& t_deg) : p_(p) {
  T t_rad = deg_to_rad(t_deg);
  sin_t = std::sin(t_rad);
  cos_t = std::cos(t_rad);
  box_bool = p_->bound_box(0, 1, bound_);

  point3<T> min(-inf<T>, -inf<T>, -inf<T>);
  point3<T> max(inf<T>, inf<T>, inf<T>);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        T x = i * bound_.max().getX();
        T y = i * bound_.max().getY();
        T z = i * bound_.max().getZ();

        T y_out = cos_t * y - sin_t * z;
        T z_out = sin_t * y + cos_t * z;

        vec3<T> out(x, y_out, z_out);

        for (int a = 0; a < 3; a++) {
          min[a] = fmin(min[a], out[a]);
          max[a] = fmax(max[a], out[a]);
        }
      }
    }
  }

  bound_ = BB<T>(min, max);
}

// We have to apply to rotation to the normal
template <typename T>
bool x_rotation<T>::is_hit(const ray<T>& r,
                           const T& t_min,
                           const T& t_max,
                           hit_rec<T>& rec) const {
  point3<T> o = r.origin();
  vec3<T> d = r.direction();

  // Want to say this is right, might have my visual rotated
  o[1] = cos_t * r.origin()[1] + sin_t * r.origin()[2];
  o[2] = -sin_t * r.origin()[1] + cos_t * r.origin()[2];

  d[1] = cos_t * r.direction()[1] + sin_t * r.direction()[2];
  d[2] = -sin_t * r.direction()[1] + cos_t * r.direction()[2];

  ray<T> rot_r(o, d, r.time());

  if (!p_->is_hit(rot_r, t_min, t_max, rec))
    return false;

  point3<T> p = rec.p;
  vec3<T> n = rec.n;

  p[1] = cos_t * rec.p[1] - sin_t * rec.p[2];
  p[2] = sin_t * rec.p[1] + cos_t * rec.p[2];

  n[1] = cos_t * rec.n[1] - sin_t * rec.n[2];
  n[2] = sin_t * rec.n[1] + cos_t * rec.n[2];

  rec.p = p;
  rec.set_face(rot_r, n);

  return true;
}

#endif
