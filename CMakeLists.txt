cmake_minimum_required(VERSION 3.14)
project(ARP_Turtlebot)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories for source and tests
add_subdirectory(src)
add_subdirectory(test)