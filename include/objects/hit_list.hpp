/**
 * @file hit_list.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief List of hittable objects
 * @details The idea here is that we can just store objects we want to hit in a
 * list. This allows our abstraction of objects that can be hit become useful
 * ->Just store everything we can hit in a list
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "hit.hpp"

/**
 * @brief List of hittable objects
 *
 * @tparam T Datatype to be used
 */
template <typename T>
class hit_list : public hit<T> {
 public:
  /**
   * @brief Construct an unitialized hit list object
   *
   */
  hit_list() {}
  /**
   * @brief Construct a new hit list object with one object
   *
   * @param obj Object to add to list
   */
  explicit hit_list(std::shared_ptr<hit<T>> obj) { add(obj); }
  /**
   * @brief Construct a new hit list object from a hit list
   *
   * @param hit_l Hit list to pass into new list
   */
  explicit hit_list(std::shared_ptr<hit_list<T>> hit_l) : obj_list(hit_l) {}

  /**
   * @brief Append object to the hit list
   *
   * @param obj Object to add to list
   */
  void add(std::shared_ptr<hit<T>> obj) { obj_list.push_back(obj); }
  /**
   * @brief Clear the object list
   *
   */
  void clear() { obj_list.clear(); }
  /**
   * @brief Return the size of the hit list
   *
   * @return T Datatype to be used
   */
  T size() const { return obj_list.size(); }

  /**
   * @brief Return the objects in the hist list
   *
   * @return std::vector<std::shared_ptr<hit<T>>> Vector of objects
   */
  std::vector<std::shared_ptr<hit<T>>> objects() const { return obj_list; }

  /**
   * @brief Compute whether ray intersects the hit list
   *
   * @return true Returns true if object is hit
   * @return false Returns true if objects is hit
   */
  bool is_hit(const ray<T>&, const T&, const T&, hit_rec<T>&) const override;

  /**
   * @brief Whether we are in the bounding box
   *
   * @param t0 Initial shutter time
   * @param t1 Final shutter time
   * @param out Bounding box of cube
   * @return true True if we are in the hit list bounding box
   * @return false False if we are outside of the hit list bounding box
   */
  bool bound_box(const T& t0, const T& t1, BB<T>& out) const override {
    if (obj_list.empty())
      return false;

    BB<T> temp;
    bool first = true;

    for (const auto& obj : obj_list) {
      if (!obj->bound_box(t0, t1, temp))
        return false;
      out = first ? temp : surround_box(out, temp);
      first = false;
    }

    return true;
  }

 private:
  /**
   * @brief Vector of objects in the hit list
   *
   */
  std::vector<std::shared_ptr<hit<T>>> obj_list;
};

/**
 * @brief Compute whether the ray intersects the hit list
 *
 * @tparam T Datatype to be used
 * @param r Ray to compute
 * @param t_min Initial shutter time
 * @param t_max Final shutter time
 * @param rec Hit record of ray
 * @return true Returns true if object is hit
 * @return false Returns true if objects is hit
 */
template <typename T>
bool hit_list<T>::is_hit(const ray<T>& r,
                         const T& t_min,
                         const T& t_max,
                         hit_rec<T>& rec) const {
  hit_rec<T> temp;
  bool hit_ = false;
  T best_guess = t_max;

  // Go through our obj list, see if they hit
  for (const std::shared_ptr<hit<T>>& obj : obj_list) {
    if (obj->is_hit(r, t_min, best_guess, temp)) {
      hit_ = true;
      best_guess = temp.t;
      rec = temp;
    }
  }
  return hit_;
}
