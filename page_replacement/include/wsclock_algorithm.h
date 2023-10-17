#ifndef WSCLOCK_ALGORITHM_H
#define WSCLOCK_ALGORITHM_H

#include <cstddef>
#include "circular_list.h"
#include "page_functions.h"
#include "page_map.h"

using PageIndex = int;

template<size_t Capacity>
void updatePageList(Ring<Capacity>& circularList, const unsigned int vpn, const Mode access_type)
{
    Entry& entry = circularList.entries[circularList.current_index++];
    entry.virtual_address = vpn;
    entry.last_access = circularList.elapsed_time++;
    entry.dirty = static_cast<bool>(access_type);
}

template<size_t Capacity>
void pageReplaceClock(Ring<Capacity>& circularList, const PageIndex index, const unsigned int vpn)
{
    circularList.entries[index].virtual_address = vpn;
    circularList.entries[index].dirty = true;
    circularList.entries[index].elapsed_time = circularList.elapsed_time;
}

template<size_t Levels, int Bits>
void pageReplaceTree(PageTable<Levels, Bits>& pageTable, const unsigned int vpn)
{
    PageMap* pageMap = findVpn2PfnMapping<Levels, Bits>(pageTable, vpn);
    if (pageMap) {
        insertVpn2PfnMapping(pageTable, vpn, pageMap->frame_number);
        pageMap->valid = false;
    }
}

template<size_t Capacity>
int findPage(Ring<Capacity>& circularList, const int threshold)
{
    static int clockHand = 0;

    do {
        auto entry = circularList.entries[clockHand];
        bool stale = entry.last_access < (circularList.elapsed_time - threshold);
        bool clean = !entry.dirty; 
        if (stale && clean) break;  
    
        circularList.entries[clockHand].dirty = false;
        clockHand = (clockHand + 1) % circularList.capacity;
    } while (true);

    return clockHand++;
}

#endif
