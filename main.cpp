/**
 * @file main.cpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Main file for Serial ray tracer project
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef datatype
#define datatype double
#endif

#include <omp.h>

#include "config_parser.hpp"
#include "timer.hpp"

#include "objects/hit_list.hpp"

#include "render/camera.hpp"
#include "render/color.hpp"

#include "scenes/comb_scene.hpp"
// #include "scenes/cornell_box.hpp"
// #include "scenes/light_scene.hpp"
// #include "scenes/random_scene.hpp"

/**
 * @brief Main function to perform scene generation and rendering
 *
 * @param argc Number of arguments
 * @param argv Vector of arguments
 * @return int Success/Error code
 */
int main(int argc, char* argv[]) {
  std::string file_name;
  if (argc < 2)
    file_name = "low.rt";
  else
    file_name = argv[1];
  std::unordered_map<std::string, double> opts;
  std::unordered_map<std::string, vec3<double>> vec_opts;

  parser parser(file_name);
  parser.parse_data();
  opts = parser.data();
  vec_opts = parser.vec_data();

  const datatype aspect{static_cast<datatype>(opts["aspect"])};
  const int width{static_cast<int>(opts["width"])};
  const int height{static_cast<int>(width / aspect) + 1};
  const int ns{static_cast<int>(opts["ns"])};
  const int max_depth{static_cast<int>(opts["max_depth"])};
  const color<datatype> bg{vec_opts["bg"]};

  // World
  timer t_scene;
  unsigned int numb = 123;
  hit_list<datatype> world = comb_scene(&numb);
  t_scene.end();
  double t_end = t_scene.seconds();
  if (t_end > 1.0)
    std::cerr << "Time to build scene: " << t_end << " seconds.\n";

  // Camera
  point3<datatype> from(vec_opts["from"]);
  point3<datatype> to(vec_opts["to"]);
  vec3<datatype> vup(vec_opts["vup"]);
  const datatype vof{opts["vof"]};
  const datatype focus{opts["focus"]};
  const datatype ap{opts["ap"]};
  camera<datatype> cam(from, to, vup, vof, aspect, ap, focus, 0.0, 1.0);
  //   Render our scene
  //   .ppm file size widthxheight
  std::cout << "P3\n" << width << " " << height << "\n255\n";

  // Let's time this, it's not going to be pretty
  timer t_render;

  color<double>* pixels = new color<double>[width * height];
#pragma omp parallel for
  for (int i = 0; i < width * height; i++) {
    unsigned int my_seed = omp_get_thread_num();
    color<double> pix = color<double>(0, 0, 0);
    for (int s = 0; s < ns; ++s) {
      int x, y;
      x = i % width;
      y = ((i - (i % width)) / width);
      datatype u =
          static_cast<datatype>(x + (rand_r(&my_seed) / (RAND_MAX + 1.0))) /
          (width - 1);
      datatype v =
          static_cast<datatype>(y + (rand_r(&my_seed) / (RAND_MAX + 1.0))) /
          (height - 1);
      ray<datatype> r = cam.getRay(u, v, &numb);
      pix += ray_color(r, bg, world, max_depth, &numb);
    }
    pixels[i] = pix;
  }

  for (int j = height - 1; j >= 0; j--) {
    for (int i = 0; i < width; i++) {
      size_t px = j * width + i;
      write_color<datatype>(std::cout, pixels[px], ns);
    }
  }

  // Print out r,g,b values for our scene
  // for (int j = height - 1; j >= 0; --j) {
  //   //   Use std::cerr to print to terminal while writing file
  //   std::cerr << "\rLines remaining: " << j << " " << std::flush;
  //   for (int i = 0; i < width; ++i) {
  //     color<datatype> px(0, 0, 0);
  //     // #pragma omp parallel
  //     // {
  //     // Let's try something quick while I work, bad renders are ok for now
  //     // #pragma omp for reduction(+ : px)
  //     // ->this reduction is messy to code, do by hand instead
  //     for (int s = 0; s < ns; ++s) {
  //       datatype u = static_cast<datatype>(i + random_double()) / (width -
  //       1); datatype v = static_cast<datatype>(j + random_double()) / (height
  //       - 1); ray<datatype> r = cam.getRay(u, v); px += ray_color(r, bg,
  //       world, max_depth);
  //       // }
  //     }
  //     write_color<datatype>(std::cout, px, ns);
  //   }
  // }

  t_render.end();
  std::cerr << "\nFinished Render in " << t_render.seconds() / 60
            << " minutes.\n";
}
