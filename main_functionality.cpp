#include "main_functionality.h"
#include "page_functions.h"
#include "vaddr_tracereader.h"
#include "log_helpers.h"

#define READ_MODE "r"
#define NO_PAGE_HIT -1

// Use Bit Manipulations to get final pfn with offset
uint32_t addFrameAndOffset(const uint32_t frame, const uint32_t offset, const uint32_t offsetBits)
{
    return ((frame << offsetBits) + offset);
}

// Passed to logging function for vpns_pfn case
std::vector<uint32_t> getVpnAtEachLevel(const uint32_t vpn, const PageTable& pageTable)
{
    std::vector<uint32_t> vpnAtEachLevel;
    for (int i = 0; i < pageTable.treeDepth; i++)
    {
        vpnAtEachLevel.push_back(getVPNFromVirtualAddress(vpn, pageTable.bitMasks[i], pageTable.bitShifts[i]));
    }
    return vpnAtEachLevel;
}

// Handle -l bitmasks command line argument
void exitIfBitMaskFlag(const Args& args, PageTable& pageTable)
{
    if (args.optionalArgs.l_flag == LoggingMode::bitmasks) {
        log_bitmasks(pageTable.bitMasks.size(), pageTable.bitMasks.data());
        exit(EXIT_SUCCESS);
    }
}

// Add all fields of our page table and recurse through nodes to get size of page table in bytes
const uint32_t getSizeOfPageTable(const PageTable* pageTable)
{
    uint32_t totalBytes = 0;
    totalBytes += sizeof(pageTable->bitMasks);
    totalBytes += sizeof(pageTable->bitShifts);
    totalBytes += sizeof(pageTable->entryCounts);
    totalBytes += sizeof(pageTable->offsetBits);
    totalBytes += sizeof(pageTable->treeDepth);
    totalBytes += sizeof(pageTable->level_zero);
    getSizeOfPageTable(pageTable->level_zero, totalBytes);
    return totalBytes;
}

// Recursive function which will take reference to what will eventually be our total, we will go through each node, adding it's
// size to the total, then recursively call if necessary or simply add all our pagemap sizes.
void getSizeOfPageTable(const PageNode* pageNode, uint32_t& totalBytes)
{   
    if (pageNode == nullptr) return;    
    totalBytes += sizeof(*pageNode);                               // Current node size

    if (pageNode->nodeDepth == pageNode->pageTable.treeDepth - 1)  // Leaf node base case
    {
        auto currentNode = (LeafNode*)pageNode;
        for (auto pageMap : currentNode->pageMaps) 
        {
            totalBytes += sizeof(pageMap);
        }
        return;
    }
    auto currentNode = (InternalNode*)pageNode;                   // Internal node means we recurse after adding current node size
    for (auto internalNode : currentNode->childNodes) 
    {
        getSizeOfPageTable((PageNode*)internalNode, totalBytes);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace // File iteration helpers
{
    const uint32_t convertAccessBit(const char accessBit)
    {
        if (accessBit == '0')
            return 0;
        else if (accessBit == '1')
            return 1;
    }

    const uint32_t getTraceEnd(const char* traceFilePath)
    {
        FILE* traceFile = fopen(traceFilePath, READ_MODE);
        p2AddrTr mtrace;
        int eof = 0;
        while (!(feof(traceFile))) {
            NextAddress(traceFile, &mtrace);
            eof++;
        }
        fclose(traceFile);
        return eof-1;
    }

    const uint32_t getAccessFileEnd(const char* accessFilePath)
    {
        FILE* accessFile = fopen(accessFilePath, READ_MODE);
        char accessBit;
        int eof = 0;
        while (!(feof(accessFile))) {
            fread(&accessBit, sizeof(char), 1, accessFile);
            eof++;
        }
        fclose(accessFile);
        return eof-1;
    }
}

// This is main loop mechanism that will iterate through the two input files, grabbing the appropriate values to pass on to main function
uint32_t forEachAddress(const Args& args, const std::function<void(const uint32_t, const uint32_t)> performOperations)
{
    int addressesProcessed =              0;
    FILE* traceFile =                     fopen(args.mandatoryArgs.traceFile, READ_MODE);
    FILE* accessFile =                    fopen(args.mandatoryArgs.accessFile, READ_MODE);
    p2AddrTr mtrace;
    char accessBit;

    const uint32_t traceFileEnd = getTraceEnd(args.mandatoryArgs.traceFile);
    const uint32_t accessFileEnd = getAccessFileEnd(args.mandatoryArgs.accessFile);

    while ((addressesProcessed < traceFileEnd && addressesProcessed < accessFileEnd))
    {
        if (addressesProcessed == args.optionalArgs.n_flag) {
            break; 
        } addressesProcessed++;

        NextAddress(traceFile, &mtrace);
        fread(&accessBit, sizeof(char), 1, accessFile);
        
        performOperations(mtrace.addr, convertAccessBit(accessBit)); // C++ "function pointer" which will be what's performed in main
    }
    return addressesProcessed;
}