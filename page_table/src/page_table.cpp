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
const unsigned int getVPNFromVirtualAddress(const unsigned int virtualAddress, const unsigned int mask, const unsigned int shift)
{
    return (virtualAddress & mask) >> shift;
}

std::vector<int> populateBitmasks(std::vector<int>& bitmasks, const int bitsPerLevel)
{
    int current_bit = INSTRUCTION_SIZE;
    for (int& bitmask : bitmasks)
    {
        bitmask = XONES(bitsPerLevel) << (current_bit-=bitsPerLevel);
    }
    return bitmasks;
}

std::vector<int> populateBitShifts(std::vector<int>& bitShifts, const int bitsPerLevel)
{
    int current_bit = INSTRUCTION_SIZE;
    for (int& bitShift : bitShifts)
    {
        bitShift = current_bit-=bitsPerLevel;
    }
    return bitShifts;
}

std::vector<int> populateEntryCount(std::vector<int>& entryCounts, const int bitsPerLevel)
{
    int current_bit = INSTRUCTION_SIZE;
    for (int& entryCount : entryCounts)
    {
        entryCount = TWO_TO_POWER_OF(bitsPerLevel);
    }
    return entryCounts;
}

PageTable createPageTable(const int treeDepth, const int bitsPerLevel)
{
    PageTable pageTable(treeDepth);
    pageTable.level_zero            = allocateNode(pageTable, LEVEL_ZERO, bitsPerLevel);
    pageTable.bitMasks              = populateBitmasks(pageTable.bitMasks, bitsPerLevel);
    pageTable.bitShifts             = populateBitShifts(pageTable.bitShifts, bitsPerLevel);
    pageTable.entryCounts           = populateEntryCount(pageTable.entryCounts, bitsPerLevel);
    return                          pageTable;
}

