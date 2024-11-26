#include "TaskQueue.hpp"

namespace fatpound::concurrency
{
    void TaskQueue::ExecuteFirstAndPopOff()
    {
        WrappedTask wtask{};

        {
            std::lock_guard lck{ m_mtx_ };

            wtask = std::move(m_tasks_.front());

            m_tasks_.pop_front();
        }

        wtask();
    }

    void TaskQueue::Push_(WrappedTask wtask)
    {
        std::lock_guard lck{ m_mtx_ };

        m_tasks_.push_back(std::move(wtask));
    }
}