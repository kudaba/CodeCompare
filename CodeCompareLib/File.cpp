#include "File.h"

#include <fstream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace std;

namespace File
{
    size_t FindFile(char const* path, char const* mask, std::function<void(char const*, size_t)> functor)
    {
        WIN32_FIND_DATA fileData;
        HANDLE hFiles;

        string search = path;
        search += "/";
        search += mask;

        hFiles = FindFirstFile(search.c_str(), &fileData);

        if (INVALID_HANDLE_VALUE == hFiles)
        {
            return 0;
        }

        size_t count = 0;

        do
        {
            if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // recursive option?
            }
            else
            {
                string filename = path;
                filename += "/";
                filename += fileData.cFileName;
                if (sizeof(size_t) == 4)
                {
                    if (fileData.nFileSizeHigh)
                        continue;

                    functor(filename.c_str(), fileData.nFileSizeLow);
                }
                else
                {
                    functor(filename.c_str(), (__int64)fileData.nFileSizeHigh << 32 | fileData.nFileSizeLow);
                }
                ++count;
            }
        } while (FindNextFile(hFiles, &fileData) != 0);

        return count;
    }

    //----------------------------------------------------------------------------------------
    vector<char> ReadFileData(fstream& file, size_t bytes)
    {
        bytes = bytes < UINT32_MAX ? bytes : UINT32_MAX;

        vector<char> data;
        data.resize(bytes);
        file.read(data.data(), bytes);

        if (file.gcount() >= 0 && (size_t)file.gcount() < bytes)
            data.resize((unsigned int)file.gcount());
        return data;
    }

    //----------------------------------------------------------------------------------------
    vector<char> ReadFile(char const* filename)
    {
        fstream file(filename, fstream::binary | fstream::in);

        file.seekg(fstream::end);
        size_t bytes = (size_t)file.tellg();
        file.seekg(fstream::beg);

        return ReadFileData(file, bytes);
    }
    //----------------------------------------------------------------------------------------
    vector<char> ReadFile(char const* filename, size_t bytes)
    {
        return ReadFileData(fstream(filename, fstream::binary | fstream::in), bytes);
    }
}