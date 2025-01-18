#include "laser_data/laser_data.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include "simdjson/simdjson.h"

using namespace simdjson;

std::string extract_json(const std::string& text) {
    size_t start = text.find("---START---");
    size_t end = text.find("---END---");

    if (start == std::string::npos || end == std::string::npos) {
        throw std::runtime_error("Invalid format: Missing ---START--- or ---END---");
    }

    start += std::string("---START---").length();
    return text.substr(start, end - start);
}


// int main(void) {
//     // Read the entire file content
//     std::ifstream file("laserdata.txt");
//     std::stringstream buffer;
//     buffer << file.rdbuf();
//     std::string file_content = buffer.str();

//     // Extract JSON content
//     std::string json_content = extract_json(file_content);

//     // Parse the JSON content
//     ondemand::parser parser;
//     ondemand::document data = parser.iterate(json_content);

//     // Access ranges array
//     ondemand::array ranges = data["ranges"];

//     std::cout << "Ranges: " << std::endl;

//     size_t count = 0;
//     for (auto dist : ranges) {
//         std::cout << double(dist) << std::endl;
//         count++;
//     }

//     std::cout << "Number of ranges: " << count << std::endl;

//     return 0;
// }