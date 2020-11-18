// Just store all the random crap I'm too lazy to organize
#ifndef _INCLUDED_UTILITIES_HPP
#define _INCLUDED_UTILITIES_HPP

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Define our own pi if it isn't for some reason
#ifndef M_PI
#define M_PI = 3.14159265358979323846
#endif

// Easy to get templated infinity
template <typename T>
const T inf = std::numeric_limits<T>::infinity();

// Convert from deg to rad
inline double deg_to_rad(long double deg) {
  return deg * static_cast<long double>(M_PI) / 180.0;
}

inline double deg_to_rad(double deg) {
  return deg * static_cast<double>(M_PI) / 180.0;
}
inline double deg_to_rad(float deg) {
  return deg * static_cast<float>(M_PI) / 180.0;
}

// Get a random number b/w 0 and 1
template <typename T>
inline T my_rand() {
  return std::rand() / (RAND_MAX + 1.0);
}

// Random b/w two values
template <typename T>
inline T my_rand_bw(T t1, T t2) {
  return t1 + (t2 - t1) * my_rand<T>();
}

template <typename T>
inline T clamp(const T& x, const T& min, const T& max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

inline double random_double() {
  // Returns a random real in [0,1).
  return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(const double& min, const double& max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
  // Returns a random integer in [min,max].
  return static_cast<int>(random_double(min, max + 1));
}

#endif
