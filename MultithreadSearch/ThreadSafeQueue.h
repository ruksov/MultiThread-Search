#pragma once
#include <queue>
#include <memory>
#include <mutex>

template<typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue()
    {
    }

    ThreadSafeQueue(const ThreadSafeQueue& other)
    {
        std::lock_guard<std::mutex> lock(other.m_lock);
        m_data = other.m_data;
    }
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    void Push(const T& value)
    {
        std::lock_guard<std::mutex> lock(m_lock);
        m_data.push(std::move(value));
    }

    bool TryPop(T& value)
    {
        std::lock_guard<std::mutex> lock(m_lock);
        if (m_data.empty())
        {
            return false;
        }

        value = m_data.front();
        m_data.pop();

        return true;
    }

    bool Empty() const
    {
        std::lock_guard<std::mutex> lock(m_lock);
        return m_data.empty();
    }

private:
    mutable std::mutex m_lock;
    std::queue<T> m_data;
};
