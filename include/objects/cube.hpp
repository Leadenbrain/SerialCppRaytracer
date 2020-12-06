/**
 * @file cube.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Cube object made of rectangles
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#include "rectangle.hpp"

/**
 * @brief Cube object made of 6 square sides
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class cube : public hit<T> {
 public:
  /**
   * @brief Construct an uninitialized cube object
   *
   */
  cube();
  /**
   * @brief Construct a new cube object
   *
   */
  cube(const point3<T>&, const point3<T>&, std::shared_ptr<material<T>>);

  /**
   * @brief Compute whether ray intersects the cube
   *
   * @param r Ray to compute
   * @param t_min Initial shutter time
   * @param t_max Final shutter time
   * @param rec Hit record of ray
   * @return true True if any of the sides are hit
   * @return false False if none of the sides are hit
   */
  bool is_hit(const ray<T>& r,
              const T& t_min,
              const T& t_max,
              hit_rec<T>& rec,
              unsigned int* seed) const override {
    return sides.is_hit(r, t_min, t_max, rec, seed);
  }

  /**
   * @brief Whether we are in the bounding box
   *
   * @param out Bounding box of cube
   * @return true This always returns true
   * @return false This will never return false
   */
  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = BB<T>(min_, max_);
    return true;
  }

 private:
  /**
   * @brief Lower, front, left vertex
   *
   */
  point3<T> min_;
  /**
   * @brief Upper back, front vertex
   *
   */
  point3<T> max_;
  /**
   * @brief Sides of the cube
   *
   */
  hit_list<T> sides;
};

/**
 * @brief Construct a new cube<T>::cube object as 6 rectangles
 *
 * @tparam T Datatype to be used
 * @param min Lower, front, left vertex
 * @param max Upper, back, right vertex
 * @param m Material of cube
 */
template <typename T>
cube<T>::cube(const point3<T>& min,
              const point3<T>& max,
              std::shared_ptr<material<T>> m) {
  min_ = min;
  max_ = max;

  sides.add(std::make_shared<xy_rectangle<T>>(
      min.getX(), max.getX(), min.getY(), max.getY(), max.getZ(), m));
  sides.add(std::make_shared<xy_rectangle<T>>(
      min.getX(), max.getX(), min.getY(), max.getY(), min.getZ(), m));

  sides.add(std::make_shared<xz_rectangle<T>>(
      min.getX(), max.getX(), min.getZ(), max.getZ(), max.getY(), m));
  sides.add(std::make_shared<xz_rectangle<T>>(
      min.getX(), max.getX(), min.getZ(), max.getZ(), min.getY(), m));

  sides.add(std::make_shared<yz_rectangle<T>>(
      min.getY(), max.getY(), min.getZ(), max.getZ(), max.getX(), m));
  sides.add(std::make_shared<yz_rectangle<T>>(
      min.getY(), max.getY(), min.getZ(), max.getZ(), min.getX(), m));
}
