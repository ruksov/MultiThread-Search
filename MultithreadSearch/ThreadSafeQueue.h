#pragma once
#include <queue>
#include <memory>
#include <mutex>

template<typename T>
class TheadSafeQueue
{
public:
    ThreadSafeQueue() = default;

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
        m_cond.notify_one();
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

    void WaitAndPop(T& value)
    {
        std::unique_lock<std::mutex> lock(m_lock);
        m_cond.wait(lock, [this] { return !m_data.empty(); });

        value = std::move(m_data.front());
        m_data.pop();
    }

private:
    mutable std::mutex m_lock;
    std::condition_variable m_cond;
    std::queue<T> m_data;
};
