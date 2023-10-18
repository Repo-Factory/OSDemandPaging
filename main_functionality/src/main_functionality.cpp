#include "main_functionality.h"
#include "page_functions.h"
#include "vaddr_tracereader.h"
#include "log_helpers.h"

#define READ_MODE "r"
#define NO_PAGE_HIT -1

uint32_t addFrameAndOffset(const uint32_t frame, const uint32_t offset, const uint32_t offsetBits)
{
    return ((frame << offsetBits) + offset);
}

int replacePageIfNecessary(PageTable& pageTable, Ring& circularList, const unsigned int virtualAddress, const int threshold)
{
    if (circularList.current_index > circularList.capacity) {
        const int index = findAvailablePage(circularList, threshold);
        pageReplaceClock(circularList, index, virtualAddress);
        pageReplaceTree(pageTable, virtualAddress);
        return index;
    }
    return NO_PAGE_HIT;
}

uint32_t forEachAddress(const Args& args, uint32_t& frame, std::function<void(const uint32_t, const uint32_t)> performOperations)
{
    int addressesProcessed =              0;
    FILE* traceFile =                 fopen(args.mandatoryArgs.traceFile, READ_MODE);
    FILE* accessFile =                fopen(args.mandatoryArgs.accessFile, READ_MODE);
    p2AddrTr mtrace;
    char accessBit;
    while (!(feof(traceFile) || feof(accessFile)))
    {
        if (addressesProcessed == args.optionalArgs.n_flag) {
            break; 
        } addressesProcessed++;

        NextAddress(traceFile, &mtrace);
        fread(&accessBit, sizeof(char), 1, accessFile);
        performOperations(mtrace.addr, atoi(&accessBit));
    }
    return addressesProcessed;
}

std::vector<uint32_t> getVpnAtEachLevel(const uint32_t vpn, const PageTable& pageTable)
{
    std::vector<uint32_t> vpnAtEachLevel;
    for (int i = 0; i < pageTable.treeDepth; i++)
    {
        vpnAtEachLevel.push_back(((vpn & pageTable.bitMasks[i]) >> pageTable.bitShifts[i]));
    }
    return vpnAtEachLevel;
}

void exitIfBitMaskFlag(const Args& args, PageTable& pageTable)
{
    if (args.optionalArgs.l_flag == LoggingMode::bitmasks) {
        log_bitmasks(pageTable.bitMasks.size(), pageTable.bitMasks.data());
        exit(EXIT_SUCCESS);
    }
}
