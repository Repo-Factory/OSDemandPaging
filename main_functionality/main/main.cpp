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
#include "input_handling.h"
#include "args_handling.h"
#include "page_table.h"
#include "wsclock_algorithm.h"
#include <memory>

void demandPaging(const unsigned int virtualAddress, const bool accessMode)
{

}

int main(int argc, char* argv[])
{
    const Args args = ArgsHandling::processArgs(argc, argv);
    PageTable<8, 3> pageTable = createPageTable<8, 3>(8, 3);
    Ring<256> circular_list;
    

    FileHandler::forEachLineOfFiles(ArgsHandling::getTraceFilePath(argv), ArgsHandling::getAccessFilePath(argv), 
    [&](const std::string virtualAddressFromFile, const std::string accessModeFromeFile){
        demandPaging(std::stoi(virtualAddressFromFile), std::stoi(accessModeFromeFile));
        printAppropriateLogs(args.optionalArgs.l_flag);
    });
}
