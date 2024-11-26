#define CATCH_CONFIG_MAIN  // This tells Catch2 to provide a main() function
#include <catch2/catch_all.hpp>
#include "../include/client/client.h"  // Adjust the path as needed to your Client class

TEST_CASE("Client establishes a connection", "[client]") {
    REQUIRE_NOTHROW(Client(9997));  // Check that creating a client connection does not throw an exception
}

TEST_CASE("Client fails to connect on invalid port", "[client]") {
    REQUIRE_THROWS_AS(Client(12345), std::runtime_error);  // Check that connecting to an invalid port throws a runtime error
}
//TODO: test for data receive
