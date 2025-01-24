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