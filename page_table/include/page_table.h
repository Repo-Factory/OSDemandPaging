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
    PageTable(const int treeDepth) : 
        treeDepth{treeDepth-1}, // Account for indexing (a tree with height 3 will have leaf nodes of depth 2)
        bitMasks(treeDepth),    // Init Vectors to appropriate size
        bitShifts(treeDepth), 
        entryCounts(treeDepth) 
    {}
    const int treeDepth;
    PageNode* level_zero;
    std::vector<int> bitMasks;
    std::vector<int> bitShifts;
    std::vector<int> entryCounts;
};

std::vector<int> populateBitmasks(std::vector<int>& bitmasks, const int bitsPerLevel);
std::vector<int> populateBitShifts(std::vector<int>& bitShifts, const int bitsPerLevel);
std::vector<int> populateEntryCount(std::vector<int>& entryCounts, const int bitsPerLevel);
PageTable createPageTable(const int treeDepth, const int bitsPerLevel);

#endif
