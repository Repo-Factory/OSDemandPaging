#ifndef INPUT_HANDLING_H
#define INPUT_HANDLING_H

#include <fstream>
#include <vector>
#include <string>
#include <functional>

using TwoLineFunction = const std::function<void(std::string&, std::string&)>;

namespace FileHandler
{
    void forEachLineOfFiles(const std::string& file1, const std::string& file2, TwoLineFunction performOperation);
}

#endif