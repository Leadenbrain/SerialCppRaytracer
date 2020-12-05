/**
 * @file bounding_box.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Bounding box object, used to compute BVH tree
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#include <utility>
#include "render/ray.hpp"

/**
 * @brief Bounding box object to reduce hit computation time
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class BB {
 public:
  /**
   * @brief Construct an uninitialized BB object
   *
   */
  BB() {}
  /**
   * @brief Construct a new BB object
   *
   * @param min Lower left boundary
   * @param max Upper Right boundary
   */
  BB(const point3<T>& min, const point3<T> max) : min_(min), max_(max) {}

  /**
   * @brief Return the minimum boundary point
   *
   * @return point3<T> Minimum boundary point
   */
  point3<T> min() const { return min_; }
  /**
   * @brief Return the maximum boundary point
   *
   * @return point3<T> Maximum boundary point
   */
  point3<T> max() const { return max_; }

  /**
   * @brief Compute whether ray intersects the bounding box
   *
   * @param r Ray to compute
   * @param t_min Initial shutter time
   * @param t_max Final shutter time
   * @return true True if object is hit
   * @return false False if object is not hit
   */
  bool is_hit(const ray<T>& r, const T& t_min, const T& t_max) const {
    T ti = t_min;
    T tf = t_max;
    for (int i = 0; i < 3; i++) {
      T invD = static_cast<T>(1.0) / r.direction()[i];
      T t0 = (min()[i] - r.origin()[i]) * invD;
      T t1 = (max()[i] - r.origin()[i]) * invD;

      if (invD < 0.0f)
        std::swap(t0, t1);
      ti = t0 > ti ? t0 : ti;
      tf = t1 < tf ? t1 : tf;
      if (tf <= ti)
        return false;
    }
    return true;
  }

  /**
   * @brief Return the longest axis
   *
   * @return int Index of longest axis
   */
  int axis() const {
    T x = max_.getX() - min_.getX();
    T y = max_.getY() - min_.getY();
    T z = max_.getZ() - min_.getZ();

    if (x > y && x > z)
      return 0;
    else if (y > z)
      return 1;
    else
      return 2;
  }

  /**
   * @brief Return the area of the bounding box
   *
   * @return T Area of bounding box
   */
  T area() const {
    T x = max_.getX() - min_.getX();
    T y = max_.getY() - min_.getY();
    T z = max_.getZ() - min_.getZ();
    return static_cast<T>(2) * (x * y + y * z + x * z);
  }

 private:
  /**
   * @brief Minimum boundary point
   *
   */
  point3<T> min_;
  /**
   * @brief Maximum boundary point
   *
   */
  point3<T> max_;
};

/**
 * @brief Generate a new bounding box containing two input boxes
 *
 * @tparam T Datatype to be used (e.g float, double)
 * @param bb0 First bounding box
 * @param bb1 Second bounding box
 * @return BB<T> Bounding box containing both input boxes
 */
template <typename T>
BB<T> surround_box(BB<T> bb0, BB<T> bb1) {
  vec3<T> small(fmin(bb0.min().getX(), bb1.min().getX()),
                fmin(bb0.min().getY(), bb1.min().getY()),
                fmin(bb0.min().getZ(), bb1.min().getZ()));
  vec3<T> big(fmax(bb0.max().getX(), bb1.max().getX()),
              fmax(bb0.max().getY(), bb1.max().getY()),
              fmax(bb0.max().getZ(), bb1.max().getZ()));

  return BB<T>(small, big);
}
