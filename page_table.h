#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <stdlib.h>
#include <vector>
#include <cstddef>
#include <vector>
#include <numeric>
#include "page_node.h"
#include "page_map.h"

constexpr const uint32_t INSTRUCTION_SIZE = 32;
#define LEVEL_ZERO 0
#define BIT 1
#define XONES(X) ((BIT << X) - 1) // Macro generates a stream of X ones by moving bit over and flipping all bits behind it
#define XZEROS(X) (XONES(32-X) << (X))

struct PageTable
{
    PageTable(const std::vector<uint32_t> treeLevels) : 
        treeDepth{(uint32_t)treeLevels.size()}, // Account for indexing (a tree with height 3 will have leaf nodes of depth 2)
        offsetBits{INSTRUCTION_SIZE - std::accumulate(treeLevels.begin(), treeLevels.end(), 0)},
        bitMasks(treeLevels.size()),    // Init Vectors to appropriate size
        bitShifts(treeLevels.size()), 
        entryCounts(treeLevels.size()) 
    {}
    const uint32_t treeDepth;
    const uint32_t offsetBits;
    PageNode* level_zero;
    std::vector<uint32_t> bitMasks;
    std::vector<uint32_t> bitShifts;
    std::vector<uint32_t> entryCounts;
};

std::vector<uint32_t> populateBitmasks(std::vector<uint32_t>& bitmasks, const std::vector<uint32_t> treeLevels);
std::vector<uint32_t> populateBitShifts(std::vector<uint32_t>& bitShifts, const std::vector<uint32_t> treeLevels);
std::vector<uint32_t> populateEntryCount(std::vector<uint32_t>& entryCounts, const std::vector<uint32_t> treeLevels);
PageTable createPageTable(const std::vector<uint32_t> treeLevels);

#endif
