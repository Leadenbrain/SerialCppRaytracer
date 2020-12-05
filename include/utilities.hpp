/**
 * @file utilities.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief All the random functionality I am too lazy to organize elsewhere
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <vector>

// Define our own pi if it isn't for some reason
#ifndef M_PI
#define M_PI = 3.14159265358979323846
#endif

/**
 * @brief Templated infinity
 *
 * @tparam T Datatype to use (e.g float, double)
 */
template <typename T>
const T inf = std::numeric_limits<T>::infinity();

/**
 * @brief Convert from deg to rad as long double
 *
 * @param deg Angle in degree
 * @return double Angle in radians
 */
inline long double deg_to_rad(long double deg) {
  return deg * static_cast<long double>(M_PI) / 180.0;
}
/**
 * @brief Convert from deg to rad as double
 *
 * @param deg Angle in degree
 * @return double Angle in radians
 */
inline double deg_to_rad(double deg) {
  return deg * static_cast<double>(M_PI) / 180.0;
}
/**
 * @brief Convert from deg to rad as float double
 *
 * @param deg Angle in degree
 * @return double Angle in radians
 */
inline double deg_to_rad(float deg) {
  return deg * static_cast<float>(M_PI) / 180.0;
}

// Get a random number b/w 0 and 1
/**
 * @brief Get a random number b/w 0 and 1
 *
 * @tparam T Datatype to use (e.g float, double)
 * @return T Random number b/w 0 and 1
 */
template <typename T>
inline T my_rand() {
  return std::rand() / (RAND_MAX + 1.0);
}

/**
 * @brief Random number between two values
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param t1 Lower value
 * @param t2 Upper value
 * @return T Random number between two values
 */
template <typename T>
inline T my_rand_bw(T t1, T t2) {
  return t1 + (t2 - t1) * my_rand<T>();
}

/**
 * @brief Clamp a value between a minimum and maximum
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param x Value to clamp
 * @param min Minimum value to clamp to
 * @param max Maximum value to clamp to
 * @return T Clamped value
 */
template <typename T>
inline T my_clamp(const T& x, const T& min, const T& max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

/**
 * @brief Returns random double between 0 and 1
 *
 * @return double Random double between 0 and 1
 */
inline double random_double() {
  // Returns a random real in [0,1).
  return std::rand() / (RAND_MAX + 1.0);
}

/**
 * @brief Returns a random number between two values
 *
 * @param min Lower bound for random number
 * @param max Upper bound for random number
 * @return double Random number between two values
 */
inline double random_double(const double& min, const double& max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_double();
}

/**
 * @brief Returns a random int between two values
 *
 * @param min Lower bound for random number
 * @param max Upper bound for random number
 * @return int Random int between two values
 */
inline int random_int(int min, int max) {
  // Returns a random integer in [min,max].
  return static_cast<int>(random_double(min, max + 1));
}

/**
 * @brief Split a string based on a delimited
 *
 * @tparam T Datatype to use (e.g float, double)
 * @param s String to split
 * @param delim Delimiter to split on
 * @return std::vector<std::string> Split string
 */
template <typename T>
inline std::vector<std::string> split(const std::string& s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string option;
  int i = 0;

  while (getline(ss, option, delim)) {
    result.push_back(option);
    i++;
  }

  return result;
}
