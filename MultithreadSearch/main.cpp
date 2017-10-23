// MultithreadSearch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



#include "MultiThreadSearch.h"




FileList GetList(std::wstring& dirStr, std::wstring& mask)
{
    fs::path dirPath(dirStr);
    FileList result;

    if (!fs::exists(dirPath))
    {
        std::wcout << "Directory " << dirPath << " does not exists\n";
        return FileList();
    }

    for (auto& el : fs::recursive_directory_iterator(dirPath))
    {
        auto elPath(el.path());

        if (!fs::is_directory(elPath) && PathMatchSpec(elPath.filename().c_str(), mask.c_str()))
        {
            result.push_back(elPath);
        }
    }

    return result;
}

int main()
{
    std::wstring dirStr, mask;
    
    std::wcout << "Input directory full name -> ";
    std::getline(std::wcin, dirStr);
  
    std::wcout << "Input file name for search -> ";
    std::getline(std::wcin, mask);
    
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;

    auto t0 = Time::now();
    auto res = GetList(dirStr, mask);
    auto t1 = Time::now();

    fsec fs = t1 - t0;
    ms d = std::chrono::duration_cast<ms>(fs);

    std::wcout << "Duration " << d.count() << "ms\n";
    std::wcout << "Found " << res.size() << " items\n";

    std::wcout << "\n\n\n\n\nMulti thread result\n\n\n\n\n";

    MultiThreadSearch search;

    t0 = Time::now();
    res = search.Search(dirStr, mask);
    t1 = Time::now();

    fs = t1 - t0;
    d = std::chrono::duration_cast<ms>(fs);

    std::wcout << "Duration " << d.count() << "ms\n";
    std::wcout << "Found " << res.size() << " items\n";

    return 0;
}

