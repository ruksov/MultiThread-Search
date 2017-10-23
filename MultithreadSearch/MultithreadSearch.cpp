#include "stdafx.h"
#include "MultiThreadSearch.h"

MultiThreadSearch::MultiThreadSearch(int threadCount) : m_maxThreads(threadCount)
    , m_done(true)
{
}

FileList MultiThreadSearch::Worker(const std::wstring & mask)
{
    {
        std::unique_lock<std::mutex> lock(m_lock);
        m_cond.wait(lock);
    }

    fs::path dirPath;

    FileList result;

    while (m_dirQueue.TryPop(dirPath))
    {
        FileList buffer = GetMatchList(dirPath, mask);

        std::move(buffer.begin(), buffer.end(), std::back_inserter(result));
    }

    return result;
}

FileList MultiThreadSearch::Search(const std::wstring & directory, const std::wstring & mask)
{
    if (!fs::exists(directory))
    {
        return FileList();
    }

    FutureList futures(m_maxThreads);
    for (int i = 0; i < m_maxThreads; ++i)
    {
        futures[i] = std::async(std::launch::async, &MultiThreadSearch::Worker, this, mask);
    }

    auto result = GetMatchList(directory, mask);
    m_cond.notify_all();

    FileList buffer;
    for (auto& f : futures)
    {
        buffer = f.get();
        std::move(buffer.begin(), buffer.end(), std::back_inserter(result));
    }

    return result;
}

FileList MultiThreadSearch::GetMatchList(const fs::path & dirPath, const std::wstring & mask)
{
    FileList result;

    for (auto& el : fs::directory_iterator(dirPath))
    {
        auto elPath = el.path();

        if (fs::is_directory(elPath))
        {
            m_dirQueue.Push(elPath);
            m_cond.notify_one();
        }
        else if (PathMatchSpec(elPath.filename().c_str(), mask.c_str()))
        {
            result.push_back(elPath);
        }
    }

    return result;
}
