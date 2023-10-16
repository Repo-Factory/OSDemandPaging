#ifndef INPUT_HANDLING_H
#define INPUT_HANDLING_H

#include <fstream>
#include <vector>
#include <string>
#include <functional>

namespace FileHandler
{
    void forEachLineOfFile(const std::string& filename, const std::function<void(std::string&)> performOperation);
    int getLineCount(const std::string& filename);
    int getLetterCount(const std::string& filename);
}

#endif