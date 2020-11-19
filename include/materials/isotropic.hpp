#ifndef INCLUDED_ISOTROPIC_HPP
#define INCLUDED_ISOTROPIC_HPP

#include "material.hpp"
#include "textures/solid.hpp"
#include "utilities.hpp"
#include "vec3.hpp"

template <typename T>
class isotropic : public material<T> {
 public:
  isotropic(std::shared_ptr<uvTex<T>>(c)) : c_(c) {}
  isotropic(color<T> c) : c_(std::make_shared<solid<T>>(c)) {}

  bool scatter(const ray<T>& r,
               const hit_rec<T>& rec,
               color<T>& att,
               ray<T>& scat) const override {
    scat = ray<T>(rec.p, random_sphere<T>(), r.time());
    att = c_->val(rec.u, rec.v, rec.p);
    return true;
  }

 private:
  std::shared_ptr<uvTex<T>> c_;
};

#endif
