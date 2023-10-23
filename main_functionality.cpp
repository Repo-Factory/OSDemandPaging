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
        
        performOperations(mtrace.addr, convertAccessBit(accessBit));
    }
    return addressesProcessed;
}

const uint32_t getSizeOfPageTable(PageTable& pageTable)
{
    
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
