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

uint32_t removeOffset(const uint32_t vAddr, const uint32_t offsetBits)
{
    return (vAddr & XZEROS(offsetBits)) >> offsetBits;
}

uint32_t forEachAddress(const Args& args, std::function<void(const uint32_t, const uint32_t)> performOperations)
{
    int addressesProcessed =              0;
    FILE* traceFile =                     fopen(args.mandatoryArgs.traceFile, READ_MODE);
    FILE* accessFile =                    fopen(args.mandatoryArgs.accessFile, READ_MODE);
    p2AddrTr mtrace;
    char accessBit;
    int accessBitInt = 0;
    
    while (!(feof(traceFile) || feof(accessFile)))
    {
        if (addressesProcessed == args.optionalArgs.n_flag) {
            break; 
        } addressesProcessed++;

        NextAddress(traceFile, &mtrace);
        fread(&accessBit, sizeof(char), 1, accessFile);
        if (accessBit == '0')
            accessBitInt = 0;
        else if ((accessBit == '1'))
            accessBitInt = 1;
        performOperations(mtrace.addr, accessBitInt);
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
