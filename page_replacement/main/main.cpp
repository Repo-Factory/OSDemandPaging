/*******************************************************************************
 * @author Strix Elixel
 * @name page_replacement
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

#include <stdio.h>
#include <stdlib.h>
#include "wsclock_algorithm.h"

void testCircularList()
{
    constexpr const size_t CAPACITY = TWO_TO_POWER_OF(16);
    Ring<CAPACITY> circularList;
    bool allTestsPassed = true;

    updatePageList(circularList, 0xFFEEDDCC, Mode::Write);

    if (circularList.entries[0].virtual_address != 0xFFEEDDCC)
    {
        printf("virtual_address test failed with value %d, expected %d\n", circularList.entries[0].virtual_address, 0xFFEEDDCC);
        allTestsPassed = false;
    }

    if (circularList.entries[0].last_access != 0)
    {
        printf("last_access test failed with value %ld, expected %d\n", circularList.entries[0].last_access, 0);
        allTestsPassed = false;
    }

    if (circularList.entries[0].dirty != 1)
    {
        printf("dirty test failed with value %d, expected %d\n", circularList.entries[0].dirty, 1);
        allTestsPassed = false;
    }
    
    updatePageList(circularList, 0xAABBCCDD, Mode::Write);

    if (circularList.entries[1].virtual_address != 0xAABBCCDD)
    {
        printf("virtual_address test failed with value %d, expected %d\n", circularList.entries[1].virtual_address, 0xAABBCCDD);
        allTestsPassed = false;
    }

    if (circularList.entries[1].last_access != 1)
    {
        printf("last_access test failed with value %ld, expected %d\n", circularList.entries[1].last_access, 1);
        allTestsPassed = false;
    }

    if (circularList.entries[1].dirty != 1)
    {
        printf("dirty test failed with value %d, expected %d\n", circularList.entries[1].dirty, 1);
        allTestsPassed = false;
    }

    if (allTestsPassed) printf("Update Page List Tests Passed!\n");
}

void testClockFindPage()
{
    constexpr const size_t CAPACITY = TWO_TO_POWER_OF(16);
    constexpr const int THRESHOLD   = 3;
    Ring<CAPACITY> circularList;
    bool allTestsPassed = true;

    updatePageList(circularList, 0x00000000, Mode::Write);
    updatePageList(circularList, 0x00000001, Mode::Write);
    updatePageList(circularList, 0x00000002, Mode::Write);
    updatePageList(circularList, 0x00000003, Mode::Write);
    updatePageList(circularList, 0x00000004, Mode::Write);
    updatePageList(circularList, 0x00000005, Mode::Write);

    int pageIndex = findPage(circularList, THRESHOLD);

    if (pageIndex != 6)
    {
        printf("find page test failed with value %d, expected %d\n", pageIndex, 0);
        allTestsPassed = false;
    }

    pageIndex = findPage(circularList, THRESHOLD);
    if (pageIndex != 7)
    {
        printf("find page test failed with value %d, expected %d\n", pageIndex, 1);
        allTestsPassed = false;
    }

    if (allTestsPassed) printf("Page Find Tests Passed!\n");
}

int main(int argc, char * argv[])
{
    testCircularList();
    testClockFindPage();
    exit(EXIT_SUCCESS);
}
