/*******************************************************************************
 * @author Conner Sommerfield 824769654
 * @name demandpaging
 * 
 * @brief Perform virtual page mapping to a physical address with page replacement.
 * 
 * @description We will use a tree implementation to map virtual addresses taken from a trace file to physical addresses
 * We will also keep track of read/write accesses and store the appropriate information to provide page replacement when necessary
 * 
 * MainFunctionality
 *  - Command line args handling
 *  - Actual file handling, page table utilization, logging
 * PageTable
 *  - PageTable Data Structure
 *  - PageTable Nodes Definitions
 *  - PageTable Functions (traversal, insertion, search)
 * WSClock
 *  - Page Replacement Handling
 * Helpers
 *  - Provided logging functions
 *  - Provided trace file functions
 * See cpp file of each for more details in each respective area
 * 
 ******************************************************************************/

#include "main_functionality.h"
#include "page_functions.h"
#include "log_helpers.h"
#define NO_REPLACEMENT -1

// Holds our mutable data that will keep track of progress
struct StatsTracker
{
    uint32_t currentFrame = 0;
    uint32_t hits = 0;
    uint32_t replacements = 0;
};

// Small helper function to clean up syntax of retrieving frame
uint32_t getFrame(PageTable& pageTable, const uint32_t vAddr)
{
    return findVpn2PfnMapping(&pageTable, vAddr)->frame_number;
}

// Case where we must add a new page and have a frame available
uint32_t handleNewFrame(StatsTracker& statsTracker, PageTable& pageTable, Ring& circularList, const uint32_t vAddr) 
{
    addPageToList(circularList, vAddr, statsTracker.currentFrame);
    statsTracker.currentFrame += insertVpn2PfnMapping(&pageTable, vAddr, statsTracker.currentFrame);
    return NO_REPLACEMENT;
}

// Case where no grame is available and we must find a victim using our clock
uint32_t handlePageReplacement(StatsTracker& statsTracker, PageTable& pageTable, Ring& circularList, const uint32_t vAddr) 
{
    const uint32_t vpnReplaced = replacePage(pageTable, circularList, vAddr, circularList.threshold);
    const uint32_t vpnNoOffset = removeOffset(vpnReplaced, pageTable.offsetBits);
    statsTracker.replacements++;
    return vpnNoOffset;
}

// Will Check if a vAddr exists, then perform proper page insertion / replacement if needed
uint32_t processAddress(PageTable& pageTable, Ring& circularList, StatsTracker& statsTracker, const uint32_t vAddr) 
{
    const PageMap* pageMap = findVpn2PfnMapping(&pageTable, vAddr);
    const bool hit = (pageMap != nullptr && pageMap->valid);
    const bool frameAvailable = statsTracker.currentFrame < circularList.capacity;
    statsTracker.hits += hit;

    if (hit) {
        return NO_REPLACEMENT;
    }
    if (frameAvailable) {
        return handleNewFrame(statsTracker, pageTable, circularList, vAddr);
    } 
    else {
        return handlePageReplacement(statsTracker, pageTable, circularList, vAddr);
    }
}

// Function to handle logging based on the l_flag
void handleLogging(const LoggingMode loggingMode, const PageTable& pageTable, const uint32_t vAddr, const uint32_t pfn, const uint32_t vpnReplaced, const bool hit) 
{
    switch (loggingMode) 
    {
        case LoggingMode::va2pa:
            log_va2pa(vAddr, pfn);
            break;
        case LoggingMode::vpns_pfn:
            log_vpns_pfn(pageTable.treeDepth, getVpnAtEachLevel(vAddr, pageTable).data(), pfn);
            break;
        case LoggingMode::vpn2pfn_pr:
            log_mapping(removeOffset(vAddr, pageTable.offsetBits), pfn, vpnReplaced, hit);
            break;
        case LoggingMode::offset:
            print_num_inHex(vAddr & XONES(pageTable.offsetBits));
            break;
    }
}

int main(int argc, char* argv[])
{
    /* INIT DATA STRUCTURES */
    const Args args =                 ArgsHandling::processArgs(argc, argv);
    const LoggingMode loggingMode =   args.optionalArgs.l_flag;
    PageTable pageTable =             createPageTable(args.mandatoryArgs.levelBits);
    Ring circularList                 (args.optionalArgs.f_flag, args.optionalArgs.a_flag);
    StatsTracker statsTracker;

    /* ENTER LOOP FOR EACH ADDRESS */
    exitIfBitMaskFlag(args, pageTable);
    const uint32_t addressesProcessed = forEachAddress(args, [&](const uint32_t vAddr, const uint32_t accessMode) {
        const uint32_t vpnReplaced = processAddress(pageTable, circularList, statsTracker, vAddr);
        updateAccessHistory(circularList, getFrame(pageTable, vAddr), static_cast<Mode>(accessMode));
        handleLogging(loggingMode, pageTable, vAddr, getFrame(pageTable, vAddr), vpnReplaced, vpnReplaced != NO_REPLACEMENT);
    });
    
    /* END PROGRAM */
    if (loggingMode != LoggingMode::summary) exit(EXIT_SUCCESS);

    log_summary(
        TWO_TO_POWER_OF(pageTable.offsetBits), statsTracker.replacements, statsTracker.hits, 
        addressesProcessed, statsTracker.currentFrame, getSizeOfPageTable(&pageTable)
    );
}
