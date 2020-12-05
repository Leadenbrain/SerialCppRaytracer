/**
 * @file iso_fog.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Isotropically scattering fog object
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#ifndef ACNE
#define ACNE 0.0001
#endif

#include "hit.hpp"
#include "materials/isotropic.hpp"

/**
 * @brief Isotropically scattering fog object
 *
 * @tparam T Datatype to be used (e.g float, double)
 */
template <typename T>
class iso_fog : public hit<T> {
 public:
  /**
   * @brief Construct a new iso fog object from a color
   *
   * @param bound Boundary of object to make fog
   * @param rho Density of the fog
   * @param c Color of the fog
   */
  iso_fog(std::shared_ptr<hit<T>> bound, const T& rho, color<T> c)
      : bound_(bound),
        rho_(-1 / rho),
        phase_(std::make_shared<isotropic<T>>(c)) {}
  /**
   * @brief Construct a new iso fog object from a texture
   *
   * @param bound Boundary of the object to make fog
   * @param rho Density of the fog
   * @param c Texture of the fog
   */
  iso_fog(std::shared_ptr<hit<T>> bound,
          const T& rho,
          std::shared_ptr<uvTex<T>> c)
      : bound_(bound),
        rho_(-1 / rho),
        phase_(std::make_shared<isotropic<T>>(c)) {}

  /**
   * @brief Returns whether a ray intersects the fog
   *
   * @return true True if the ray hits the fog
   * @return false False if the ray does not hit the fog
   */
  bool is_hit(const ray<T>&, const T&, const T&, hit_rec<T>&) const override;

  /**
   * @brief Returns whether we are inside the bounding box of the fog
   *
   * @param t0 Initial shutter time
   * @param t1 Final shutter time
   * @param out Bounding box of object
   * @return true True if we are in the fog bounding box
   * @return false False if we are outside of the fog bounding box
   */
  bool bound_box(const T& t0, const T& t1, BB<T>& out) const override {
    return bound_->bound_box(t0, t1, out);
  }

 private:
  /**
   * @brief Boundary object to make fog
   *
   */
  std::shared_ptr<hit<T>> bound_;
  /**
   * @brief Inverse density of fog
   *
   */
  T rho_;
  /**
   * @brief Material to make fog
   *
   */
  std::shared_ptr<material<T>> phase_;
};

/**
 * @brief Compute whether the ray intersects the fog object
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
bool iso_fog<T>::is_hit(const ray<T>& r,
                        const T& t_min,
                        const T& t_max,
                        hit_rec<T>& rec) const {
  hit_rec<T> rec0, rec1;

  // if not hitting bounding box through passing
  if (!bound_->is_hit(r, -inf<T>, inf<T>, rec0))
    return false;

  // realizing now I hard code this 0.0001 acne factor a lot
  // FIXME: look back through code and replace acne with definition
  // if not hitting bound as we travel through
  if (!bound_->is_hit(r, rec0.t + ACNE, inf<T>, rec1))
    return false;

  // set min and max if over calcing
  if (rec0.t < t_min)
    rec0.t = t_min;
  if (rec1.t > t_max)
    rec1.t = t_max;

  if (rec0.t >= rec1.t)
    return false;

  if (rec0.t < 0)
    rec0.t = 0;

  const T r_len = r.direction().norm();
  const T d_bound = (rec1.t - rec0.t) * r_len;
  const T dist = rho_ * std::log(random_double());

  // if scatter puts us outside
  if (dist > d_bound)
    return false;

  rec.t = rec0.t + dist / r_len;
  rec.p = r.at(rec.t);
  rec.n = vec3<T>(1, 0, 0);
  rec.front = true;
  rec.mat = phase_;

  return true;
}
