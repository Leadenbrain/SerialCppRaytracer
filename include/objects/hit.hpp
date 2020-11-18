// We want to abstract the idea of an object that can be hit
// This is what we will use as a base class for all our other objects to render
#ifndef INCLUDED_HIT_HPP
#define INCLUDED_HIT_HPP

#include "bounding_box.hpp"
#include "render/ray.hpp"
#include "utilities.hpp"

// Forward decl
template <typename T>
class material;

// We want a structure to store the data
template <typename T>
struct hit_rec {
  point3<T> p;
  vec3<T> n;
  std::shared_ptr<material<T>> mat;
  T t, u, v;
  bool front;

  // Determine whether our ray is aligned with the normal
  // ->This tells us whether we are inside or outside our object
  void set_face(const ray<T>& r, const vec3<T>& n_out) {
    front = dot(r.direction(), n_out) < 0;
    n = front ? n_out : -n_out;
  }
};

// Our base class for objects
template <typename T>
class hit {
 public:
  //  Return whether an object is hit or not
  virtual bool is_hit(const ray<T>&, const T&, const T&, hit_rec<T>&) const = 0;
  virtual bool bound_box(const T&, const T&, BB<T>& out) const = 0;
};

#endif
