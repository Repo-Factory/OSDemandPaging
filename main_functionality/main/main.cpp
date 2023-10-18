/*******************************************************************************
 * @author Strix Elixel
 * @name main_functionality
 * 
 * @brief Reads words from two files passed as cmd line arguments 
 * 
 * @description 
 * 
 * 
 * 
 * See cpp file of each for more details in each respective area
 * 
 ******************************************************************************/

#include "args_handling.h"
#include "page_table.h"
#include "page_functions.h"
#include "vaddr_tracereader.h"
#include "wsclock_algorithm.h"
#include "log_helpers.h"
#include <functional>
    #include <iostream>

#define READ_MODE "r"

uint32_t addFrameAndOffset(const uint32_t frame, const uint32_t offset, const uint32_t offsetBits)
{
    return ((frame << offsetBits) + offset);
}

int replacePageIfNecessary(PageTable& pageTable, Ring& circularList, const unsigned int virtualAddress, const int threshold)
{
    int index = findAvailablePage(circularList, threshold);
    if (circularList.current_index == circularList.capacity) {
        pageReplaceClock(circularList, index, virtualAddress);
        pageReplaceTree(pageTable, virtualAddress);
    }
    return index;
}

void forEachAddress(const Args& args, uint32_t& frame, std::function<void(const uint32_t, const uint32_t)> performOperations)
{
    int programCounter =              0;
    FILE* traceFile =                 fopen(args.mandatoryArgs.traceFile, READ_MODE);
    FILE* accessFile =                fopen(args.mandatoryArgs.accessFile, READ_MODE);
    p2AddrTr mtrace;
    char accessBit;
    while (!(feof(traceFile) || feof(accessFile)))
    {
        if (programCounter++ == args.optionalArgs.n_flag) break; //args.optionalArgs.n_flag
        NextAddress(traceFile, &mtrace);
        fread(&accessBit, sizeof(char), 1, accessFile);
        performOperations(mtrace.addr, atoi(&accessBit));
    }
}

void exitIfBitMaskFlag(const Args& args, PageTable& pageTable)
{
    if (args.optionalArgs.l_flag == LoggingMode::bitmasks) {
        log_bitmasks(pageTable.bitMasks.size(), pageTable.bitMasks.data());
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char* argv[])
{
    const Args args =                 ArgsHandling::processArgs(argc, argv);
    PageTable pageTable =             createPageTable(args.mandatoryArgs.levelBits);
    exitIfBitMaskFlag(args, pageTable);
    
    
    Ring circularList                 (args.optionalArgs.f_flag);
    uint32_t seqFrame                 = 0;    

    forEachAddress(args, seqFrame, [&](const uint32_t vAddr, const uint32_t accessMode) {
        seqFrame += insertVpn2PfnMapping(&pageTable, vAddr, seqFrame);
       
        switch(args.optionalArgs.l_flag)
        {
            case LoggingMode::bitmasks:
                log_bitmasks(pageTable.bitMasks.size(), pageTable.bitMasks.data());
                break;
            case LoggingMode::va2pa:
                log_va2pa(vAddr, addFrameAndOffset(findVpn2PfnMapping(&pageTable, vAddr)->frame_number, (vAddr & XONES(pageTable.offsetBits)), pageTable.offsetBits));
                break;
            case LoggingMode::vpns_pfn:
                // log_vpns_pfn(pageTable.bitMasks.size(), , seqFrame);
                break;
            // case LoggingMode::offset:
            //     log_mapping(vAddr, addFrameAndOffset(findVpn2PfnMapping(&pageTable, vAddr)->frame_number, vAddr & offsetBits, offsetBits)), 
            //     index, findVpn2PfnMapping(&pageTable, vAddr) != nullptr);
            //     break; 
        }

        // updatePageList(circularList, vAddr, (Mode)accessMode);
        // const int index = replacePageIfNecessary(pageTable, circularList, vAddr, args.optionalArgs.a_flag);
        
    });
}

/* addFrameAndOffset(frame, vpn & offsetBits, offsetBits); */
/* printf("%x, %d\n", vAddr, accessMode);
        seqFrame++; */


/* switch(args.optionalArgs.l_flag)
        {
            case LoggingMode::bitmasks:
                log_bitmasks(args.mandatoryArgs.levelBits, pageTable.bitMasks.data());
                break;
            case LoggingMode::va2pa:
                log_va2pa(virtualAddress, findVpn2PfnMapping(&pageTable, virtualAddress))>frame_number);
                break;
            case LoggingMode::vpns_pfn:
                // log_vpns_pfn(args.mandatoryArgs.levelBits, pageTable., sequentialFrameNumber);
                break;
            case LoggingMode::offset:
                // log_mapping(virtualAddress, findVpn2PfnMapping(&pageTable, virtualAddress))>frame_number, 
                // index, );
                break;        
            case LoggingMode::summary:
                // log_summary(args.mandatoryArgs.totalBits, );
                break;
        }; */