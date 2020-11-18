#ifndef INCLUDED_BOUNDING_BOX_HPP
#define INCLUDED_BOUNDING_BOX_HPP

#include <cmath>
#include <utility>
#include "hit.hpp"
#include "render/ray.hpp"
#include "vec3.hpp"

template <typename T>
class BB {
 public:
  BB() {}
  BB(const point3<T>& min, const point3<T> max) : min_(min), max_(max) {}

  point3<T> min() const { return min_; }
  point3<T> max() const { return max_; }

  // Kensler's method
#ifdef KENSLER
  bool is_hit(const ray<T>& r, const T& t_min, const T& t_max) const {
    // FIXME: I had issues with the bounding boxes tracking motion blur
    // I think I need to make a t_min/max variable and store it to get both
    // speed and accuracy in this calculation
    // -> produces -Wunused error
    // Right now I'm over estimating the area to calc (providing little speedup
    // w/ motion blur on)
    // TODO: Add a compilation toggle for MB?
    // This method over calcs, but is fast w/o motion blur (i.e no moving
    // spheres)
    //      -> I expect to see issues with my 2D objects...
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
#else
  // This method is fast, but won't track MB correctly
  // ->Underlying algo is slower, but lack of MB makes it faster
  bool is_hit(const ray<T>& r, const T& t_min, const T& t_max) const {
    T ti = t_min;
    T tf = t_max;
    for (int i = 0; i < 3; i++) {
      T t0 = fmin((min_[i] - r.origin()[i]) / r.direction()[i],
                  (max_[i] - r.origin()[i]) / r.direction()[i]);
      T t1 = fmax((min_[i] - r.origin()[i]) / r.direction()[i],
                  (max_[i] - r.origin()[i]) / r.direction()[i]);

      ti = fmax(t0, ti);
      tf = fmin(t1, tf);

      if (tf <= ti)
        return false;
    }
    return true;
  }
#endif  // End of kensler section

  // Return which axis is longest
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

  T area() const {
    T x = max_.getX() - min_.getX();
    T y = max_.getY() - min_.getY();
    T z = max_.getZ() - min_.getZ();
    return static_cast<T>(2) * (x * y + y * z + x * z);
  }

 private:
  point3<T> min_;
  point3<T> max_;
};

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

#endif
