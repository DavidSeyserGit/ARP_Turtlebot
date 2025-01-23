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