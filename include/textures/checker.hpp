// Checkered pattern for texture
#ifndef INCLUDE_TEXTURES_CHECKER_HPP_
#define INCLUDE_TEXTURES_CHECKER_HPP_

#include "texture.hpp"

// Subclass of uvTex: checker
template <typename T>
class checker : public uvTex<T> {
 public:
  checker() {}
  checker(std::shared_ptr<uvTex<T>> even, std::shared_ptr<uvTex<T>> odd) {}
  checker(color<T> c1, color<T> c2)
      : even_(std::make_shared<solid<T>>(c1)),
        odd_(std::make_shared<solid<T>>(c2)) {}

  color<T> val(const T& u, const T& v, const point3<T>& p) const override {
    T s = std::sin(10 * p.getX()) * std::sin(10 * p.getY()) *
          std::sin(10 * p.getZ());
    if (s < 0)
      return odd_->val(u, v, p);
    else
      return even_->val(u, v, p);
  }

 private:
  std::shared_ptr<uvTex<T>> even_;
  std::shared_ptr<uvTex<T>> odd_;
};

#endif  // INCLUDE_TEXTURES_CHECKER_HPP_
