#ifndef INCLUDE_SCENES_COMB_SCENE_HPP_
#define INCLUDE_SCENES_COMB_SCENE_HPP_

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

hit_list<datatype> light_scene() {
  hit_list<datatype> floor;

  auto mat_ground = std::make_shared<diffuse<T>>(color<T>(0.45, 0.36, 0.83));

  const int n_box = 20;
  for (int i = 0; i < n_box; i++) {
    for (int j = 0; j < n_box; j++) {
      datatype w = 1.000;
      datatype x0 = -10.0 + i * w;
      datatype y0 = 0.0;
      datatype z0 = -10.0 + j * w;
      datatype x1 = x0 + w;
      datatype y1 = random_double(1, 101);
      datatype z1 = z0 + w;

      floor.add(std::make_shared<cube<datatype>>(point3<datatype>(x0, y0, z0),
                                                 point3<datatype>(x1, y1, z1),
                                                 mat_ground));
    }
  }

  hit_list<datatype> objects;
  objects.add(std::make_shared<bvh_node<datatype>>(floor, 0, 1));

  objects.add(std::make_shared<xz_rectangle<datatype>>(1.2, 4.2, 1.5, 4.1, 5.54,
                                                       light));

  point3<datatype> c0 = point3<datatype>(4, 4, 4);
  point3<datatype> c1 = c0 + point3<datatype>(0.3, 0, 0);

  return objects;
}

#endif  // INCLUDE_SCENES_COMB_SCENE_HPP_
