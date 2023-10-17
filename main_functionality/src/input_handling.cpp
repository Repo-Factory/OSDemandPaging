/* 
 * @brief FileHandler namespace defines functions for file operations
 *
 * This is used for interacting with vocabulary.txt and testfile.txt files that get passed in.
 */


#include "input_handling.h"
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <thread>
#include <chrono>

#define FILE_OPEN_ERROR_MESSAGE "Unable to open %s\n"

namespace  
{
    std::ifstream openFile(const std::string& filename) // Generic open function.
    {
        std::ifstream stream(filename);
        if (stream.fail()) {
            printf(FILE_OPEN_ERROR_MESSAGE, filename.c_str());
            exit(EXIT_FAILURE);
        }
        return stream;
    } // Note the file will have to be closed at some point if this function is called
}

// Takes in function pointer param (well, C++ version) to perform passed in operation on each string of file passed in
// I chose to do this to keep the function generic to file handling and keep file traversal logic out of main

void FileHandler::forEachLineOfFiles(const std::string& file1, const std::string& file2, TwoLineFunction performOperation)
{
    std::ifstream stream1 = openFile(file1);
    std::ifstream stream2 = openFile(file2);
    std::string line1;
    std::string line2;
    while (!(stream1.eof() || stream2.eof())) {
        std::getline(stream1, line1);
        std::getline(stream2, line2);
        performOperation(line1, line2);
    }
    stream1.close();
    stream2.close();
}

/* Count number of lines in file */
int FileHandler::getLineCount(const std::string& filename)
{
    std::ifstream stream = openFile(filename);
    std::string line;
    int lineCount=0;
    while (std::getline(stream, line))
        lineCount++;    
    stream.close();
    return lineCount;
}

/* Count number of characters in file */
int FileHandler::getLetterCount(const std::string& filename)
{
    std::ifstream stream = openFile(filename);
    char letter;
    int letterCount=0;
    while (stream.get(letter))
        letterCount++;
    stream.close();
    return letterCount;
}