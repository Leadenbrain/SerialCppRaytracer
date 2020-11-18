// The idea here is that we can just store objects we want to hit in a list
// This allows our abstraction of objects that can be hit become useful
// ->Just store everything we can hit in a list
#ifndef INCLUDED_HIT_LIST_HPP
#define INCLUDED_HIT_LIST_HPP

// We want shared pointers so we can share data like materials
#include <memory>
#include <vector>
#include "hit.hpp"
#include "utilities.hpp"

template <typename T>
class hit_list : public hit<T> {
 public:
  hit_list() {}
  // If we have an object, lets initialize with it
  explicit hit_list(std::shared_ptr<hit<T>> obj) { add(obj); }
  // Or let's pass lists to lists
  explicit hit_list(std::shared_ptr<hit_list<T>> hit_l) : obj_list(hit_l) {}

  // Append object to our list
  void add(std::shared_ptr<hit<T>> obj) { obj_list.push_back(obj); }
  // Clear the object list
  void clear() { obj_list.clear(); }
  // Return size of object list
  T size() const { return obj_list.size(); }
  // Return object list
  std::vector<std::shared_ptr<hit<T>>> objects() const { return obj_list; }

  // Overriding our base function
  virtual bool is_hit(const ray<T>&,
                      const T&,
                      const T&,
                      hit_rec<T>&) const override;
  // Overriding our base function
  // -If obj_list is empty returns false
  // -
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

  // Not sure if I can make this private and do what I want with it
  // TODO: look into making this private -> friend?
 private:
  std::vector<std::shared_ptr<hit<T>>> obj_list;
};

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

#endif
