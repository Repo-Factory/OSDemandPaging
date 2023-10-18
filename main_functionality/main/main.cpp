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

// uint32_t traverseLevelForStatistics(const PageNode* pageNode, uint32_t& totalBytes)
// {
//     if (pageNode->nodeDepth == pageNode->pageTable.+1) // -1 To account for Index starting from 0 
//     {
//         auto currentNode = (LeafNode*)pageNode;
//         return totalBytes + 
//     }
//     auto currentNode = (InternalNode*)pageNode;
//     return currentNode->childNodes
// }

// uint32_t traverseTreeForStatistics(const PageTable& pageTable)
// {
//     uint32_t totalBytes=0;
//     traverseLevelForStatistics(pageTable.level_zero, totalBytes);
// }

std::vector<uint32_t> getVpnAtEachLevel(const uint32_t vpn, const PageTable& pageTable)
{
    std::vector<uint32_t> vpnAtEachLevel;
    for (int i = 0; i < pageTable.treeDepth+1; i++)
    {
        vpnAtEachLevel.push_back(((vpn & pageTable.bitMasks[i]) >> pageTable.bitShifts[i]));
    }
    return vpnAtEachLevel;
}

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

        switch(args.optionalArgs.l_flag)
        {
            case LoggingMode::va2pa:
                log_va2pa(vAddr, pfn);
                break;
            case LoggingMode::vpns_pfn:
                log_vpns_pfn(pageTable.treeDepth+1, getVpnAtEachLevel(vAddr, pageTable).data(), frame);
                break;
            case LoggingMode::vpns2pfn_ptr:
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