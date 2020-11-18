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
#include "objects/moving_sphere.hpp"
#include "objects/rectangle.hpp"
#include "objects/sphere.hpp"
#include "textures/checker.hpp"

hit_list<datatype> cornell_box() {
  hit_list<datatype> objects;

  auto red =
      std::make_shared<diffuse<datatype>>(color<datatype>(.65, .05, .05));
  auto white =
      std::make_shared<diffuse<datatype>>(color<datatype>(.73, .73, .73));
  auto green =
      std::make_shared<diffuse<datatype>>(color<datatype>(.12, .45, .15));
  auto light =
      std::make_shared<diffuse_light<datatype>>(color<datatype>(15, 15, 15));

  objects.add(
      std::make_shared<yz_rectangle<datatype>>(0, 555, 0, 555, 555, green));
  objects.add(std::make_shared<yz_rectangle<datatype>>(0, 555, 0, 555, 0, red));
  objects.add(
      std::make_shared<xz_rectangle<datatype>>(213, 343, 227, 332, 554, light));
  objects.add(
      std::make_shared<xz_rectangle<datatype>>(0, 555, 0, 555, 0, white));
  objects.add(
      std::make_shared<xz_rectangle<datatype>>(0, 555, 0, 555, 555, white));
  objects.add(
      std::make_shared<xy_rectangle<datatype>>(0, 555, 0, 555, 555, white));

  return objects;
}

#endif
