#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H

#include <stdlib.h>
#include <vector>
#include <cstddef>
#include <chrono>

enum class Mode 
{
    Read  = 0,
    Write = 1
};

struct Entry
{
    int32_t virtual_address;
    std::time_t last_access;
    bool dirty = false;
};

struct Ring
{
    Ring(const int capacity)
        : entries(capacity),
        capacity{capacity} 
    {}
    int current_index = 0;
    std::time_t elapsed_time = 0;
    int capacity;
    std::vector<Entry> entries;
};

#endif
