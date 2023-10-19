#ifndef WSCLOCK_ALGORITHM_H
#define WSCLOCK_ALGORITHM_H

#include <cstddef>
#include "circular_list.h"
#include "page_table.h"

using PageIndex = uint32_t;

void updatePageList(Ring& circularList, const uint32_t vpn, const Mode access_type);
uint32_t findAvailablePage(Ring& circularList, const uint32_t threshold);
void pageReplaceClock(Ring& circularList, const PageIndex index, const uint32_t vpn);
void pageReplaceTree(PageTable& pageTable, const uint32_t vpnOfPage, const uint32_t vpnToReplace);
uint32_t replacePage(PageTable& pageTable, Ring& circularList, const uint32_t virtualAddress, const uint32_t threshold);

#endif
