#ifndef MAIN_H
#define MAIN_H

#include "args_handling.h"
#include "page_table.h"
#include "wsclock_algorithm.h"
#include <functional>
#include <iostream>

uint32_t forEachAddress(const Args& args, uint32_t& frame, std::function<void(const uint32_t, const uint32_t)> performOperations);
uint32_t addFrameAndOffset(const uint32_t frame, const uint32_t offset, const uint32_t offsetBits);
int replacePageIfNecessary(PageTable& pageTable, Ring& circularList, const unsigned int virtualAddress, const int threshold);
void exitIfBitMaskFlag(const Args& args, PageTable& pageTable);

#endif 