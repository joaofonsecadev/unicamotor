#pragma once

#include <deque>
#include <functional>

class VulkanDeletionQueue
{
public:
    void PushDeletionFunction(const std::function<void()>& deletion_function);
    void FlushDeletionQueue();

private:
    std::deque<std::function<void()>> m_deletion_queue;
};
