/* 
 * @brief page_table is a templated class
 *
 * See page_table.h for more details
 */

#include "page_table.h"

/*
 * Given a virtual address, apply the given bit mask and shift right by the given number of
 * bits. Returns the virtual page number. This function can be used to extract the VPN of
 * any level or the full VPN by supplying the appropriate parameters.
 */
const uint32_t getVPNFromVirtualAddress(const uint32_t virtualAddress, const uint32_t mask, const uint32_t shift)
{
    return (virtualAddress & mask) >> shift;
}

std::vector<uint32_t> populateBitmasks(std::vector<uint32_t>& bitmasks, const uint32_t bitsPerLevel)
{
    uint32_t current_bit = INSTRUCTION_SIZE;
    for (uint32_t& bitmask : bitmasks)
    {
        bitmask = XONES(bitsPerLevel) << (current_bit-=bitsPerLevel);
    }
    return bitmasks;
}

std::vector<uint32_t> populateBitShifts(std::vector<uint32_t>& bitShifts, const uint32_t bitsPerLevel)
{
    uint32_t current_bit = INSTRUCTION_SIZE;
    for (uint32_t& bitShift : bitShifts)
    {
        bitShift = current_bit-=bitsPerLevel;
    }
    return bitShifts;
}

std::vector<uint32_t> populateEntryCount(std::vector<uint32_t>& entryCounts, const uint32_t bitsPerLevel)
{
    uint32_t current_bit = INSTRUCTION_SIZE;
    for (uint32_t& entryCount : entryCounts)
    {
        entryCount = TWO_TO_POWER_OF(bitsPerLevel);
    }
    return entryCounts;
}

PageTable createPageTable(const uint32_t treeDepth, const uint32_t bitsPerLevel)
{
    PageTable pageTable(treeDepth);
    pageTable.level_zero            = allocateNode(pageTable, LEVEL_ZERO, bitsPerLevel);
    pageTable.bitMasks              = populateBitmasks(pageTable.bitMasks, bitsPerLevel);
    pageTable.bitShifts             = populateBitShifts(pageTable.bitShifts, bitsPerLevel);
    pageTable.entryCounts           = populateEntryCount(pageTable.entryCounts, bitsPerLevel);
    return                          pageTable;
}

