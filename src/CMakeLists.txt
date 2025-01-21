# Add executable for the main application

#later the other files need to be added here too -> main will be the visualisation file.
add_executable(main main.cpp client.cpp cmd_vel.cpp laser_data.cpp simdjson.cpp) #add laserscaner and imu data

find_package (OpenCV REQUIRED)
add_executable(cmd_vel client.cpp cmd_vel.cpp) #add laserscaner and imu data
add_executable(odom odometry.cpp client.cpp)

# Include directories for headers
target_include_directories(cmd_vel PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(odom PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Conditionally link libraries based on the platform
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # On Linux, link with rt for POSIX shared memory
    target_link_libraries(odom rt)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # On macOS, no additional linking is needed for shared memory
    message(STATUS "Building on macOS - librt not required")
endif()