#ifndef ODOMETRY_H_
#define ODOMETRY_H_

#include "client/client.h" // Ensure Client is defined or forward-declared

struct OdomData {
    float x;
    float y;
    float theta;
} __attribute__((aligned(4))); // Enforces 4-byte alignment


OdomData* InitializeSharedMemory(int& shm_fd);
void ProcessAndStoreOdometryData(Client& client, OdomData* odom_data);

#endif // ODOMETRY_H_