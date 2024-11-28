#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <cstddef>   // For size_t
#include <string>    // For std::string

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#endif

/**
 * @file shared_memory.h
 * @brief A lightweight cross platform wrapper for shared_memory
 * 
 * This library is design to be lightweight and cross platform
 * it will hand the creation of shared memory space for unix
 * and windows systems
 */

// Create or open a shared memory region
bool CreateSharedMemory(const std::string& name, size_t size, void*& memory);

// Attach to an existing shared memory region
bool AttachSharedMemory(const std::string& name, size_t size, void*& memory);

// Detach the shared memory region
void DetachSharedMemory(void* memory, size_t size);

// Destroy a shared memory region
void DestroySharedMemory(const std::string& name);

#endif // SHARED_MEMORY_H