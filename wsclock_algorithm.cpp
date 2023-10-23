/* 
 * @brief wsclock_algorithm implementation for page replacement
 *
 * We will keep track of our frames allocated in a circular list. We will use this list to free a page when we encounter a page
 * fault 
 */

#include "wsclock_algorithm.h"
#include "page_functions.h"
#include <iostream>

void updateAccessHistory(Ring& circularList, const uint32_t frame, const Mode access_type)
{
    circularList.entries[frame].last_access = circularList.elapsed_time++;
    if (access_type == Mode::Write) circularList.entries[frame].dirty = true;
}

void addPageToList(Ring& circularList, const uint32_t vpn, const uint32_t frame)
{ 
    circularList.entries[frame].virtual_address = vpn; // % circularList.capacity
}

bool isPageAvailable(const Ring& circularList, const uint32_t clockHand, const uint32_t threshold)
{
    const auto entry = circularList.entries[clockHand];
    const bool stale = entry.last_access < (circularList.elapsed_time - threshold);
    const bool clean = !entry.dirty;
    return stale && clean;
}    

uint32_t findAvailablePage(Ring& circularList, const uint32_t threshold)
{
    static int clockHand = 0;       // Icky static but it's self-contained to this function
    do {
        if (isPageAvailable(circularList, clockHand, threshold)) break;
        circularList.entries[clockHand].dirty = false;           // Simulate scheduling
        clockHand = (clockHand + 1) % circularList.capacity;
    } while (true);
    const int currentClock = clockHand;
    clockHand = (clockHand + 1) % circularList.capacity; 
    return currentClock;
}

void pageReplaceTree(PageTable& pageTable, const uint32_t vpnToReplace, const uint32_t replacementVpn, const uint32_t frame)
{
    insertVpn2PfnMapping(&pageTable, replacementVpn, frame);
    findVpn2PfnMapping(&pageTable, vpnToReplace)->valid = false;
}

// Returns vpn of replaced Page
uint32_t replacePage(PageTable& pageTable, Ring& circularList, const uint32_t virtualAddress, const uint32_t threshold)
{
    const PageIndex victimIndex = findAvailablePage(circularList, threshold);
    const uint32_t vpnOfPage = circularList.entries[victimIndex].virtual_address;
    circularList.entries[victimIndex].virtual_address = virtualAddress;
    pageReplaceTree(pageTable, vpnOfPage, virtualAddress, victimIndex);
    return vpnOfPage;
}

