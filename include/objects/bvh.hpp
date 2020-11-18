#ifndef INCLUDED_BVH_HPP
#define INCLUDED_BVH_HPP

#include <algorithm>
#include "bounding_box.hpp"
#include "hit.hpp"
#include "hit_list.hpp"
#include "utilities.hpp"

template <typename T>
class bvh_node : public hit<T> {
 public:
  bvh_node() {}
  bvh_node(const hit_list<T>& l, const T& t0, const T& t1)
      : bvh_node(l.objects(), 0, l.size(), t0, t1) {}
  bvh_node(const std::vector<std::shared_ptr<hit<T>>>&,
           const size_t&,
           const size_t&,
           const T&,
           const T&);

  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = box;
    return true;
  }

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
  std::shared_ptr<hit<T>> left_;
  std::shared_ptr<hit<T>> right_;
  BB<T> box;
};

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

template <typename T>
bool x_comp(const std::shared_ptr<hit<T>> a, const std::shared_ptr<hit<T>> b) {
  return comp(a, b, 0);
}

template <typename T>
bool y_comp(const std::shared_ptr<hit<T>> a, const std::shared_ptr<hit<T>> b) {
  return comp(a, b, 1);
}

template <typename T>
bool z_comp(const std::shared_ptr<hit<T>> a, const std::shared_ptr<hit<T>> b) {
  return comp(a, b, 2);
}

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

#endif
