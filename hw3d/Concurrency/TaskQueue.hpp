#pragma once

#include <deque>
#include <functional>
#include <future>
#include <mutex>

namespace fatpound::concurrency
{
    class TaskQueue final
    {
        using WrappedTask = std::move_only_function<void()>;

    public:
        explicit TaskQueue() noexcept = default;
        explicit TaskQueue(const TaskQueue& src) = delete;
        explicit TaskQueue(TaskQueue&& src) = delete;

        auto operator = (const TaskQueue& src) -> TaskQueue& = delete;
        auto operator = (TaskQueue&& src)      -> TaskQueue& = delete;
        ~TaskQueue() noexcept = default;


    public:
        void ExecuteFirstAndPopOff();


    public:
        template <
            typename F,
            typename... Args
        >
        requires(std::invocable<F, Args...>)
        auto Push(F&& function, Args&&... args) -> auto
        {
            using T = std::invoke_result_t<F, Args...>;

            auto pkgTask = std::packaged_task<T(Args...)>{ std::bind(std::forward<F>(function), std::forward<Args>(args)...) };
            auto future  = pkgTask.get_future();

            Push_([&, task = std::move(pkgTask)]() mutable -> void { static_cast<void>(task(args...)); });

            return future;
        }


    protected:


    private:
        void Push_(WrappedTask wtask);


    private:
        std::deque<WrappedTask> m_tasks_;

        std::mutex m_mtx_;
    };
}