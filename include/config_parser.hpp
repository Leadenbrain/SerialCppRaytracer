// Very crude configuration parser; I'd typically use yaml-cpp

#ifndef INCLUDE_CONFIG_PARSER_HPP_
#define INCLUDE_CONFIG_PARSER_HPP_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "utilities.hpp"
#include "vec3.hpp"

// template <typename T>
class parser {
 public:
  //   parser() : file_{"config/default.rt"} {}
  explicit parser(const std::string& file)
      : file_(std::string("config/") + file) {}

  std::unordered_map<std::string, double> data() const { return data_; }
  std::unordered_map<std::string, vec3<double>> vec_data() const {
    return vec_data_;
  }

  void parse_data() { parse_data(file_); }
  void parse_data(const std::string&);

 private:
  std::string file_;
  std::unordered_map<std::string, double> data_;
  std::unordered_map<std::string, vec3<double>> vec_data_;
};

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

#endif  // INCLUDE_CONFIG_PARSER_HPP_
