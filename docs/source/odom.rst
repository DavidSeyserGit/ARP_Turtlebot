odometry Module
===============

The `odometry` module provides functionality for reading odometry data and saving the processed data into shared memory for further use.

Functions
---------

InitializeSharedMemory
----------------------
Initializes a shared memory region for storing odometry data.

.. cpp:function:: OdomData* InitializeSharedMemory(int& shm_fd)

   Initializes a shared memory object for odometry data storage. If the shared memory already exists, it is cleaned up and recreated.

   :param shm_fd: A reference to the shared memory file descriptor.
   :type shm_fd: int&
   :return: A pointer to the initialized shared memory region, cast to an `OdomData` object.
   :return type: OdomData*

ProcessAndStoreOdometryData
---------------------------
Processes odometry data received from a client and stores it in shared memory.

.. cpp:function:: void ProcessAndStoreOdometryData(Client& client, OdomData* odom_data)

   Continuously receives odometry data from the client, extracts the relevant information (x, y, and orientation), and updates the shared memory.

   :param client: A reference to the client object used to receive odometry data.
   :type client: Client&
   :param odom_data: A pointer to the shared memory region for storing odometry data.
   :type odom_data: OdomData*
   :return: None

Examples
--------

Below is an example of how to use the `odometry` module to initialize shared memory and process incoming odometry data.

.. code-block:: cpp

   #include "odom.h"
   #include <iostream>

   int main() {
       try {
           // Shared memory setup
           int shm_fd;
           OdomData* odom_data = InitializeSharedMemory(shm_fd);

           // Client setup
           Client odomClient(9998);

           // Process and store odometry data
           ProcessAndStoreOdometryData(odomClient, odom_data);

           // Cleanup shared resources
           munmap(odom_data, sizeof(OdomData));
           close(shm_fd);
           shm_unlink(kSharedMemoryName);
       } catch (const std::exception& e) {
           std::cerr << "Error: " << e.what() << std::endl;
           return 1;
       }
   }