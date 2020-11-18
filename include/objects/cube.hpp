#ifndef INCLUDED_CUBE_HPP
#define INCLUDED_CUBE_HPP

#include "vec3.hpp"

#include "bounding_box.hpp"
#include "hit_list.hpp"
#include "rectangle.hpp"

template <typename T>
class cube : public hit<T> {
 public:
  cube();
  cube(const point3<T>&, const point3<T>&, std::shared_ptr<material<T>>);

  bool is_hit(const ray<T>& r,
              const T& t_min,
              const T& t_max,
              hit_rec<T>& rec) const override {
    return sides.is_hit(r, t_min, t_max, rec);
  }

  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = BB<T>(min_, max_);
    return true;
  }

 private:
  point3<T> min_;
  point3<T> max_;
  hit_list<T> sides;
};

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

#endif
