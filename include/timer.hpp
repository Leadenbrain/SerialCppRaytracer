/**
 * @file timer.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Simple chrono timer object
 * @version 0.1
 * @date 2020-12-04
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <chrono>
#include <iostream>

using namespace std;

/**
 * @brief Chrono timer object
 *
 */
class timer {
 public:
  /**
   * @brief Stop the timer and set the elapsed time
   *
   */
  void end() { elapsed_time = chrono::steady_clock::now() - start_time; }

  /**
   * @brief Returns the elapsed time in seconds
   *
   * @return double Elapsed time in seconds
   */
  double seconds() const { return elapsed_time.count(); }

 private:
  /**
   * @brief Starting time
   *
   */
  chrono::time_point<chrono::steady_clock> start_time{
      chrono::steady_clock::now()};
  /**
   * @brief elapsed time
   *
   */
  chrono::duration<double> elapsed_time{0};
};