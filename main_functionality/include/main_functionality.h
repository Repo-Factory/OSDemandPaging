#ifndef MAIN_FUNCTIONALITY_H
#define MAIN_FUNCTIONALITY_H

#include <stdio.h>
#include <stdint.h>
#include <functional>
#include <string>
#include "args_handling.h"

using Success = bool;

Success printAppropriateLogs(const LoggingMode loggingMode);

#endif
