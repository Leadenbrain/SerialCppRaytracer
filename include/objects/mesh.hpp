#pragma once

#include <fstream>
#include <sstream>

#include "hit.hpp"
#include "hit_list.hpp"
#include "materials/material.hpp"
#include "triangle.hpp"

template <typename T>
class mesh : public hit<T> {
 public:
  mesh() {}
  mesh(const std::string& file, std::shared_ptr<material<T>> m);

  void print() const {}

  bool is_hit(const ray<T>& r,
              const T& t_min,
              const T& t_max,
              hit_rec<T>& rec) const override;

  bool bound_box(const T&, const T&, BB<T>& out) const override {
    out = BB<T>(min_, max_);
    return true;
  }

 private:
  std::shared_ptr<material<T>> m_;
  std::ifstream file_;
  point3<T> min_, max_;
  hit_list<T> faces;
  std::vector<vec3<T>> verts;
  //   triangle<T>* faces[];
  //   vec3<T>* verts[];
};

template <typename T>
mesh<T>::mesh(const std::string& file, std::shared_ptr<material<T>> m) : m_(m) {
  std::ifstream file_(file);
  std::string line, op;
  std::stringstream lstream;
  std::vector<std::string> ss;

  vec3<T> temp_pos;
  T x, y, z;
  int count{0};
  int i = 0;
  T minx(10), miny(10), minz(10);
  T maxx(0), maxy(0), maxz(0);
  while (std::getline(file_, line)) {
    lstream << line;
    lstream >> op;
    ss = split<T>(line, ' ');
    // std::cerr << ss[0] << "\n";
    if (line[0] == 'v') {
      // std::cerr << "PRINT \n";
      // lstream >> x >> y >> z;
      x = std::stod(ss[1]);
      y = std::stod(ss[2]);
      z = std::stod(ss[3]);
      if (x > minx)
        maxx = x;
      if (y > miny)
        maxy = y;
      if (z > minz)
        maxz = z;
      if (x < minx)
        minx = x;
      if (y < miny)
        miny = y;
      if (z < minz)
        minz = z;

      point3<T> temp_vert(x, y, z);
      verts.push_back(1.0 * temp_vert);
    } else if (line[0] == 'f') {
      int v0, v1, v2;
      v0 = std::stoi(ss[1]);
      v1 = std::stoi(ss[2]);
      v2 = std::stoi(ss[3]);
      // lstream >> v0 >> v1 >> v2;
      faces.add(std::make_shared<triangle<T>>(
          verts.at(v0 - 1), verts.at(v1 - 1), verts.at(v2 - 1), 0, m));
    }
  }
  min_ = point3<T>(minx, miny, minz);
  max_ = point3<T>(maxx, maxy, maxz);
}

template <typename T>
bool mesh<T>::is_hit(const ray<T>& r,
                     const T& t_min,
                     const T& t_max,
                     hit_rec<T>& rec) const {
  return faces.is_hit(r, t_min, t_max, rec);
  // vec3<T> n(0, 0, 0);
  // T thit = 10000000.0;
  // T t = t_max;

  // bool hit = false;
  // for (auto& f : faces) {
  //   if (f.is_hit(r, t_min, t_max, rec)) {
  //     if (rec.t < thit) {
  //       thit = rec.t;
  //       n = rec.n;
  //     }
  //     hit = true;
  //   }
  // }

  // rec.t = t;
  // rec.p = r.at(t);
  // rec.set_face(r, n);
  // rec.n = n;
  // return hit;
}