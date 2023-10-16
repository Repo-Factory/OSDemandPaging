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
    int levelCount;
    PageNode* level_zero;
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
constexpr PageTable<Levels, Bits> createPageTable(const int levelCount, const int bitsPerLevel)
{
    PageTable<Levels, Bits> pageTable;
    pageTable.levelCount    = levelCount;
    pageTable.level_zero    = allocateNode<Bits>(LEVEL_ZERO, Levels);
    pageTable.bitMasks      = populateBitmasks<Levels>(pageTable.bitMasks, bitsPerLevel);
    pageTable.bitShifts     = populateBitShifts<Levels>(pageTable.bitShifts, bitsPerLevel);
    pageTable.entryCounts   = populateEntryCount<Levels>(pageTable.entryCounts, bitsPerLevel);
    return                  pageTable;
}


/*
 *
 * Given a virtual address, apply the given bit mask and shift right by the given number of
 * bits. Returns the virtual page number. This function can be used to extract the VPN of
 * any level or the full VPN by supplying the appropriate parameters.
 * Example: With a 32-bit system, suppose the level 1 pages occupied bits 22 through 27,
 * and we wish to extract the level 1 page number of address 0x3c654321.
 * Mask is 0b00001111110000000000, shift is 22. The invocation would be
 * getVPNFromVirtualAddress(0x3c654321, 0x0FC00000, 22) which should
 * return 0x31 (decimal 49).
 * First take the bitwise ‘and’ operation between 0x3c654321 and 0x0FC00000,
 * which is 0x0C400000, then shift right by 22 bits. The last five hexadecimal zeros
 * take up 20 bits, and the bits higher than this are 1100 0110 (C6). We shift by
 * two more bits to have the 22 bits, leaving us with 11 0001, or 0x31.
 * Check out the given bitmasking-demo.c for an example of bit masking and
 * shifting for extracting bits in a hexadecimal number.
 * Note: to get the full Virtual Page Number (VPN) from all page levels, you would
 * construct the bit mask for all bits preceding the offset bits, take the bitwise and of the
 * virtual address and the mask, then shift right for the number of offset bits.
 * Full VPN (all levels combined) is needed for page replacement, see below.
 *  
 */
// unsigned int getVPNFromVirtualAddress(unsigned int virtualAddress, unsigned int mask, unsigned int shift);

/* 
 *
 * Used to add new entries to or update the page table for the VPN to PFN mapping:
 * when the virtual page (VPN corresponding to the passed-in virtual address) has
 * not yet been allocated with a frame (findVpn2PfnMapping returns NULL or
 * invalid).
 * when trying to update mapping of the VPN to a particular PFN.
 * frame (or PFN) is the frame index that is mapped to the VPN.
 * important: you could use a -1 value for the frame argument for
 * updating the mapping to be invalid. In page replacement, you would
 * need to update the replaced (evicted) VPN to PFN mapping to be
 * invalid.
 * if you wish, you may replace void with int or bool and return an error code if
 * unable to allocate memory for the page table.
 * HINT: If you are inserting a page, you do not always add nodes at every level.
 * Part of the VPN may already exist at some or all the levels.
 * 
 */

// template<std::size_t Levels>
// void insertVpn2PfnMapping(PageTable<Levels>* pagetable, unsigned int vpn, int frame);

#endif
