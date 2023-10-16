#ifndef ARGS_HANDLING_H
#define ARGS_HANDLING_H

#include <string>
#include <fstream>

#define N_FLAG_DEFAULT 50
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
    offset              = 3, 
    summary             = 4, 
};

struct MandatoryArgs
{
    char* traceFile;
    char* accessFile;
    int levelBits;
    int totalBits;
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
    MandatoryArgs mandatoryArgs;
    OptionalArgs optionalArgs;
};

namespace ArgsHandling
{
    Args processArgs(int argc, char* argv[]);
    const char* getTraceFilePath(char* argv[]);
    const char* getAccessFilePath(char* argv[]);
}

std::ostream& operator<<(std::ostream& stream, Args& args);

#endif