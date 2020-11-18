// Wrap our color writing into a separate file
#ifndef INCLUDED_COLOR_HPP
#define INCLUDED_COLOR_HPP

#include "vec3.hpp"

#include <iostream>
#include <sstream>

template <typename T>
void write_color(std::ostream& out,
                 const color<T>& pixel_color,
                 const int& samples) {
  T r = pixel_color.getX();
  T g = pixel_color.getY();
  T b = pixel_color.getZ();

  // Remove possible NaN and replace with zero
  if (r != r)
    r = 0.0;
  if (g != g)
    g = 0.0;
  if (b != b)
    b = 0.0;

  T scale = static_cast<T>(1.0) / samples;
  r = std::sqrt(scale * r);
  g = std::sqrt(scale * g);
  b = std::sqrt(scale * b);
  // Write the translated [0,255] value of each color component.
  // std::stringstream stream;
  out << static_cast<int>(static_cast<T>(256) * clamp<T>(r, static_cast<T>(0.0),
                                                         static_cast<T>(0.999)))
      << ' '
      << static_cast<int>(static_cast<T>(256) * clamp<T>(g, static_cast<T>(0.0),
                                                         static_cast<T>(0.999)))
      << ' '
      << static_cast<int>(static_cast<T>(256) * clamp<T>(b, static_cast<T>(0.0),
                                                         static_cast<T>(0.999)))
      << '\n';
  // out << stream.str();
}

// Determine the color of our ray after interaction
template <typename T>
color<T> ray_color(const ray<T>& r,
                   const color<T>& bg,
                   const hit<T>& world,
                   int depth) {
  hit_rec<T> rec;

  // Limit of bouncing; stop gathering light
  if (depth <= 0)
    return color<T>(0, 0, 0);

  // Return the bg when we dont hit
  if (!world.is_hit(r, 0.001, inf<T>, rec))
    return bg;

  color<T> c;
  ray<T> scat;
  color<T> light = rec.mat->emit(rec.u, rec.v, rec.p);

  // If we hit a light source
  if (!rec.mat->scatter(r, rec, c, scat))
    return light;

  return light + c * ray_color<T>(scat, bg, world, depth - 1);
}

#endif