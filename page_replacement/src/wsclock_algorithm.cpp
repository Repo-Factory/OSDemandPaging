/* 
 * @brief wsclock_algorithm does something cool
 *
 * more
 * more
 * more
 */

#include "wsclock_algorithm.h"
#include "page_functions.h"
#include "page_map.h"

void updateAccessHistory(Ring& circularList, const uint32_t frame_number, const Mode access_type)
{
    circularList.entries[frame_number].last_access = circularList.elapsed_time++;
    circularList.entries[frame_number].dirty = static_cast<bool>(access_type);
}

void addPageToList(Ring& circularList, const uint32_t vpn)
{ 
    circularList.entries[circularList.current_index++].virtual_address = vpn; // % circularList.capacity
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
    static int clockHand = 0;       // Icky static but it's very self-contained
    do {
        if (isPageAvailable(circularList, clockHand, threshold)) break;
        circularList.entries[clockHand].dirty = false;           // Simulate scheduling
        clockHand = (clockHand + 1) % circularList.capacity;
    } while (true);
    return clockHand++;
}

void pageReplaceTree(PageTable& pageTable, const uint32_t vpnToReplace, const uint32_t replacementVpn)
{
    PageMap* pageMap = findVpn2PfnMapping(&pageTable, vpnToReplace);
    if (pageMap) {
        insertVpn2PfnMapping(&pageTable, replacementVpn, pageMap->frame_number);
        pageMap->valid = false;
    }
}

// Returns vpn of replaced Page
uint32_t replacePage(PageTable& pageTable, Ring& circularList, const uint32_t virtualAddress, const uint32_t threshold)
{
    const PageIndex victimIndex = findAvailablePage(circularList, threshold);
    const uint32_t vpnOfPage = circularList.entries[victimIndex].virtual_address;
    circularList.entries[victimIndex].virtual_address = virtualAddress;
    pageReplaceTree(pageTable, vpnOfPage, virtualAddress);
    return vpnOfPage;
}
