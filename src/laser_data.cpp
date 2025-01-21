#include "../include/laser_data/laser_data.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/simdjson/simdjson.h"

using namespace simdjson;

std::string extract_json(const std::string &text)
{
    size_t start = text.find("---START---");
    size_t end = text.find("___END___");

    if (start == std::string::npos || end == std::string::npos)
    {
        // throw std::runtime_error("Invalid format: Missing ---START--- or ___END___");
        std::cout << "Invalid format: Missing ---START--- or ___END___" << std::endl;
    }

    start += std::string("---START---").length();
    return text.substr(start, end - start);
}

void create_map(ondemand::array &ranges, const int px_height, const int px_width, float pixels_per_meter, float Tx_robot, float Ty_robot, float R_robot)
{

    float meters_per_pixel = 1.0 / pixels_per_meter; // Calculate the number of meters per pixel
    int px;                                          // X coordinate of a measured point in pixel coordinates
    int py;                                          // Y coordinate of a measured point in pixel coordinates
    int px_offset = px_width / 2;                    // Offset of the X coordinate in pixel coordinates to put robot in the center
    int py_offset = px_height / 2;                   // Offset of the Y coordinate in pixel coordinates to put robot in the center
    float xw;                                        // X coordinate of a measured point in world coordinates
    float yw;                                        // Y coordinate of a measured point in world coordinates
    float xr;                                        // X coordinate of a measured point in robot coordinates
    float yr;                                        // Y coordinate of a measured point in robot coordinates
    float xrw;                                       // X element of the vector from the robot base to a measured point in world coordinates
    float yrw;                                       // Y element of the vector from the robot base to a measured point in world coordinates
    float xerw;                                      // X element of the unit vector from the robot base to a measured point in world coordinates
    float yerw;                                      // Y element of the unit vector from the robot base to a measured point in world coordinates
    int pxrw;                                        // X coordinate of a point along the xrw yrw vector in pixel coordinates
    int pyrw;                                        // Y coordinate of a point along the xrw yrw vector in pixel coordinates

    static cv::Mat img = cv::Mat::ones(px_height, px_width, CV_8UC1) * 100; // Create a grey canvas (unknown space) which will be overwritten with the laser scan

    int i = 0;
    for (auto range : ranges)
    {
        if (i >= 360)
            break; // Ensure we do not exceed the array bounds

        xr = float(range) * cos(i * M_PI / 180);               // Convert polar coordinates to Cartesian coordinates
        yr = float(range) * sin(i * M_PI / 180);               // Convert polar coordinates to Cartesian coordinates
        xw = xr * cos(R_robot) - yr * sin(R_robot) + Tx_robot; // Convert robot coordinates to world coordinates
        yw = xr * sin(R_robot) + yr * cos(R_robot) + Ty_robot; // Convert robot coordinates to world coordinates
        xrw = xw - Tx_robot;                                   // Calculate the vector from the robot base to the measured point in world coordinates
        yrw = yw - Ty_robot;                                   // Calculate the vector from the robot base to the measured point in world coordinates
        xerw = xrw / sqrt(xrw * xrw + yrw * yrw);              // Calculate the unit vector from the robot base to the measured point in world coordinates
        yerw = yrw / sqrt(xrw * xrw + yrw * yrw);              // Calculate the unit vector from the robot base to the measured point in world coordinates
        px = int(xw * pixels_per_meter + px_offset);           // Convert world coordinates to pixel coordinates
        py = int(yw * pixels_per_meter + py_offset);           // Convert world coordinates to pixel coordinates

        int index = 0;

        while (sqrt(pow(xerw * meters_per_pixel * index, 2) + pow(yerw * meters_per_pixel * index, 2)) <= sqrt(pow(xr, 2) + pow(yr, 2))) // If the length of the vector from the robot base to the measured point in world coordinates is less than the length of the vector from the robot base to the measured point in robot coordinates
        {
            pxrw = int((xerw * meters_per_pixel * index + Tx_robot) * pixels_per_meter + px_offset); // Convert world coordinates to pixel coordinates
            pyrw = int((yerw * meters_per_pixel * index + Ty_robot) * pixels_per_meter + py_offset); // Convert world coordinates to pixel coordinates
            cv::circle(img, cv::Point(pxrw, pyrw), 2, cv::Scalar(255), -1);                          // Create white pixels along the laser scan for free space
            index++;
        }
        index = 0;

        if (px >= 0 && px < px_width && py >= 0 && py < px_height)
        {
            cv::circle(img, cv::Point(px, py), 2, cv::Scalar(0), -1); // Create a black pixel for the obstacle
        }
        else
        {
            std::cout << "Point out of image boundaries" << std::endl;
        }

        i++;
    }

    cv::imshow("Laser scan", img);

    cv::waitKey(1);
}
/*
int main(void) {
    // Read the entire file content
    std::ifstream file("laserdata.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string file_content = buffer.str();

    // Extract JSON content
    std::string json_content = extract_json(file_content);

    // Parse the JSON content
    ondemand::parser parser;
    ondemand::document data = parser.iterate(json_content);

    // Access ranges array
    ondemand::array ranges = data["ranges"];

    std::cout << "Ranges: " << std::endl;

    size_t count = 0;
    for (auto dist : ranges) {
        std::cout << double(dist) << std::endl;
        count++;
    }

    std::cout << "Number of ranges: " << count << std::endl;

    return 0;
}
*/