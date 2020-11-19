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
#include "objects/iso_fog.hpp"
#include "objects/moving_sphere.hpp"
#include "objects/rectangle.hpp"
#include "objects/sphere.hpp"
#include "objects/translation.hpp"
#include "objects/y_rotation.hpp"
#include "textures/checker.hpp"

// Colours
auto red = std::make_shared<diffuse<datatype>>(color<datatype>(.65, .05, .05));
auto white =
    std::make_shared<diffuse<datatype>>(color<datatype>(.73, .73, .73));
auto green =
    std::make_shared<diffuse<datatype>>(color<datatype>(.12, .45, .15));

// Lights
auto light =
    std::make_shared<diffuse_light<datatype>>(color<datatype>(15, 15, 15));

// Textures
auto mat_checker =
    std::make_shared<checker<datatype>>(color<datatype>(0.2, 0.3, 0.1),
                                        color<datatype>(0.9, 0.9, 0.9));

// Just an empty cornell box
hit_list<datatype> empty_cornell_box() {
  hit_list<datatype> objects;

  objects.add(
      std::make_shared<yz_rectangle<datatype>>(0, 5.55, 0, 5.55, 5.55, red));
  objects.add(
      std::make_shared<yz_rectangle<datatype>>(0, 5.55, 0, 5.55, 0, green));
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

// The standard cornell box
hit_list<datatype> standard_cornell_box() {
  hit_list<datatype> objects = empty_cornell_box();

  std::shared_ptr<hit<datatype>> b1 = std::make_shared<cube<datatype>>(
      point3<datatype>(0, 0, 0), point3<datatype>(1.65, 3.30, 1.65), white);

  b1 = std::make_shared<y_rotation<datatype>>(b1, 15);
  b1 = std::make_shared<translate<datatype>>(b1, vec3<datatype>(2.65, 0, 2.95));

  objects.add(b1);

  std::shared_ptr<hit<datatype>> b2 = std::make_shared<cube<datatype>>(
      point3<datatype>(0, 0, 0), point3<datatype>(1.65, 1.65, 1.65), white);
  b2 = std::make_shared<y_rotation<datatype>>(b2, -18);
  b2 = std::make_shared<translate<datatype>>(b2, vec3<datatype>(1.30, 0, .65));

  objects.add(b2);

  return objects;
}

hit_list<datatype> fog_cornell_box() {
  hit_list<datatype> objects = empty_cornell_box();

  std::shared_ptr<hit<datatype>> b1 = std::make_shared<cube<datatype>>(
      point3<datatype>(0, 0, 0), point3<datatype>(1.65, 3.30, 1.65), white);

  b1 = std::make_shared<y_rotation<datatype>>(b1, 15);
  b1 = std::make_shared<translate<datatype>>(b1, vec3<datatype>(2.65, 0, 2.95));

  objects.add(
      std::make_shared<iso_fog<datatype>>(b1, 1, color<datatype>(1, 1, 1)));

  std::shared_ptr<hit<datatype>> b2 = std::make_shared<cube<datatype>>(
      point3<datatype>(0, 0, 0), point3<datatype>(1.65, 1.65, 1.65), white);
  b2 = std::make_shared<y_rotation<datatype>>(b2, -18);
  b2 = std::make_shared<translate<datatype>>(b2, vec3<datatype>(1.30, 0, .65));

  objects.add(
      std::make_shared<iso_fog<datatype>>(b2, 1, color<datatype>(1, 0, 1)));

  return objects;
}

#endif
