/* 
 * @brief page_table is a templated class
 *
 * See page_table.h for more details
 */

#include "page_table.h"
#include <vector>
#include <numeric>

/*
 * Given a virtual address, apply the given bit mask and shift right by the given number of
 * bits. Returns the virtual page number. This function can be used to extract the VPN of
 * any level or the full VPN by supplying the appropriate parameters.
 */
const uint32_t getVPNFromVirtualAddress(const uint32_t virtualAddress, const uint32_t mask, const uint32_t shift)
{
    return (virtualAddress & mask) >> shift;
}

std::vector<uint32_t> populateBitmasks(std::vector<uint32_t>& bitmasks, const std::vector<uint32_t> treeLevels)
{
    uint32_t current_bit = INSTRUCTION_SIZE;
    for (int i = 0; i < bitmasks.size(); i++)
    {
        bitmasks[i] = XONES(treeLevels[i]) << (current_bit-=treeLevels[i]);
    }
    return bitmasks;
}

std::vector<uint32_t> populateBitShifts(std::vector<uint32_t>& bitShifts, const std::vector<uint32_t> treeLevels)
{
    uint32_t current_bit = INSTRUCTION_SIZE;
    for (int i = 0; i < bitShifts.size(); i++)
    {
        bitShifts[i] = current_bit-=treeLevels[i];
    }
    return bitShifts;
}

std::vector<uint32_t> populateEntryCount(std::vector<uint32_t>& entryCounts, const std::vector<uint32_t> treeLevels)
{
    for (int i = 0; i < entryCounts.size(); i++)
    {
        entryCounts[i] = TWO_TO_POWER_OF(treeLevels[i]);
    }
    return entryCounts;
}

PageTable createPageTable(const std::vector<uint32_t> treeLevels)
{
    PageTable pageTable             (treeLevels);
    pageTable.bitMasks              = populateBitmasks(pageTable.bitMasks, treeLevels);
    pageTable.bitShifts             = populateBitShifts(pageTable.bitShifts, treeLevels);
    pageTable.entryCounts           = populateEntryCount(pageTable.entryCounts, treeLevels);
    pageTable.level_zero            = allocateNode(pageTable, LEVEL_ZERO);
    return                          pageTable;
}
