/**
 * @file config_parser.hpp
 * @author Dylan Bassi (bassidj@mcmaster.ca)
 * @brief Configuration parser for render and camera settings
 * @version 0.1
 * @date 2020-12-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "vec3.hpp"

// template <typename T>
/**
 * @brief Parser for reading in the configuration for rendering
 *
 */
class parser {
 public:
  /**
   * @brief Construct a new parser object
   *
   * @param file Name of the file to parse
   */
  explicit parser(const std::string& file)
      : file_(std::string("config/") + file) {}

  /**
   * @brief Return the data in the config file
   *
   * @return std::unordered_map<std::string, double> Configuration data
   */
  std::unordered_map<std::string, double> data() const { return data_; }
  /**
   * @brief Return the vector data in the config file
   *
   * @return std::unordered_map<std::string, vec3<double>> Vector data in config
   * file
   */
  std::unordered_map<std::string, vec3<double>> vec_data() const {
    return vec_data_;
  }

  /**
   * @brief Parse the data in the file by default
   *
   */
  void parse_data() { parse_data(file_); }
  /**
   * @brief Parse the data in a given file
   *
   */
  void parse_data(const std::string&);

 private:
  /**
   * @brief Name of the configuration file
   *
   */
  std::string file_;
  /**
   * @brief Non vector data in config file
   *
   */
  std::unordered_map<std::string, double> data_;
  /**
   * @brief Vector data in the config file
   *
   */
  std::unordered_map<std::string, vec3<double>> vec_data_;
};

/**
 * @brief Parse the data in a given file
 *
 * @param file Name of the file to parse
 */
void parser::parse_data(const std::string& file) {
  std::ifstream input{file};
  // try/catch?
  if (!input)
    perror("Error opening input file");

  std::string s;
  std::string temp_s;
  double x, y, z;
  std::vector<std::string> ss;

  while (getline(input, s))
    if (!((s[0] == '#') || (isspace(s[0])))) {
      ss = split<double>(s, '=');
      if (ss[1][0] == ',') {
        x = std::stod(split<double>(ss[1], ',')[1]);
        y = std::stod(split<double>(ss[1], ',')[2]);
        z = std::stod(split<double>(ss[1], ',')[3]);

        vec_data_[ss[0]] = vec3<double>(x, y, z);
      } else {
        data_[ss[0]] = std::stod(split<double>(s, '=')[1]);
      }
    }
  input.close();
}
