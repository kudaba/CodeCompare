#pragma once

#include <vector>
#include <functional>

//----------------------------------------------------------------------------------------
// File utilities
//----------------------------------------------------------------------------------------
namespace File
{
    //----------------------------------------------------------------------------------------
    // Do a file search given a path and mask: i.e. "../path/*.txt"
    // Calls the provided functor for each file found
    // * void Function(char const* filename, size_t filesize)
    //
    // Returns number of files found
    //----------------------------------------------------------------------------------------
    size_t FindFile(char const* path, char const* mask, std::function<void(char const*, size_t)> functor);

    //----------------------------------------------------------------------------------------
    // Read entire contents of file in one shot
    //----------------------------------------------------------------------------------------
    std::vector<char> ReadFile(char const* search);
    std::vector<char> ReadFile(char const* search, size_t bytes);
}