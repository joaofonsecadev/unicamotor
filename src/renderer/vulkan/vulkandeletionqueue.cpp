#include "vulkandeletionqueue.h"

#include <tracy/Tracy.hpp>

void VulkanDeletionQueue::PushDeletionFunction(const std::function<void()>& deletion_function)
{
    m_deletion_queue.push_back(deletion_function);
}

void VulkanDeletionQueue::FlushDeletionQueue()
{
    ZoneScoped;

    for (auto it = m_deletion_queue.rbegin(); it != m_deletion_queue.rend(); ++it)
    {
        (*it)();
    }

    m_deletion_queue.clear();
}
