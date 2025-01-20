#ifndef LASER_DATA_H
#define LASER_DATA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "simdjson/simdjson.h"
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace simdjson;

std::string extract_json(const std::string& text);
void create_map(simdjson::ondemand::array &ranges, const int px_height, const int px_width, float pixels_per_meter, float Tx_robot, float Ty_robot, float R_robot);

#endif // LASER_DATA_H