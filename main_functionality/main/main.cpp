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

#include "main_functionality.h"
#include "page_functions.h"
#include "log_helpers.h"

int main(int argc, char* argv[])
{
    const Args args =                 ArgsHandling::processArgs(argc, argv);
    PageTable pageTable =             createPageTable(args.mandatoryArgs.levelBits);
    exitIfBitMaskFlag                 (args, pageTable);
    Ring circularList                 (args.optionalArgs.f_flag);
    uint32_t frameNumber              = 0;    

    const uint32_t AddressesProcessed = forEachAddress(args, frameNumber, [&](const uint32_t vAddr, const uint32_t accessMode) {
        
        frameNumber += insertVpn2PfnMapping(&pageTable, vAddr, frameNumber);
        updatePageList(circularList, vAddr, (Mode)accessMode);
        const int index = replacePageIfNecessary(pageTable, circularList, vAddr, args.optionalArgs.a_flag);
        const uint32_t frame = findVpn2PfnMapping(&pageTable, vAddr)->frame_number;
        const uint32_t offset = vAddr & XONES(pageTable.offsetBits);
        const uint32_t pfn = addFrameAndOffset(frame, offset, pageTable.offsetBits); 
        const bool hit = findVpn2PfnMapping(&pageTable, vAddr) != nullptr;
        std::vector vpns = getVpnAtEachLevel(vAddr, pageTable);

        switch(args.optionalArgs.l_flag)
        {
            case LoggingMode::va2pa:
                log_va2pa(vAddr, pfn);
                break;
            case LoggingMode::vpns_pfn:
                log_vpns_pfn(pageTable.treeDepth, vpns.data(), frame);
                break;
            case LoggingMode::vpn2pfn_pr:
                log_mapping(vAddr, offset, index, index==-1);
                break;
            case LoggingMode::offset:
                print_num_inHex(offset);
                break; 
        }
    });

    if (args.optionalArgs.l_flag == LoggingMode::summary) {
        log_summary(TWO_TO_POWER_OF(pageTable.offsetBits), 0, frameNumber, AddressesProcessed, frameNumber, sizeof(pageTable));
    }
}