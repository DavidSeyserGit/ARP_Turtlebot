#include "laser_data/laser_data.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include "simdjson/simdjson.h"

using namespace simdjson;

std::string extract_json(const std::string &text)
{
    size_t start = text.find("---START---");
    size_t end = text.find("---END---");

    if (start == std::string::npos || end == std::string::npos)
    {
        throw std::runtime_error("Invalid format: Missing ---START--- or ---END---");
    }

    start += std::string("---START---").length();
    return text.substr(start, end - start);
}

void calc_pixel_coord(ondemand::array &ranges, std::vector<std::vector<float>> &pixel_array, const int px_height, const int px_width, float Tx_robot, float Ty_robot, float R_robot)
{
    int px;                        // X coordinate of a measured point in pixel coordinates
    int py;                        // Y coordinate of a measured point in pixel coordinates
    int px_offset = px_width / 2;  // Offset of the X coordinate in pixel coordinates to put robot in the center
    int py_offset = px_height / 2; // Offset of the Y coordinate in pixel coordinates to put robot in the center
    float xw;                      // X coordinate of a measured point in world coordinates
    float yw;                      // Y coordinate of a measured point in world coordinates
    float xr;                      // X coordinate of a measured point in robot coordinates
    float yr;                      // Y coordinate of a measured point in robot coordinates

    // cv::Mat img = cv::Mat::zeros(px_height, px_width, CV_8UC1);
    cv::Mat img = cv::Mat::ones(px_height, px_width, CV_8UC1) * 255;

    int i = 0;
    for (auto range : ranges)
    {
        if (i >= 360)
            break; // Ensure we do not exceed the array bounds

        xr = float(range) * cos(i * M_PI / 180);               // Convert polar coordinates to Cartesian coordinates
        yr = float(range) * sin(i * M_PI / 180);               // Convert polar coordinates to Cartesian coordinates
        xw = xr * cos(R_robot) - yr * sin(R_robot) + Tx_robot; // Convert robot coordinates to world coordinates
        yw = xr * sin(R_robot) + yr * cos(R_robot) + Ty_robot; // Convert robot coordinates to world coordinates
        px = int(xw * 144 + px_offset);                        // Convert world coordinates to pixel coordinates
        py = int(yw * 144 + py_offset);                        // Convert world coordinates to pixel coordinates

        if (px >= 0 && px < px_width && py >= 0 && py < px_height)
        {
            pixel_array[px][py] = 0.0; // Set the pixel value to 0.0 if the point is within the image boundaries
            // img.at<uchar>(py, px) = 0;
            cv::circle(img, cv::Point(px, py), 2, cv::Scalar(0), -1);
        }
        else
        {
            std::cout << "Point out of image boundaries" << std::endl;
        }

        i++;
    }

    // cv::Mat img(px_height, px_width, CV_16U, pixel_array.data());
    
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