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
    /* INIT DATA STRUCTURES */
    const Args args =                 ArgsHandling::processArgs(argc, argv);
    PageTable pageTable =             createPageTable(args.mandatoryArgs.levelBits);
    exitIfBitMaskFlag                 (args, pageTable);

    /////////////////////////////////////////////////////////////
    Ring circularList                 (args.optionalArgs.f_flag);
    uint32_t frameNumber              = 0;    
    uint32_t hits                     = 0;
    uint32_t replacements             = 0;
    /////////////////////////////////////////////////////////////

    /* ENTER LOOP FOR EACH ADDRESS */
    const uint32_t addressesProcessed = forEachAddress(args, [&](const uint32_t vAddr, const uint32_t accessMode) {
        
        const bool hit = findVpn2PfnMapping(&pageTable, vAddr) != nullptr;
        hits += hit;
        int vpnReplaced = -1; // No page replacement

        // If hit exists, fall through to rest of functionality
        if (!hit)
        {
            if (args.optionalArgs.f_flag > circularList.current_index)
            {
                frameNumber += insertVpn2PfnMapping(&pageTable, vAddr, frameNumber);
            }
            else
            {
                vpnReplaced = replacePage(pageTable, circularList, vAddr, args.optionalArgs.a_flag);
                replacements++;
            }
        }
        
        if (vpnReplaced==-1) updatePageList(circularList, vAddr, (Mode)accessMode);

        /* EXTRACT DATA */
        const uint32_t frame = findVpn2PfnMapping(&pageTable, vAddr)->frame_number;
        const uint32_t offset = vAddr & XONES(pageTable.offsetBits);
        const uint32_t pfn = addFrameAndOffset(frame, offset, pageTable.offsetBits); 

        /* LOGGING */
        switch(args.optionalArgs.l_flag)
        {
            case LoggingMode::va2pa:
                log_va2pa(vAddr, pfn);
                break;
            case LoggingMode::vpns_pfn:
                log_vpns_pfn(pageTable.treeDepth, getVpnAtEachLevel(vAddr, pageTable).data(), frame);
                break;
            case LoggingMode::vpn2pfn_pr:
                log_mapping(offset, pfn, vpnReplaced, hit);
                break;
            case LoggingMode::offset:
                print_num_inHex(offset);
                break; 
        }
    });

    if (args.optionalArgs.l_flag == LoggingMode::summary) {
        log_summary(TWO_TO_POWER_OF(pageTable.offsetBits), replacements, hits, addressesProcessed, frameNumber, sizeof(pageTable));
    }
}
