// Let user define precision
// Default to double
#ifndef datatype
#define datatype double
#endif

#include <iostream>
#include "timer.hpp"
#include "utilities.hpp"

#include "objects/hit_list.hpp"
#include "render/ray.hpp"
#include "vec3.hpp"

#include "render/camera.hpp"
#include "render/color.hpp"

#include "scenes/cornell_box.hpp"
#include "scenes/light_scene.hpp"
#include "scenes/random_scene.hpp"

#include "objects/bounding_box.hpp"
#include "objects/bvh.hpp"

int main(void) {
  //   Set image parameters
  // TODO: Move all these configurations to config file
  const datatype aspect{16.0 / 16.0};
  const int width{1080};
  const int height{static_cast<int>(width / aspect)};
  const int ns{10000};
  const int max_depth{50};
  const color<datatype> bg(0., 0., 0.0);

  // World
  timer t_scene;
  hit_list<datatype> world = standard_cornell_box();
  t_scene.end();
  double t_end = t_scene.seconds();
  if (t_end > 1.0)
    std::cerr << "Time to build scene: " << t_end << " seconds.\n";

  // Camera
  point3<datatype> from(278, 278, -800);
  point3<datatype> to(278, 278, 1);
  vec3<datatype> vup(0, 1, 0);
  // datatype focus = (from - to).norm();
  const datatype vof{40};
  const datatype focus{10.00};
  const datatype ap{static_cast<datatype>(0.01)};
  camera<datatype> cam(from, to, vup, vof, aspect, ap, focus, 0.0, 1.0);
  //   Render our scene
  //   .ppm file size widthxheight
  std::cout << "P3\n" << width << " " << height << "\n255\n";

  // TODO: This whole thing can be subdivided for parallelism
  // also TODO: probably want this in a function
  //            -> assign parallelism from this func?
  // Let's time this, it's not going to be pretty
  timer t_render;
  // Print out r,g,b values for our scene
  for (int j = height - 1; j >= 0; --j) {
    //   Use std::cerr to print to terminal while writing file
    std::cerr << "\rLines remaining: " << j << " " << std::flush;
    for (int i = 0; i < width; ++i) {
      color<datatype> px(0, 0, 0);
      // #pragma omp parallel
      // {
      // Let's try something quick while I work, bad renders are ok for now
      // #pragma omp for reduction(+ : px)
      // ->this reduction is messy to code, do by hand instead
      for (int s = 0; s < ns; ++s) {
        datatype u = static_cast<datatype>(i + random_double()) / (width - 1);
        datatype v = static_cast<datatype>(j + random_double()) / (height - 1);
        ray<datatype> r = cam.getRay(u, v);
        px += ray_color(r, bg, world, max_depth);
        // }
      }
      write_color<datatype>(std::cout, px, ns);
    }
  }
  t_render.end();
  std::cerr << "\nFinished Render in " << t_render.seconds() / 60
            << " minutes.\n";
}
