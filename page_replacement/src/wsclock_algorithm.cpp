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

void updatePageList(Ring& circularList, const uint32_t vpn, const Mode access_type)
{
    Entry& entry = circularList.entries[circularList.current_index++ % circularList.capacity];
    entry.virtual_address = vpn;
    entry.last_access = circularList.elapsed_time++;
    entry.dirty = static_cast<bool>(access_type);
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

void pageReplaceClock(Ring& circularList, const PageIndex index, const uint32_t vpn)
{
    circularList.entries[index].virtual_address = vpn;
    circularList.entries[index].dirty = true;
    circularList.entries[index].last_access = circularList.elapsed_time;
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
    pageReplaceClock(circularList, victimIndex, virtualAddress);
    pageReplaceTree(pageTable, vpnOfPage, virtualAddress);
    return vpnOfPage;
}
