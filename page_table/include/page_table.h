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

unsigned int getVPNFromVirtualAddress(unsigned int virtualAddress, unsigned int mask, unsigned int shift);

template<size_t Levels, int Bits>
void assignVPNToFrame(PageNode<Levels, Bits>* pageLevel, const unsigned int jumpIndex, int frame)
{
    auto leafNode = (LeafNode<Levels, Bits>*)pageLevel;
    leafNode->pageMaps[jumpIndex] = new PageMap;
    leafNode->pageMaps[jumpIndex]->frame_number = frame;
}

template<size_t Levels, int Bits>
InternalNode<Levels, Bits>* allocateNextLevel(PageNode<Levels, Bits>* pageLevel, const unsigned int jumpIndex)
{
    auto internalNode = (InternalNode<Levels, Bits>*)pageLevel;
    internalNode->childNodes[jumpIndex] = new InternalNode(pageLevel->pageTable);
    internalNode->childNodes[jumpIndex]->nodeDepth = pageLevel->nodeDepth+1;
    return internalNode;
}

template<size_t Levels, int Bits>
void insertVpn2PfnMapping(PageNode<Levels, Bits>* pageLevel, unsigned int vpn, int frame)
{
    const unsigned int jumpIndex = vpn & pageLevel->pageTable.bitMasks[pageLevel->nodeDepth];
    if (pageLevel->nodeDepth == pageLevel->pageTable.treeDepth)
    {
        assignVPNToFrame(pageLevel, jumpIndex, frame);
    }
    auto internalNode = allocateNextLevel(pageLevel, jumpIndex);
    insertVpn2PfnMapping(internalNode, vpn, frame);
}

template<size_t Levels, int Bits>
void insertVpn2PfnMapping(PageTable<Levels, Bits>* pageTable, unsigned int vpn, int frame)
{
    insertVpn2PfnMapping(pageTable->next_level, vpn, frame);
}

#endif
