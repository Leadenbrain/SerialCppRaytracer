/**
 * @file checker.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Two color checkered texture
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "texture.hpp"

// Subclass of uvTex: checker
/**
 * @brief Two color checker subclass uvTex
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class checker : public uvTex<T> {
 public:
  /**
   * @brief Construct an uninitialized checker object
   *
   */
  checker() {}
  /**
   * @brief Construct a new checker object from textures
   *
   * @param even Texture to apply to even squares
   * @param odd Texture to apply to odd squares
   */
  checker(std::shared_ptr<uvTex<T>> even, std::shared_ptr<uvTex<T>> odd) {}
  /**
   * @brief Construct a new checker object from rgb colors
   *
   * @param c1 Color for even squares
   * @param c2 Color for odd squares
   */
  checker(color<T> c1, color<T> c2)
      : even_(std::make_shared<solid<T>>(c1)),
        odd_(std::make_shared<solid<T>>(c2)) {}

  /**
   * @brief Return the value for a point at a given (u,v)
   *
   * @param u u value of texture map
   * @param v v value of texture map
   * @param p Point to determine color
   * @return color<T> Color of the point on texture map
   */
  color<T> val(const T& u, const T& v, const point3<T>& p) const override {
    T s = std::sin(10 * p.getX()) * std::sin(10 * p.getY()) *
          std::sin(10 * p.getZ());
    if (s < 0)
      return odd_->val(u, v, p);
    else
      return even_->val(u, v, p);
  }

 private:
  /**
   * @brief Texture for even squares
   *
   */
  std::shared_ptr<uvTex<T>> even_;
  /**
   * @brief Texture for odd squares
   *
   */
  std::shared_ptr<uvTex<T>> odd_;
};
