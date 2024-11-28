# Add executable for the main application
#later the other files need to be added here too -> main will be the visualisation file.
add_executable(main main.cpp client.cpp cmd_vel.cpp) #add laserscaner and imu data

# Include directories for headers
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)
find_package(OpenCV REQUIRED)
target_link_libraries(main PRIVATE ${OpenCV_LIBS})