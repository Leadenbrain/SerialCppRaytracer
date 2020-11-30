// Let user define precision
// Default to double
#ifndef datatype
#define datatype double
#endif

#include <iostream>
#include "config_parser.hpp"
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

  const datatype aspect{opts["aspect"]};
  const int width{static_cast<int>(opts["width"])};
  const int height{static_cast<int>(width / aspect) + 1};
  const int ns{static_cast<int>(opts["ns"])};
  const int max_depth{static_cast<int>(opts["max_depth"])};
  const color<datatype> bg{vec_opts["bg"]};

  // World
  timer t_scene;
  hit_list<datatype> world = standard_cornell_box();
  t_scene.end();
  double t_end = t_scene.seconds();
  if (t_end > 1.0)
    std::cerr << "Time to build scene: " << t_end << " seconds.\n";

  // Camera
  point3<datatype> from(vec_opts["from"]);
  point3<datatype> to(vec_opts["to"]);
  // point3<datatype> from(2.78, 2.78, -8.00);
  // point3<datatype> to(2.78, 2.78, 1);
  vec3<datatype> vup(vec_opts["vup"]);
  // datatype focus = (from - to).norm();
  const datatype vof{opts["vof"]};
  const datatype focus{opts["focus"]};
  const datatype ap{opts["ap"]};
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
