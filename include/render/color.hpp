/**
 * @file color.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Wrapper file for color writing.
 * @details This file contains the function for calculating the pixel color for
 * a given ray, and the function to write it out.
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#include "vec3.hpp"

/**
 * @brief Print r,g,b values of pixel_color to ostream out.
 *
 * @tparam T Datatype to be used (e.g float, double)
 * @param out Ostream to print out to
 * @param pixel_color Pixel color to print out
 * @param samples Number of rays per pixel
 */
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
  out << static_cast<int>(
             static_cast<T>(256) *
             my_clamp<T>(r, static_cast<T>(0.0), static_cast<T>(0.999)))
      << ' '
      << static_cast<int>(
             static_cast<T>(256) *
             my_clamp<T>(g, static_cast<T>(0.0), static_cast<T>(0.999)))
      << ' '
      << static_cast<int>(
             static_cast<T>(256) *
             my_clamp<T>(b, static_cast<T>(0.0), static_cast<T>(0.999)))
      << '\n';
  // out << stream.str();
}

// Determine the color of our ray after interaction
/**
 * @brief Calculate the color for a particular ray.
 *
 * @tparam T Datatype to be used (e.g float, double)
 * @param r Ray to compute
 * @param bg Background color
 * @param world Hit list constaining the scene
 * @param depth Maximum recursion depth
 * @return color<T> Color of the ray
 */
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
