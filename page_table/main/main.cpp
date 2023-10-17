/*******************************************************************************
 * @author Strix Elixel
 * @name page_table
 * 
 * @brief Templated structs used for defining page_tabel tree structure to be used in main program
 * 
 * This test code can be built separately from main executable using given CMakeLists.txt
 * 
 * Relevant entities
 *  - page_table
 *  - page_node
 *  - page_map
 *  - page_functions
 * 
 * See .h file of each for more details in each respective area (templated functions must be defined in header)
 * 
 ******************************************************************************/

#include "page_table.h"
#include "page_functions.h"
#include <stdio.h>

void testPageTable()
{
    constexpr const int LEVELS = 3;
    constexpr const int BITS = 8;
    PageTable pageTable = createPageTable(LEVELS, BITS);
    
    bool allTestsPassed = true;

    if (pageTable.bitMasks[0] != 0xFF000000)
    {
        printf("Bitmask test failed with value %x, expected %x\n", pageTable.bitMasks[0], 0xFF000000);
        allTestsPassed = false;
    }
    if (pageTable.bitMasks[1] != 0x00FF0000)
    {
        printf("Bitmask test failed with value %x, expected %x\n", pageTable.bitMasks[0], 0xFF000000);
        allTestsPassed = false;
    } 
    if (pageTable.bitMasks[2] != 0x0000FF00)
    {
        printf("Bitmask test failed with value %x, expected %x\n", pageTable.bitMasks[0], 0xFF000000);
        allTestsPassed = false;
    } 

    if (pageTable.bitShifts[0] != 24)
    {
        printf("BitShift test failed with value %d, expected %d\n", pageTable.bitShifts[0], 24);
        allTestsPassed = false;
    }
    if (pageTable.bitShifts[1] != 16)
    {
        printf("BitShift test failed with value %d, expected %d\n", pageTable.bitShifts[0], 16);
        allTestsPassed = false;
    } 
    if (pageTable.bitShifts[2] != 8)
    {
        printf("BitShift test failed with value %d, expected %d\n", pageTable.bitShifts[0], 8);
        allTestsPassed = false;
    }

    if (pageTable.entryCounts[0] != 256)
    {
        printf("EntryCount test failed with value %d, expected %d\n", pageTable.entryCounts[0], 256);
        allTestsPassed = false;
    }
    if (pageTable.entryCounts[1] != 256)
    {
        printf("EntryCount test failed with value %d, expected %d\n", pageTable.entryCounts[0], 256);
        allTestsPassed = false;
    } 
    if (pageTable.entryCounts[2] != 256)
    {
        printf("EntryCount test failed with value %d, expected %d\n", pageTable.entryCounts[0], 256);
        allTestsPassed = false;
    }

    if (pageTable.level_zero->nodeDepth != 0)
    {
        printf("EntryCount test failed with value %d, expected %d\n", pageTable.level_zero->nodeDepth, 0);
        allTestsPassed = false;
    }

    const Success status = insertVpn2PfnMapping(&pageTable, 0xFEFFFEC2, 3);
    const PageMap* valid_mapping = findVpn2PfnMapping(&pageTable, 0xFEFFFEC2);
    const PageMap* invalid_mapping = findVpn2PfnMapping(&pageTable, 0xDABEFEC1);

    if (status != 1)
    {
        printf("Mapping test failed with value %d, expected %d\n", status, 1);
        allTestsPassed = false;
    }

    if (valid_mapping->frame_number != 3)
    {
        printf("Mapping test failed with value %d, expected %d\n", valid_mapping->frame_number, 3);
        allTestsPassed = false;
    }

    if (invalid_mapping != nullptr)
    {
        printf("Mapping test failed with value %p, expected %p\n", invalid_mapping, nullptr);
        allTestsPassed = false;
    }
    
    if (allTestsPassed) printf("All Page Table Tests Passed!\n");
}

// Test Page Table
int main(int argc, char * argv[])
{
    testPageTable();
    exit(EXIT_SUCCESS);
}
