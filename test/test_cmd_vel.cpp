#define CATCH_CONFIG_MAIN  // This tells Catch2 to provide a main() function
#include <catch2/catch_all.hpp>
#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../src/cmd_vel.cpp"