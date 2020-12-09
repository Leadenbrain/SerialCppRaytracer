#pragma once

#include "materials/diffuse.hpp"
#include "materials/diffuse_light.hpp"
#include "materials/glass.hpp"
#include "materials/metal.hpp"
#include "objects/bvh.hpp"
#include "objects/cube.hpp"
#include "objects/hit_list.hpp"
#include "objects/iso_fog.hpp"
#include "objects/mesh.hpp"
#include "objects/moving_sphere.hpp"
#include "objects/sphere.hpp"
#include "objects/translation.hpp"
#include "objects/triangle.hpp"
#include "objects/y_rotation.hpp"
#include "textures/checker.hpp"

#ifndef datatype
#define datatype double
#endif

// // Colours
// auto red = std::make_shared<diffuse<datatype>>(color<datatype>(.65, .05,
// .05)); auto white =
//     std::make_shared<diffuse<datatype>>(color<datatype>(.73, .73, .73));
// auto green =
//     std::make_shared<diffuse<datatype>>(color<datatype>(.12, .45, .15));

// // Lights
// auto light = std::make_shared<diffuse_light<datatype>>(
//     color<datatype>(26.656, 15.375, 3.5625));

// // Textures
auto mat_checker =
    std::make_shared<checker<datatype>>(color<datatype>(0.2, 0.3, 0.1),
                                        color<datatype>(0.9, 0.9, 0.9));

auto red = std::make_shared<diffuse<datatype>>(color<datatype>(.65, .05, .05));

// Lights
auto light =
    std::make_shared<diffuse_light<datatype>>(color<datatype>(2, 2, 2));

hit_list<datatype> mesh_scene() {
  hit_list<datatype> objects;

  objects.add(std::make_shared<sphere<datatype>>(
      point3<datatype>(0, -1000, 0), 1000,
      std::make_shared<diffuse<datatype>>(mat_checker)));

  objects.add(std::make_shared<xz_rectangle<datatype>>(-343, 343, -332, 332,
                                                       548.7, light));
  objects.add(std::make_shared<mesh<datatype>>("bunny.obj", red));

  return objects;
}