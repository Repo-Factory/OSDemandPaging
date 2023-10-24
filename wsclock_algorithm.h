#ifndef WSCLOCK_ALGORITHM_H
#define WSCLOCK_ALGORITHM_H

#include <stdlib.h>
#include <vector>
#include <cstddef>
#include <chrono>
#include "page_table.h"

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
    Ring(const int capacity, const int threshold)
        : entries(capacity),
        capacity{capacity},
        threshold{threshold}
    {}
    int current_index = 0;
    std::time_t elapsed_time = 0;
    const int threshold;
    const int capacity;
    std::vector<Entry> entries;
};

using PageIndex = uint32_t;

void updateAccessHistory(Ring& circularList, const uint32_t frame_number, const Mode access_type);
void addPageToList(Ring& circularList, const uint32_t vpn, const uint32_t frame);
uint32_t findAvailablePage(Ring& circularList, const uint32_t threshold);
void pageReplaceClock(Ring& circularList, const PageIndex index, const uint32_t vpn);
void pageReplaceTree(PageTable& pageTable, const uint32_t vpnOfPage, const uint32_t vpnToReplace, const uint32_t frame);
uint32_t replacePage(PageTable& pageTable, Ring& circularList, const uint32_t virtualAddress, const uint32_t threshold);

#endif
