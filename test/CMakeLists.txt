include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0  # Replace with the latest version
)
FetchContent_MakeAvailable(Catch2)

# Add the test executable
add_executable(test_client test_client.cpp ../src/client.cpp)
add_executable(test_laser test_laser.cpp ../src/client.cpp)
add_executable(test_imu test_imu.cpp ../src/client.cpp) 

# Include directories
target_include_directories(test_client PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(test_laser PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(test_imu PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link Catch2
target_link_libraries(test_client PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_laser PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_imu PRIVATE Catch2::Catch2WithMain)

# Add CTest integration
include(CTest)
include(Catch)
catch_discover_tests(test_client test_laser test_imu)