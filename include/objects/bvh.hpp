/**
 * @file bvh.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Class for our Bounding Volume Hierarchy. Acts as both tree and root
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#include <algorithm>
#include "bounding_box.hpp"

/**
 * @brief BVH Node class, these nodes recurse to build a tree
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class bvh_node : public hit<T> {
 public:
  /**
   * @brief Construct an uninitialized bvh node object
   *
   */
  bvh_node() {}
  /**
   * @brief Construct a new bvh node object from hit list
   *
   * @param l Hit list
   * @param t0 Initial shutter time
   * @param t1 Final shutter time
   */
  bvh_node(const hit_list<T>& l, const T& t0, const T& t1)
      : bvh_node(l.objects(), 0, l.size(), t0, t1) {}
  /**
   * @brief Construct a new bvh node object
   *
   */
  bvh_node(const std::vector<std::shared_ptr<hit<T>>>&,
           const size_t&,
           const size_t&,
           const T&,
           const T&);

  /**
   * @brief Whether we are in the bounding box
   *
   * @param out Bounding box of node
   * @return true This always returns true
   * @return false This will never return false
   */
  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = box;
    return true;
  }

  /**
   * @brief Compute whether ray intersects the BVH tree
   *
   * @param r Ray to compute
   * @param t_min Initial shutter time
   * @param t_max Final shutter time
   * @param rec Hit record of ray
   * @return true True if object is hit
   * @return false False if object is not hit
   */
  bool is_hit(const ray<T>& r,
              const T& t_min,
              const T& t_max,
              hit_rec<T>& rec) const override {
    if (!box.is_hit(r, t_min, t_max))
      return false;

    bool hit_l = left_->is_hit(r, t_min, t_max, rec);
    bool hit_r = right_->is_hit(r, t_min, hit_l ? rec.t : t_max, rec);

    return hit_l || hit_r;
  }

 private:
  /**
   * @brief Left boundary of BVH
   *
   */
  std::shared_ptr<hit<T>> left_;
  /**
   * @brief Right boundary of BVH
   *
   */
  std::shared_ptr<hit<T>> right_;
  /**
   * @brief Bounding box of the BVH tree
   *
   */
  BB<T> box;
};

/**
 * @brief Compare the boxes, a and b, to determine maximal bounding box.
 *
 * @tparam T Datatype to be used (e.g float, double)
 * @param a First box to compare
 * @param b Second box to compare
 * @param axis Axis to compare along
 * @return true True if a < b
 * @return false False if a > b
 */
template <typename T>
inline bool comp(const std::shared_ptr<hit<T>> a,
                 const std::shared_ptr<hit<T>> b,
                 const int& axis) {
  BB<T> b1;
  BB<T> b2;

  if (!a->bound_box(0, 0, b1) || !b->bound_box(0, 0, b2))
    std::cerr << "No box in bvh_node constructor \n";

  if (axis == 0)
    return b1.min().getX() < b2.min().getX();
  else if (axis == 1)
    return b1.min().getY() < b2.min().getY();
  else
    return b1.min().getZ() < b2.min().getZ();
}

/**
 * @brief Compare the boxes, a and b, along x.
 *
 * @tparam T Datatype to be used (e.g float, double)
 * @param a First box to compare
 * @param b Second box to compare
 * @return true True if a < b
 * @return false False if a > b
 */
template <typename T>
bool x_comp(const std::shared_ptr<hit<T>> a, const std::shared_ptr<hit<T>> b) {
  return comp(a, b, 0);
}

/**
 * @brief Compare the boxes, a and b, along y.
 *
 * @tparam T Datatype to be used (e.g float, double)
 * @param a First box to compare
 * @param b Second box to compare
 * @return true True if a < b
 * @return false False if a > b
 */
template <typename T>
bool y_comp(const std::shared_ptr<hit<T>> a, const std::shared_ptr<hit<T>> b) {
  return comp(a, b, 1);
}

/**
 * @brief Compare the boxes, a and b, along z.
 *
 * @tparam T Datatype to be used (e.g float, double)
 * @param a First box to compare
 * @param b Second box to compare
 * @return true True if a < b
 * @return false False if a > b
 */
template <typename T>
bool z_comp(const std::shared_ptr<hit<T>> a, const std::shared_ptr<hit<T>> b) {
  return comp(a, b, 2);
}

/**
 * @brief Construct a new bvh node<T>::bvh node object
 *
 * @tparam T Datatype to be used (e.g float, double)
 * @param src Source hit list/vector of hits
 * @param start Size of start iteration
 * @param end Size of end iteration
 * @param t0 Initial shutter time
 * @param t1 Final shutter time
 */
template <typename T>
bvh_node<T>::bvh_node(const std::vector<std::shared_ptr<hit<T>>>& src,
                      const size_t& start,
                      const size_t& end,
                      const T& t0,
                      const T& t1) {
  // Didn't want to put this code here but had errors implementing below
  std::vector<std::shared_ptr<hit<T>>> obj = src;

  int axis = random_int(0, 2);

  auto comparator =
      (axis == 0) ? x_comp<T> : (axis == 1) ? y_comp<T> : z_comp<T>;

  size_t obj_len = end - start;
  if (obj_len == 1) {
    left_ = right_ = obj[start];
  } else if (obj_len == 2) {
    if (comparator(obj[start], obj[start + 1])) {
      left_ = obj[start];
      right_ = obj[start + 1];
    } else {
      left_ = obj[start + 1];
      right_ = obj[start];
    }
  } else {
    std::sort(obj.begin() + start, obj.begin() + end, comparator);

    size_t mid = start + obj_len / 2;
    left_ = std::make_shared<bvh_node<T>>(obj, start, mid, t0, t1);
    right_ = std::make_shared<bvh_node<T>>(obj, mid, end, t0, t1);
  }

  BB<T> bL, bR;

  if (!left_->bound_box(t0, t1, bL) || !right_->bound_box(t0, t1, bR))
    std::cerr << "No box in bvh_node constructor \n";
  box = surround_box(bL, bR);
}
