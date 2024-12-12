#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <cstdlib>

#include "pca_connect.hpp"
#include "servo_motor.hpp"

#define NUM_SERVOS 17

void parse(const std::string& line);
void moveServos();
void setServos();
void defPosition();
void process_line(const std::string& line);

#endif
