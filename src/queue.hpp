#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class Queue
{
public:

    void enqueue(const T& item)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(item);
        cv_.notify_one();
    }

    T dequeue()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            cv_.wait(lock, [this] { return !this->queue_.empty(); });
        }
        auto item = queue_.front();
        queue_.pop();
        lock.unlock();
        return item;
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

#endif // QUEUE_HPP
