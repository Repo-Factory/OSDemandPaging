#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <stdlib.h>
#include <array>
#include <cstddef>
#include "page_node.h"
#include "page_map.h"

#define INSTRUCTION_SIZE 32
#define LEVEL_ZERO 0

template<size_t Levels, int Bits>
struct PageTable
{
    int treeDepth;
    PageNode<Levels, Bits>* next_level;
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
        bitmask = ((1 << bitsPerLevel) - 1) << (current_bit-=bitsPerLevel);
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
        entryCount = 1 << bitsPerLevel;
    }
    return entryCounts;
}

template<size_t Levels, int Bits>
constexpr PageTable<Levels, Bits> createPageTable(const int treeDepth, const int bitsPerLevel)
{
    PageTable<Levels, Bits> pageTable;
    pageTable.treeDepth     = treeDepth;
    pageTable.next_level    = allocateNode<Levels, Bits>(pageTable, LEVEL_ZERO);
    pageTable.bitMasks      = populateBitmasks<Levels>(pageTable.bitMasks, bitsPerLevel);
    pageTable.bitShifts     = populateBitShifts<Levels>(pageTable.bitShifts, bitsPerLevel);
    pageTable.entryCounts   = populateEntryCount<Levels>(pageTable.entryCounts, bitsPerLevel);
    return                  pageTable;
}

#endif
