#ifndef MAIN_FUNCTIONALITY_H
#define MAIN_FUNCTIONALITY_H

#include <stdio.h>
#include <stdint.h>
#include <functional>
#include <string>
#include "args_handling.h"

using AccessMode = bool;
using Address = int32_t;
using VPN = int32_t;
using PFN = int32_t;
using Success = bool;

Address readAddressFromTraceFile(FILE* traceFile);
AccessMode readAccessMode(FILE* accessFile);
void forEachAddress(std::function<void(Address)> performOperations);
Address convertStringToAddress(const std::string vpn);
Success printAppropriateLogs(const LoggingMode loggingMode);


#endif
