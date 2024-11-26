# Add executable for the main application
add_executable(main main.cpp client.cpp cmd_vel.cpp)
# Include directories for headers
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)
find_package(OpenCV REQUIRED)
target_link_libraries(main PRIVATE ${OpenCV_LIBS})