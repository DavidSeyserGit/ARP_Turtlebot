# Turtlebot3 Control System in C++

This project provides a lightweight C++ implementation for controlling a Turtlebot3 robot. It focuses on real-time control and data acquisition, enabling basic navigation and environmental mapping.

[![Code Style: Google](https://img.shields.io/badge/code%20style-google-blue.svg)](https://google.github.io/styleguide/cppguide.html) 
[![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.svg?v=103)](https://github.com/DavidSeyserGit/ARP_Turtlebot) 
[![Platform](https://img.shields.io/badge/platform-Linux%20%2F%20macOS-green.svg)](#)

## Features

* **Velocity Control:** Send velocity commands to the Turtlebot3 over TCP to control its movement (linear and angular velocities).
* **Sensor Data Acquisition:** Receive real-time IMU (Inertial Measurement Unit) data from the robot via TCP.
* **Lidar-based Mapping:**  Utilize lidar data to create a map of the robot's surrounding environment. 

## Getting Started
### Prerequisites

* CMake

### Build Instructions

1. Clone this repository: `git clone https://github.com/DavidSeyserGit/ARP_Turtlebot.git`
2. Navigate to the project directory: `cd ARP_Turtlebot`
3. `make`
