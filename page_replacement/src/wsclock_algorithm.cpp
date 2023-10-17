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
    Entry& entry = circularList.entries[circularList.current_index++];
    entry.virtual_address = vpn;
    entry.last_access = circularList.elapsed_time++;
    entry.dirty = static_cast<bool>(access_type);
}

namespace
{
    bool isPageAvailable(const Ring& circularList, const uint32_t clockHand, const uint32_t threshold)
    {
        auto entry = circularList.entries[clockHand];
        bool stale = entry.last_access < (circularList.elapsed_time - threshold);
        bool clean = !entry.dirty;
        return stale && clean;
    }
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

void pageReplaceTree(PageTable& pageTable, const uint32_t vpn)
{
    PageMap* pageMap = findVpn2PfnMapping(&pageTable, vpn);
    if (pageMap) {
        insertVpn2PfnMapping(&pageTable, vpn, pageMap->frame_number);
        pageMap->valid = false;
    }
}
