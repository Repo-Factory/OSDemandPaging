#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H

#include <stdlib.h>
#include <array>
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

template<size_t Capacity>
struct Ring
{
    int current_index = 0;
    std::time_t elapsed_time = 0;
    size_t capacity = Capacity;
    std::array<Entry, Capacity> entries;
};

#endif
