#include <future>
#include <numeric>
#include <gtest/gtest.h>
#include "misc/queue.hpp"

class QueueTest : public ::testing::Test
{
protected:
    QueueTest()
    {
    }
    virtual ~QueueTest() = default;

    Queue<int> queue;
};


template <typename T>
void producer(Queue<T> &queue, std::vector<T> input)
{
    for (auto& item : input)
        queue.enqueue(item);
}


template <typename T>
std::vector<T> consumer(Queue<T> &queue, size_t number)
{
    std::vector<T> output;
    for (size_t i(0); i < number; ++i)
        output.push_back(queue.dequeue());
    return output;
}

TEST_F(QueueTest, AsyncEnqueueDequeue)
{
    std::vector<int> input(64);
    std::iota(input.begin(), input.end(), 0);

    auto fut_prod{std::async(std::launch::async,
        [this, input] { producer(this->queue, input); })};
    auto fut_cons{std::async(std::launch::async,
        [this, input] { return consumer(this->queue, input.size()); })};
    fut_prod.get();
    auto output{fut_cons.get()};
    ASSERT_EQ(input, output);
}
