// Subclass of material for diffuse lights
// This corresponds to objects which emit diffuse light
// I have been coding for like three days straight
// ->Commenting is likely going to go to trash soon
// FIXME: Comments
#ifndef INCLUDE_MATERIALS_LIGHT_HPP_
#define INCLUDE_MATERIALS_LIGHT_HPP_

#include "material.hpp"
#include "textures/solid.hpp"

template <typename T>
class diffuse_light : public material<T> {
 public:
  diffuse_light() {}
  explicit diffuse_light(color<T> c) : c_(std::make_shared<solid<T>>(c)) {}
  explicit diffuse_light(std::shared_ptr<uvTex<T>> c) : c_(c) {}

  // We won't be scattering this time
  bool scatter(const ray<T>&,
               const hit_rec<T>&,
               color<T>&,
               ray<T>&) const override {
    return false;
  }

  color<T> emit(const T& u, const T& v, const point3<T>& p) const override {
    return c_->val(u, v, p);
  }

  //  private:
  std::shared_ptr<uvTex<T>> c_;
};

#endif  // INCLUDE_MATERIALS_LIGHT_HPP_
