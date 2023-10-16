#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <stdlib.h>
#include <array>
#include <cstddef>
#include "page_node.h"
#include "page_map.h"

#define INSTRUCTION_SIZE 32
#define LEVEL_ZERO 0
#define BIT 1
#define XONES(X) ((BIT << X) - 1) // Macro generates a stream of X ones by moving bit over and flipping all bits behind it

template<size_t Levels, int Bits>
struct PageTable
{
    int treeDepth;
    PageNode<Levels, Bits>* level_zero;
    std::array<int, Levels> bitMasks;
    std::array<int, Levels> bitShifts;
    std::array<int, Levels> entryCounts;
};

template<size_t Levels>
constexpr std::array<int, Levels> populateBitmasks(std::array<int, Levels>& bitmasks, const int bitsPerLevel)
{
    int current_bit = INSTRUCTION_SIZE;
    for (int& bitmask : bitmasks)
    {
        bitmask = XONES(bitsPerLevel) << (current_bit-=bitsPerLevel);
    }
    return bitmasks;
}

template<size_t Levels>
constexpr std::array<int, Levels> populateBitShifts(std::array<int, Levels>& bitShifts, const int bitsPerLevel)
{
    int current_bit = INSTRUCTION_SIZE;
    for (int& bitShift : bitShifts)
    {
        bitShift = current_bit-=bitsPerLevel;
    }
    return bitShifts;
}

template<size_t Levels>
constexpr std::array<int, Levels> populateEntryCount(std::array<int, Levels>& entryCounts, const int bitsPerLevel)
{
    int current_bit = INSTRUCTION_SIZE;
    for (int& entryCount : entryCounts)
    {
        entryCount = TWO_TO_POWER_OF(bitsPerLevel);
    }
    return entryCounts;
}

template<size_t Levels, int Bits>
constexpr PageTable<Levels, Bits> createPageTable(const int treeDepth, const int bitsPerLevel)
{
    PageTable<Levels, Bits> pageTable;
    pageTable.treeDepth     = treeDepth - 1;    // Account for indexing difference (tree with 3 levels will have leaves of depth 2)
    pageTable.level_zero    = allocateNode<Levels, Bits>(pageTable, LEVEL_ZERO);
    pageTable.bitMasks      = populateBitmasks<Levels>(pageTable.bitMasks, bitsPerLevel);
    pageTable.bitShifts     = populateBitShifts<Levels>(pageTable.bitShifts, bitsPerLevel);
    pageTable.entryCounts   = populateEntryCount<Levels>(pageTable.entryCounts, bitsPerLevel);
    return                  pageTable;
}

#endif
