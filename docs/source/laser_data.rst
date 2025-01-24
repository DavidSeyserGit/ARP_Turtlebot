laser_data Module
=================

The `laser_data` module provides functions for reading laser scan data and visualizing it on a map. This module is essential for mapping the environment and processing sensor data.

extract_json
------------
Processes a given string and extracts relevant data in JSON format.

.. cpp:function:: std::string extract_json(const std::string &text)

   Extracts JSON-formatted data from the given text input.

   :param text: The input string containing the data to be processed.
   :type text: const std::string&
   :return: A string containing the extracted JSON data.
   :return type: std::string

.. cpp:function:: void create_map(ondemand::array &ranges, const int px_height, const int px_width, float pixels_per_meter, float Tx_robot, float Ty_robot, float R_robot)

   Creates a map from the given laser scan data.

   :param ranges: The array of range values from the laser scan.
   :type ranges: ondemand::array
   :param px_height: The height of the map in pixels.
   :type px_height: const int
   :param px_width: The width of the map in pixels.
   :type px_width: const int
   :param pixels_per_meter: The scale factor for converting meters to pixels.
   :type pixels_per_meter: float
   :param Tx_robot: The x-coordinate of the robot's position.
   :type Tx_robot: float
   :param Ty_robot: The y-coordinate of the robot's position.
   :type Ty_robot: float
   :param R_robot: The orientation of the robot.
   :type R_robot: float
   :return: None

Examples
--------

### Using `extract_json` and `create_map`

The following example demonstrates how to read laser scan data from a file, extract the JSON content, and use it to generate a 2D map.

.. code-block:: cpp

   #include "../include/laser_data/laser_data.h"
   #include "../include/simdjson/simdjson.h"
   #include <fstream>
   #include <iostream>
   #include <sstream>
   #include <opencv2/opencv.hpp>

   int main() {
       try {
           // Step 1: Read laser scan data from a file
           std::ifstream file("laserdata.txt");
           if (!file) {
               throw std::runtime_error("Failed to open laserdata.txt");
           }
           std::stringstream buffer;
           buffer << file.rdbuf();
           std::string file_content = buffer.str();

           // Step 2: Extract JSON content from the file
           std::string json_content = extract_json(file_content);

           // Step 3: Parse the JSON content to get ranges
           simdjson::ondemand::parser parser;
           simdjson::ondemand::document data = parser.iterate(json_content);
           simdjson::ondemand::array ranges = data["ranges"];

           // Step 4: Define map parameters
           const int map_height = 500;       // Map height in pixels
           const int map_width = 500;        // Map width in pixels
           const float pixels_per_meter = 100.0f;  // Scale: 100 pixels per meter
           const float Tx_robot = 0.0f;      // Robot's x-coordinate in world coordinates
           const float Ty_robot = 0.0f;      // Robot's y-coordinate in world coordinates
           const float R_robot = 0.0f;       // Robot's orientation in radians

           // Step 5: Generate the map
           create_map(ranges, map_height, map_width, pixels_per_meter, Tx_robot, Ty_robot, R_robot);

           std::cout << "Map generated successfully. Check the visualization window." << std::endl;

       } catch (const std::exception& e) {
           std::cerr << "Error: " << e.what() << std::endl;
           return 1;
       }

       return 0;
   }