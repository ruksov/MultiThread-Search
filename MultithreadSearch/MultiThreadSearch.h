#pragma once
#include <future>
#include "ThreadSafeQueue.h"

namespace fs = std::experimental::filesystem;
using FileList = std::vector<std::wstring>;
using FutureList = std::vector<std::future<FileList>>;

class MultiThreadSearch
{
public:
    MultiThreadSearch() : m_threadCount(0)
    {
    }

    void Worker(const std::wstring& directory, const std::wstring& mask, FileList& result)
    {
        fs::path dirPath(directory);

        for (auto& el : fs::recursive_directory_iterator(dirPath))
        {
            auto elPath = el.path();

            if (fs::is_directory(elPath))
            {
                m_directoryQueue.Push(elPath.filename());
            }
            else if (PathMatchSpec(elPath.filename().c_str(), mask.c_str()))
            {
                std::lock_guard<std::mutex> lock(m_lock);
                result.push_back(elPath.filename());
            }
        }
    }

    FileList Search(const std::wstring& directory, const std::wstring& mask)
    {
        FileList result;

        std::async(std::launch::async, &MultiThreadSearch::Worker, this, directory, mask, result);
    }

private:
    const unsigned int m_maxThreads = std::thread::hardware_concurrency();
    unsigned int m_threadCount;
    
    std::mutex m_lock;
    std::condition_variable m_cond;
    TheadSafeQueue<std::wstring> m_directoryQueue;
};