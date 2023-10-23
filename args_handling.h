#ifndef ARGS_HANDLING_H
#define ARGS_HANDLING_H

#include <string>
#include <fstream>
#include <vector>

#define N_FLAG_DEFAULT 999999
#define N_FLAG_MINIMUM 10
#define F_FLAG_DEFAULT 999999
#define A_FLAG_DEFAULT 10

#define MINIMUM_LEVEL_BITS 1
#define MAXIMUM_TOTAL_BITS 28
#define INVALID_LEVEL_BITS_MESSAGE "Level 0 page table must be at least 1 bit"
#define INVALID_TOTAL_BITS_MESSAGE "Too many bits used in page tables"

enum class LoggingMode
{
    bitmasks            = 0,  
    va2pa               = 1, 
    vpns_pfn            = 2,
    vpn2pfn_pr          = 3,
    offset              = 4, 
    summary             = 5, 
};

struct MandatoryArgs
{
    char* traceFile;
    char* accessFile;
    std::vector<uint32_t> levelBits;
};

struct OptionalArgs
{
    int n_flag = N_FLAG_DEFAULT;
    int f_flag = F_FLAG_DEFAULT;
    int a_flag = A_FLAG_DEFAULT;
    LoggingMode l_flag = LoggingMode::summary;
};

struct Args
{   
    OptionalArgs optionalArgs;
    MandatoryArgs mandatoryArgs;
};

namespace ArgsHandling
{
    Args processArgs(int argc, char* argv[]);
}

#endif