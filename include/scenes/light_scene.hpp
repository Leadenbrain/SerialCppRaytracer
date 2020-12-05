/**
 * @file light_scene.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Header file for light scene example (unfinished)
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "materials/diffuse.hpp"
#include "materials/diffuse_light.hpp"
#include "materials/glass.hpp"
#include "materials/metal.hpp"
#include "objects/bvh.hpp"
#include "objects/moving_sphere.hpp"
#include "objects/rectangle.hpp"
#include "objects/sphere.hpp"
#include "textures/checker.hpp"

/**
 * @brief Construct a scene with metal and glass sphere, with spherical and
 * rectangular lights
 *
 * @return hit_list<datatype> Hit list containing the scene.
 */
hit_list<datatype> light_scene() {
  hit_list<datatype> world;

  auto mat_ground =
      std::make_shared<diffuse<datatype>>(color<datatype>(0.5, 0.5, 0.5));

  auto mat3 =
      std::make_shared<metal<datatype>>(color<datatype>(0.7, 0.6, 0.5), 0.0);

  auto mat_checker = std::make_shared<checker<datatype>>(
      color<datatype>(0.2, 0.3, 0.1), color<datatype>(0.9, 0.9, 0.9));

  auto mat_metal =
      std::make_shared<metal<datatype>>(color<datatype>(0.7, 0.1, 0.8), 0.5);

  auto light1 =
      std::make_shared<diffuse_light<datatype>>(color<datatype>(2, 4, 8));
  auto light2 =
      std::make_shared<diffuse_light<datatype>>(color<datatype>(8, 4, 2));

  auto mat_glass = std::make_shared<glass<datatype>>(1.5);

  world.add(std::make_shared<sphere<datatype>>(
      point3<datatype>(0, -1000, 0), 1000,
      std::make_shared<diffuse<datatype>>(mat_checker)));
  world.add(std::make_shared<sphere<datatype>>(point3<datatype>(0, 1, 0), 1,
                                               mat_glass));
  world.add(std::make_shared<sphere<datatype>>(point3<datatype>(0, 1, 2), 1,
                                               mat_metal));
  world.add(
      std::make_shared<sphere<datatype>>(point3<datatype>(0, 7, 0), 2, light1));
  world.add(std::make_shared<xy_rectangle<datatype>>(-3, 3, 1, 3, -6, light2));

  return hit_list<datatype>(
      std::make_shared<bvh_node<datatype>>(world, 0.0, 1.0));
}
