// MultithreadSearch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <experimental/filesystem>


#include <queue>
#include <functional>



namespace fs = std::experimental::filesystem;

struct A
{
    void Print(std::wstring fileName)
    {
        std::wcout << fileName << '\n';
    }

    std::queue<std::function<void(std::wstring)>> taslQueue;
};

int main()
{
    A a;
    std::wstring inputData;
    
    std::wcout << "Input directory full name -> ";
    std::getline(std::wcin, inputData);

    fs::path directoryPath(inputData);
    
    std::wcout << "Input file name for search -> ";
    std::getline(std::wcin, inputData);
    
    if (!fs::exists(directoryPath))
    {
        std::wcout << "Directory " << directoryPath << " does not exists\n";
        return 1;
    }

    for (auto& el : fs::recursive_directory_iterator(directoryPath))
    {
        auto elPath(el.path());

        if (!fs::is_directory(elPath) && PathMatchSpec(elPath.filename().c_str(), inputData.c_str()))
        {
            std::wcout << elPath << '\n';
        }
    }

    return 0;
}

