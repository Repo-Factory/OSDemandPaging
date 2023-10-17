#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <stdlib.h>
#include <vector>
#include <cstddef>
#include "page_node.h"
#include "page_map.h"

#define INSTRUCTION_SIZE 32
#define LEVEL_ZERO 0
#define BIT 1
#define XONES(X) ((BIT << X) - 1) // Macro generates a stream of X ones by moving bit over and flipping all bits behind it

struct PageTable
{
    PageTable(const uint32_t treeDepth) : 
        treeDepth{treeDepth-1}, // Account for indexing (a tree with height 3 will have leaf nodes of depth 2)
        bitMasks(treeDepth),    // Init Vectors to appropriate size
        bitShifts(treeDepth), 
        entryCounts(treeDepth) 
    {}
    const uint32_t treeDepth;
    PageNode* level_zero;
    std::vector<uint32_t> bitMasks;
    std::vector<uint32_t> bitShifts;
    std::vector<uint32_t> entryCounts;
};

std::vector<uint32_t> populateBitmasks(std::vector<uint32_t>& bitmasks, const uint32_t bitsPerLevel);
std::vector<uint32_t> populateBitShifts(std::vector<uint32_t>& bitShifts, const uint32_t bitsPerLevel);
std::vector<uint32_t> populateEntryCount(std::vector<uint32_t>& entryCounts, const uint32_t bitsPerLevel);
PageTable createPageTable(const uint32_t treeDepth, const uint32_t bitsPerLevel);

#endif
