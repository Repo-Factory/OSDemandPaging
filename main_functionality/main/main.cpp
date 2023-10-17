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
#include <memory>

int main(int argc, char* argv[])
{
    const Args args = ArgsHandling::processArgs(argc, argv);
    FileHandler::forEachLineOfFiles(ArgsHandling::getTraceFilePath(argv), ArgsHandling::getAccessFilePath(argv), 
    [&](const std::string virtualAddressFromFile, const std::string accessModeFromeFile){
        const unsigned int virtualAddress = std::stoi(virtualAddressFromFile);
        const bool accessMode = std::stoi(accessModeFromeFile);
        printAppropriateLogs(args.optionalArgs.l_flag);
    });
}
