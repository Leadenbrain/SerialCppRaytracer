#ifndef INCLUDE_SCENES_RANDOM_SCENE_HPP_
#define INCLUDE_SCENES_RANDOM_SCENE_HPP_

#include "materials/material.hpp"
#include "utilities.hpp"
#include "vec3.hpp"

#include "objects/hit_list.hpp"

#include "materials/diffuse.hpp"
#include "materials/glass.hpp"
#include "materials/metal.hpp"
#include "objects/bvh.hpp"
#include "objects/moving_sphere.hpp"
#include "objects/sphere.hpp"
#include "textures/checker.hpp"

hit_list<datatype> random_scene() {
  hit_list<datatype> world;

  auto mat_checker = std::make_shared<checker<datatype>>(
      color<datatype>(0.2, 0.3, 0.1), color<datatype>(0.9, 0.9, 0.9));
  // auto mat_ground =
  // std::make_shared<diffuse<datatype>>(color<datatype>(0.5, 0.5, 0.5));
  world.add(std::make_shared<sphere<datatype>>(
      point3<datatype>(0, -1000, 0), 1000,
      std::make_shared<diffuse<datatype>>(mat_checker)));

  int size = 11;

  for (int i = -size; i < size; i++) {
    for (int j = -size; j < size; j++) {
      datatype mat_ = random_double();
      point3<datatype> c(i + 0.9 * random_double(), 0.2,
                         j + 0.9 * random_double());

      if ((c - point3<datatype>(4, 0.2, 0)).norm() > 0.9) {
        std::shared_ptr<material<datatype>> mat_sph;

        if (mat_ < 0.8) {
          color<datatype> col =
              color<datatype>::random() * color<datatype>::random();
          mat_sph = std::make_shared<diffuse<datatype>>(col);

          point3<datatype> c2 = c + vec3<datatype>(0, random_double(0, 0.5), 0);

          world.add(std::make_shared<moving_sphere<datatype>>(c, c2, 0.0, 1.0,
                                                              0.2, mat_sph));
        } else if (mat_ < 0.95) {
          color<datatype> col = color<datatype>::random(0.5, 1);
          datatype f = random_double(0, 0.5);
          mat_sph = std::make_shared<metal<datatype>>(col, f);
          world.add(std::make_shared<sphere<datatype>>(c, 0.2, mat_sph));
        } else {
          mat_sph = std::make_shared<glass<datatype>>(1.5);
          world.add(std::make_shared<sphere<datatype>>(c, 0.2, mat_sph));
        }
      }
    }
  }

  auto mat1 = std::make_shared<glass<datatype>>(1.5);
  auto mat2 =
      std::make_shared<diffuse<datatype>>(color<datatype>(0.4, 0.2, 0.1));
  auto mat3 =
      std::make_shared<metal<datatype>>(color<datatype>(0.7, 0.6, 0.5), 0.0);

  world.add(
      std::make_shared<sphere<datatype>>(point3<datatype>(0, 1, 0), 1.0, mat1));
  world.add(std::make_shared<sphere<datatype>>(point3<datatype>(-4, 1, 0), 1.0,
                                               mat2));
  world.add(
      std::make_shared<sphere<datatype>>(point3<datatype>(4, 1, 0), 1.0, mat3));
  // auto out_node = bvh_node<datatype>(world, 0.0, 1.0);
  // auto out_node = std::make_shared<node<datatype>>(world, 0.0, 1.0);
  return hit_list<datatype>(
      std::make_shared<bvh_node<datatype>>(world, 0.0, 0.1));
  // return world;
}

#endif  // INCLUDE_SCENES_RANDOM_SCENE_HPP_
