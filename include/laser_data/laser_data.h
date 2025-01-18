#ifndef LASER_DATA_H
#define LASER_DATA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "simdjson/simdjson.h"

using namespace simdjson;

std::string extract_json(const std::string& text);

#endif // LASER_DATA_H