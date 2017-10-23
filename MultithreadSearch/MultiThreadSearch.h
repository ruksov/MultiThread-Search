#pragma once
#include <future>
#include <atomic>
#include <iterator>
#include "ThreadSafeQueue.h"

using FileList = std::vector<std::wstring>;
using FutureList = std::vector<std::future<FileList>>;

class MultiThreadSearch
{
public:
    MultiThreadSearch(int threadCount = std::thread::hardware_concurrency());

    FileList Search(const std::wstring& directory, const std::wstring& mask);

private:
    FileList Worker(const std::wstring& mask);
    FileList GetMatchList(const fs::path& dirPath, const std::wstring& mask);


private:
    int m_maxThreads;
    
    std::atomic_bool m_done;
    std::mutex m_lock;
    std::condition_variable m_cond;
    ThreadSafeQueue<fs::path> m_dirQueue;
};