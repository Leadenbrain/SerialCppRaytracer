// Class for making a translated instace of an object
// Rather than actually moving the objects, we'll just move the rays
// Pretty clever idea, still not sure how I feel about it compared to moving the
// objects themselves in the scene
// ->likely scales much better
#ifndef INCLUDE_OBJECTS_TRANSLATION_HPP_
#define INCLUDE_OBJECTS_TRANSLATION_HPP_

#include "hit.hpp"

template <typename T>
class translate : public hit<T> {
 public:
  translate() {}
  translate(std::shared_ptr<hit<T>> p, const vec3<T>& disp)
      : p_(p), disp_(disp) {}

  bool is_hit(const ray<T>& r,
              const T& t_min,
              const T& t_max,
              hit_rec<T>& rec) const override {
    ray<T> trans_r(r.origin() - disp_, r.direction(), r.time());
    if (!p_->is_hit(trans_r, t_min, t_max, rec))
      return false;

    rec.p += disp_;
    rec.set_face(trans_r, rec.n);

    return true;
  }

  bool bound_box(const T& t0, const T& t1, BB<T>& out) const override {
    if (!p_->bound_box(t0, t1, out))
      return false;

    out = BB<T>(out.min() + disp_, out.max() + disp_);
    return true;
  }

 private:
  std::shared_ptr<hit<T>> p_;
  vec3<T> disp_;
};

#endif  // INCLUDE_OBJECTS_TRANSLATION_HPP_
