/**
 * @file comb_scene.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Header file for Combined scene example (unfinished)
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#include "objects/hit_list.hpp"

#include "materials/diffuse.hpp"
#include "materials/diffuse_light.hpp"
#include "materials/glass.hpp"
#include "materials/metal.hpp"
#include "objects/bvh.hpp"
#include "objects/cube.hpp"
#include "objects/iso_fog.hpp"
#include "objects/moving_sphere.hpp"
#include "objects/sphere.hpp"
#include "objects/translation.hpp"
#include "objects/y_rotation.hpp"
#include "textures/checker.hpp"

#ifndef datatype
#define datatype double
#endif

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

/**
 * @brief Construct a hit list containing our scene to render
 *
 * @return hit_list<datatype> Our combined scene to render
 */
hit_list<datatype> comb_scene(unsigned int* seed) {
  hit_list<datatype> floor;

  auto mat_ground =
      std::make_shared<diffuse<datatype>>(color<datatype>(0.45, 0.36, 0.83));

  const int n_box = 20;
  for (int i = 0; i < n_box; i++) {
    for (int j = 0; j < n_box; j++) {
      datatype w = 100.0;
      datatype x0 = -1000 + i * w;
      datatype y0 = 0.0;
      datatype z0 = -1000.0 + j * w;
      datatype x1 = x0 + w;
      datatype y1 = random_double(1, 101, seed);
      datatype z1 = z0 + w;

      floor.add(std::make_shared<cube<datatype>>(point3<datatype>(x0, y0, z0),
                                                 point3<datatype>(x1, y1, z1),
                                                 mat_ground));
    }
  }

  hit_list<datatype> objects;
  objects.add(std::make_shared<bvh_node<datatype>>(floor, 0, 1, seed));

  objects.add(
      std::make_shared<xz_rectangle<datatype>>(123, 423, 147, 412, 554, light));

  point3<datatype> c0 = point3<datatype>(400, 400, 400);
  point3<datatype> c1 = c0 + point3<datatype>(30, 0, 0);
  auto mat_sphere_moving =
      std::make_shared<diffuse<datatype>>(color<datatype>(0.7, 0.3, 0.1));
  objects.add(std::make_shared<moving_sphere<datatype>>(c0, c1, 0, 1, 50,
                                                        mat_sphere_moving));

  objects.add(std::make_shared<sphere<datatype>>(
      point3<datatype>(260, 150, 45), 50,
      std::make_shared<glass<datatype>>(1.7)));

  objects.add(std::make_shared<sphere<datatype>>(
      point3<datatype>(0, 150, 145), 50,
      std::make_shared<metal<datatype>>(color<datatype>(0.7, 0.2, 0.3), 0.1)));

  auto boundary = std::make_shared<sphere<datatype>>(
      point3<datatype>(360, 150, 145), 70,
      std::make_shared<glass<datatype>>(1.5));

  objects.add(boundary);
  objects.add(std::make_shared<iso_fog<datatype>>(
      boundary, 0.2, color<datatype>(0.2, 0.4, 0.9)));
  boundary = std::make_shared<sphere<datatype>>(
      point3<datatype>(0, 0, 0), 5000, std::make_shared<glass<datatype>>(1.5));
  objects.add(std::make_shared<iso_fog<datatype>>(boundary, 0.0001,
                                                  color<datatype>(1, 1, 1)));

  auto glass2 = std::make_shared<glass<datatype>>(1.3);
  objects.add(std::make_shared<sphere<datatype>>(
      point3<datatype>(400, 200, 400), 100, glass2));
  auto metal2 =
      std::make_shared<metal<datatype>>(color<datatype>(0.1, 0.4, 0.8), 1.0);
  objects.add(std::make_shared<sphere<datatype>>(
      point3<datatype>(220, 280, 300), 80, metal2));

  //   hit_list<datatype> sph_cube;
  //   int n_spheres = 1000;
  //   for (int i = 0; i < n_spheres; i++) {
  //     white = std::make_shared<diffuse<datatype>>(
  //         color<datatype>(random_double(), random_double(),
  //         random_double()));
  //     if (random_double() > 0.8)
  //       sph_cube.add(std::make_shared<sphere<datatype>>(
  //           point3<datatype>::random(0, 165), 10, white));
  //     else if (random_double() < 0.9)
  //       sph_cube.add(std::make_shared<sphere<datatype>>(
  //           point3<datatype>::random(0, 165), 10, glass2));
  //     else
  //       sph_cube.add(std::make_shared<sphere<datatype>>(
  //           point3<datatype>::random(0, 165), 10,
  //           std::make_shared<metal<datatype>>(
  //               color<datatype>(random_double(), random_double(),
  //                               random_double()),
  //               random_double())));
  //   }

  //   objects.add(std::make_shared<translate<datatype>>(
  //       std::make_shared<y_rotation<datatype>>(
  //           std::make_shared<bvh_node<datatype>>(sph_cube, 0.0, 1.0), 15),
  //       vec3<datatype>(-100, 270, 395)));

  return objects;
}
