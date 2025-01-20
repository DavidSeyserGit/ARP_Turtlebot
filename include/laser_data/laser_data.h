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
void calc_pixel_coord(simdjson::ondemand::array &ranges, std::vector<std::vector<float>> &pixel_array, const int px_height, const int px_width, float pixels_per_meter, float Tx_robot, float Ty_robot, float R_robot);

#endif // LASER_DATA_H