/* 
 * @brief Class Handles Command line argument parsing and validation
 *
 * Args object made of optional and mandatory args structs
 * Struct will lay out default values for flags if none are passed in
 * Getopt will be used to parse optional arguments if given
 * Appropriate error messages will be displayed for incorrect usages
 * Program mandates two arguments for the file inputs
 * See macros for more usage details
 * 
 */

#include "args_handling.h"
#include <getopt.h>
#include <unistd.h>
#include <numeric>
#include <iostream>
#include <map>

#define TRACE_FILE_INDEX 1
#define ACCESS_FILE_INDEX 2
#define INIT_LOOP_COUNTER 0

#define NUM_MANDATORY_ARGS 3
#define INVALID -1
#define READ_MODE "r"
#define FLAG_OPTIONS "n:f:a:l:"
#define N_FLAG_IDENTIFER 'n'
#define F_FLAG_IDENTIFER 'f'
#define A_FLAG_IDENTIFER 'a'
#define L_FLAG_IDENTIFER 'l'
#define OUT_OF_RANGE_N_FLAG(n_flag) n_flag<1
#define OUT_OF_RANGE_F_FLAG(f_flag) f_flag<1 
#define OUT_OF_RANGE_A_FLAG(a_flag) a_flag<1 
#define INVALID_FILE_MESSAGE "Unable to open %s"
#define INVALID_N_FLAG_ERROR_MESSAGE "Number of memory accesses must be a number, greater than 0"
#define INVALID_F_FLAG_ERROR_MESSAGE "Number of available frames must be a number, greater than 0"
#define INVALID_A_FLAG_ERROR_MESSAGE "Age of last access considered recent must be a number, greater than 0"
#define DEFAULT_ERROR_MESSAGE "Usage: %s -n memory_accesses -f physical_frames -a age_last_access -l logging_mode tracefile accessfile [levels_bits] "

namespace
{
    const std::map<std::string, LoggingMode> loggingModes
    {
        {"bitmasks",     LoggingMode::bitmasks},              
        {"va2pa",        LoggingMode::va2pa},                
        {"vpns_pfn",     LoggingMode::vpns_pfn},
        {"vpn2pfn_pr",   LoggingMode::vpn2pfn_pr},             
        {"offset",       LoggingMode::offset},               
        {"summary",      LoggingMode::summary},              
    };

    LoggingMode getLoggingMode(const char* loggingMode)
    {
        auto it = loggingModes.find(std::string(loggingMode));
        if (it != loggingModes.end()) {
            return it->second;
        } else {
            return LoggingMode::summary;
        }
    }
}

namespace
{
    void validate_nFlag(const OptionalArgs& optionalArgs)
    {
        if (OUT_OF_RANGE_N_FLAG(optionalArgs.n_flag)) {
            printf(INVALID_N_FLAG_ERROR_MESSAGE);
            exit(EXIT_FAILURE);
        }
    }

    void validate_fFlag(const OptionalArgs& optionalArgs)
    {
        if (OUT_OF_RANGE_F_FLAG(optionalArgs.f_flag)) {
            printf(INVALID_F_FLAG_ERROR_MESSAGE);
            exit(EXIT_FAILURE);
        }
    }

    void validate_aFlag(const OptionalArgs& optionalArgs)
    {
        if (OUT_OF_RANGE_A_FLAG(optionalArgs.a_flag)) {
            printf(INVALID_A_FLAG_ERROR_MESSAGE);
            exit(EXIT_FAILURE);
        }
    }

    void checkMinimumLevelZero(const std::vector<uint32_t>& levels)
    {
        if (levels[0] < MINIMUM_LEVEL_BITS) {
            printf(INVALID_LEVEL_BITS_MESSAGE);
            exit(EXIT_FAILURE);
        }
    }

    void checkMaximumLevelsBits(const std::vector<uint32_t>& levels)
    {
        if (std::accumulate(levels.begin(), levels.end(), 0) > MAXIMUM_TOTAL_BITS) {
            printf(INVALID_TOTAL_BITS_MESSAGE);
            exit(EXIT_FAILURE);
        }
    }

    void checkInputFileValidity(char* const file_path)
    {
        FILE *file = fopen(file_path, READ_MODE); 
        if (file == NULL) {
            printf(INVALID_FILE_MESSAGE, file_path);
            exit(EXIT_FAILURE);
        }
        fclose(file);
    }

    void printDefaultError(char* argv[])
    {
        printf(DEFAULT_ERROR_MESSAGE, argv[0]);
        exit(EXIT_FAILURE);
    }
}

namespace
{
    MandatoryArgs getMandatoryArgs(int argc, char* argv[])
    {
        if (argc - optind < NUM_MANDATORY_ARGS) {
            printDefaultError(argv);
        }

        int idx = optind;
        char* const trace_file_path = argv[idx++];
        char* const access_file_path = argv[idx++];
        
        std::vector<uint32_t> levels;               // Grab variable number of level bits
        while (idx < argc) {
            levels.push_back(std::stoi(argv[idx++])); 
        }

        checkInputFileValidity(trace_file_path);
        checkInputFileValidity(access_file_path);
        checkMinimumLevelZero(levels);
        checkMaximumLevelsBits(levels);

        return MandatoryArgs{trace_file_path, access_file_path, levels};
    }

    OptionalArgs getOptionalArgs(int argc, char* argv[])
    {
        OptionalArgs optionalArgs;
        int option_index = INIT_LOOP_COUNTER;
        while ((option_index = getopt(argc, argv, FLAG_OPTIONS)) != INVALID) 
        {
            switch (option_index) 
            {
                case N_FLAG_IDENTIFER:
                    optionalArgs.n_flag = std::stoi(optarg);
                    validate_nFlag(optionalArgs);
                    break;
                case F_FLAG_IDENTIFER:
                    optionalArgs.f_flag = std::stoi(optarg);
                    validate_fFlag(optionalArgs);
                    break;
                case A_FLAG_IDENTIFER:
                    optionalArgs.a_flag = std::stoi(optarg);
                    validate_aFlag(optionalArgs);
                    break;
                case L_FLAG_IDENTIFER:
                    optionalArgs.l_flag = getLoggingMode(optarg);
                    break;
                default:
                    printDefaultError(argv);
            }
        }
        return optionalArgs;
    }
}

Args ArgsHandling::processArgs(int argc, char* argv[])
{
    return Args{getOptionalArgs(argc, argv), getMandatoryArgs(argc, argv)};
}
