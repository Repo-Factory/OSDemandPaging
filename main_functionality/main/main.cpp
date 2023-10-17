/*******************************************************************************
 * @author Strix Elixel
 * @name main_functionality
 * 
 * @brief Reads words from two files passed as cmd line arguments 
 * 
 * @description For each line in file2, it will output to stdout the numbers of substrings
 * 
 * Program must be called like this or will gracefully exit: ./program arg1 arg2
 * 
 * Relevant entities
 * - Dependencies
 *      - ProgressBar
 * - ArgsHandling       - For command line arguments
 * - InputHandling      - For file operations
 * 
 * See cpp file of each for more details in each respective area
 * 
 ******************************************************************************/

/* Upon start, your program creates an empty page table (only the level 0 / root node should
be allocated). The program should read logical / virtual addresses one at a time from a
memory trace file. The trace file consists of memory reference traces for simulating a
series of access attempts to logical / virtual addresses. Another file with a series of zeros
and ones would give the access mode of each memory access, 0 means read, whereas 1
means write.
For each virtual address read in, simulate the Operating System demand paging, and
page replacement process as well as the memory management unit (MMU) for
translating virtual address to physical address (See Figure 1 below):
2 | P a g e
1) Extract the virtual page number (VPN), walk the page table tree to find the
Virtual Page Number (VPN) → Physical Frame Number (PFN) mapping
information.
2) If the VPN → PFN mapping entry is found, use the found PFN for translation.
3) If the VPN → PFN mapping entry is NOT found:
a. If there is still free physical frame available:
i. insert the page to the page table tree to map it to the next available
frame index (starts from 0 and continues sequentially) that
simulates the demand paging allocation of a new physical frame
(PFN) to the virtual page loaded into that frame.
b. Else:
i. Use a modified working set clock (WSClock) algorithm (see
a3specs.pdf) to find a victim frame to evict and map the VPN of
the page being accessed to the victim frame as the PFN.
i. Remember: Update the mapping of the replaced VPN
(victim) to PFN in the page table to be invalid.
c. Use the PFN for translation.
4) In all cases described in 2) and 3), update or insert to the page recent access
history for potential later page replacement.
5) Print appropriate logs to the standard output as specified below in “User
Interface” */

#include "main_functionality.h"
#include "input_handling.h"
#include "args_handling.h"
#include "page_table.h"
#include <memory>

int main(int argc, char* argv[])
{
    const Args args = ArgsHandling::processArgs(argc, argv);
    FileHandler::forEachLineOfFile(ArgsHandling::getTraceFilePath(argv), [&](const std::string virtualAddress){

        printAppropriateLogs(args.optionalArgs.l_flag);
    });
}
