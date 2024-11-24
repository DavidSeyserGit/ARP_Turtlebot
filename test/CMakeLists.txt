include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0  # Replace with the latest version
)
FetchContent_MakeAvailable(Catch2)

# Add the test executable
add_executable(tests test_client.cpp ../src/client.cpp)

# Include directories
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link Catch2
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
# Add CTest integration
include(CTest)
include(Catch)
catch_discover_tests(tests)