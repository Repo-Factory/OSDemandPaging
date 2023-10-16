/*******************************************************************************
 * @author Strix Elixel
 * @name page_table
 * 
 * @brief Reads words from two files passed as cmd line arguments 
 * 
 * @description For each line in file2, it will output to stdout the numbers of substrings
 * 
 * Program must be called like this or will gracefully exit: ./program arg1 arg2
 * 
 * Relevant entities
 * - Dependencies
 *      - ProgressBar
 * - ArgsHandling       - For command line arguments
 * - InputHandling      - For file operations
 * 
 * See cpp file of each for more details in each respective area
 * 
 ******************************************************************************/

#include "page_table.h"
#include <stdio.h>

void testPageTable()
{
    constexpr const int LEVELS = 3;
    constexpr const int BITS = 8;
    PageTable<LEVELS, BITS> pageTable = createPageTable<LEVELS, BITS>(LEVELS, BITS);
    
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

    if (allTestsPassed) printf("All Page Table Tests Passed!\n");
}

// Test Page Table
int main(int argc, char * argv[])
{
    testPageTable();
    exit(EXIT_SUCCESS);
}
