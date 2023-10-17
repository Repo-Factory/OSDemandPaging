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

#include "input_handling.h"
#include "args_handling.h"
#include "page_table.h"
#include "page_functions.h"
#include "wsclock_algorithm.h"
#include "log_helpers.h"

int replacePageIfNecessary(PageTable& pageTable, Ring& circularList, const unsigned int virtualAddress, const int threshold)
{
    int index = findAvailablePage(circularList, threshold);
    if (circularList.current_index == circularList.capacity) {
        pageReplaceClock(circularList, index, virtualAddress);
        pageReplaceTree(pageTable, virtualAddress);
    }
    return index;
}

int main(int argc, char* argv[])
{
    const Args args = ArgsHandling::processArgs(argc, argv);
    PageTable pageTable = createPageTable(args.mandatoryArgs.levelBits, args.mandatoryArgs.totalBits);
    Ring circularList(args.optionalArgs.f_flag);
    uint32_t sequentialFrameNumber = 0;
    


    FileHandler::traverseFiles(ArgsHandling::getTraceFilePath(argv), ArgsHandling::getAccessFilePath(argv), 
    [&](const std::string& virtualAddressFromFile, const std::string& accessModeFromeFile) {

        if (sequentialFrameNumber == args.optionalArgs.n_flag) return;
        insertVpn2PfnMapping(&pageTable, std::stoi(virtualAddressFromFile), sequentialFrameNumber++);
        updatePageList(circularList, std::stoi(virtualAddressFromFile), static_cast<Mode>(std::stoi(accessModeFromeFile)));
        const int index = replacePageIfNecessary(pageTable, circularList, std::stoi(virtualAddressFromFile), args.optionalArgs.a_flag);
        
        
        switch(args.optionalArgs.l_flag)
        {
            case LoggingMode::bitmasks:
                log_bitmasks(args.mandatoryArgs.levelBits, pageTable.bitMasks.data());
                break;
            case LoggingMode::va2pa:
                // log_va2pa(std::stoi(virtualAddressFromFile), findVpn2PfnMapping(&pageTable, std::stoi(virtualAddressFromFile))->frame_number);
                break;
            case LoggingMode::vpns_pfn:
                // log_vpns_pfn(args.mandatoryArgs.levelBits, pageTable., sequentialFrameNumber);
                break;
            case LoggingMode::offset:
                // log_mapping(std::stoi(virtualAddressFromFile), findVpn2PfnMapping(&pageTable, std::stoi(virtualAddressFromFile))->frame_number, 
                // index, );
                break;        
            case LoggingMode::summary:
                // log_summary(args.mandatoryArgs.totalBits, );
                break;
        };
    });
}
