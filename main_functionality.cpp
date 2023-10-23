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

// Used to get pure level value without offset
uint32_t removeOffset(const uint32_t vAddr, const uint32_t offsetBits)
{
    return (vAddr & XZEROS(offsetBits)) >> offsetBits;
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

const uint32_t getSizeOfPageTable(PageTable* pageTable)
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

void getSizeOfPageTable(PageNode* pageNode, uint32_t& totalBytes)
{   
    if (pageNode == nullptr) return;
    totalBytes += sizeof(*pageNode);

    if (pageNode->nodeDepth == pageNode->pageTable.treeDepth - 1)  // -1 To account for Index starting from 0 
    {
        auto currentNode = (LeafNode*)pageNode;
        for (auto node : currentNode->pageMaps) 
        {
            totalBytes += sizeof(node);
        }
        return;
    }
    auto currentNode = (InternalNode*)pageNode;
    for (auto node : currentNode->childNodes) 
    {
        getSizeOfPageTable((PageNode*)node, totalBytes);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace // File iteration helpers
{
    const uint32_t convertAccessBit(char accessBit)
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
uint32_t forEachAddress(const Args& args, std::function<void(const uint32_t, const uint32_t)> performOperations)
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