#ifndef INCLUDED_CORNELL_SCENE_HPP
#define INCLUDED_CORNELL_SCENE_HPP

#include "materials/material.hpp"
#include "utilities.hpp"
#include "vec3.hpp"

#include "objects/hit_list.hpp"

#include "materials/diffuse.hpp"
#include "materials/diffuse_light.hpp"
#include "materials/glass.hpp"
#include "materials/metal.hpp"
#include "objects/bvh.hpp"
#include "objects/cube.hpp"
#include "objects/moving_sphere.hpp"
#include "objects/rectangle.hpp"
#include "objects/sphere.hpp"
#include "textures/checker.hpp"

hit_list<datatype> empty_cornell_box() {
  hit_list<datatype> objects;

  auto mat_checker = std::make_shared<checker<datatype>>(
      color<datatype>(0.2, 0.3, 0.1), color<datatype>(0.9, 0.9, 0.9));

  auto red =
      std::make_shared<diffuse<datatype>>(color<datatype>(.65, .05, .05));
  auto white =
      std::make_shared<diffuse<datatype>>(color<datatype>(.73, .73, .73));
  auto green =
      std::make_shared<diffuse<datatype>>(color<datatype>(.12, .45, .15));
  auto light =
      std::make_shared<diffuse_light<datatype>>(color<datatype>(15, 15, 15));

  objects.add(
      std::make_shared<yz_rectangle<datatype>>(0, 5.55, 0, 5.55, 5.55, green));
  objects.add(
      std::make_shared<yz_rectangle<datatype>>(0, 5.55, 0, 5.55, 0, red));
  objects.add(std::make_shared<xz_rectangle<datatype>>(2.13, 3.43, 2.27, 3.32,
                                                       5.54, light));
  objects.add(
      std::make_shared<xz_rectangle<datatype>>(0, 5.55, 0, 5.55, 0, white));
  objects.add(
      std::make_shared<xz_rectangle<datatype>>(0, 5.55, 0, 5.55, 5.55, white));
  objects.add(
      std::make_shared<xy_rectangle<datatype>>(0, 5.55, 0, 5.55, 5.55, white));

  return objects;
}

hit_list<datatype> standard_cornell_box() {
  hit_list<datatype> objects;

  auto mat_checker = std::make_shared<checker<datatype>>(
      color<datatype>(0.2, 0.3, 0.1), color<datatype>(0.9, 0.9, 0.9));

  auto red =
      std::make_shared<diffuse<datatype>>(color<datatype>(.65, .05, .05));
  auto white =
      std::make_shared<diffuse<datatype>>(color<datatype>(.73, .73, .73));
  auto green =
      std::make_shared<diffuse<datatype>>(color<datatype>(.12, .45, .15));
  auto light =
      std::make_shared<diffuse_light<datatype>>(color<datatype>(15, 15, 15));

  objects.add(
      std::make_shared<yz_rectangle<datatype>>(0, 5.55, 0, 5.55, 5.55, green));
  objects.add(
      std::make_shared<yz_rectangle<datatype>>(0, 5.55, 0, 5.55, 0, red));
  objects.add(std::make_shared<xz_rectangle<datatype>>(2.13, 3.43, 2.27, 3.32,
                                                       5.54, light));
  objects.add(
      std::make_shared<xz_rectangle<datatype>>(0, 5.55, 0, 5.55, 0, white));
  objects.add(
      std::make_shared<xz_rectangle<datatype>>(0, 5.55, 0, 5.55, 5.55, white));
  objects.add(
      std::make_shared<xy_rectangle<datatype>>(0, 5.55, 0, 5.55, 5.55, white));

  objects.add(std::make_shared<cube<datatype>>(
      point3<datatype>(1.30, 0, 0.65), point3<datatype>(2.95, 1.65, 2.30),
      white));
  objects.add(std::make_shared<cube<datatype>>(
      point3<datatype>(2.65, 0, 2.95), point3<datatype>(4.30, 3.30, 4.60),
      white));

  return objects;
}

#endif
