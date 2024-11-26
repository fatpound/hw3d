#pragma once

#include <FatNamespaces.hpp>

#include "MouseEvent.hpp"

#include <queue>
#include <optional>

namespace fatpound::win32
{
    class WindowEx;
}

namespace fatpound::io
{
    class Mouse final
    {
        friend FATSPACE_WIN32::WindowEx;

    public:
        using pos_t = decltype(MouseEvent::pos_x);


    public:
        explicit Mouse() = default;
        explicit Mouse(const Mouse& src) = delete;
        explicit Mouse(Mouse&& src) = delete;

        auto operator = (const Mouse& src) -> Mouse& = delete;
        auto operator = (Mouse&& src)      -> Mouse& = delete;
        ~Mouse() noexcept = default;


    public:
        auto GetEvent() noexcept -> std::optional<MouseEvent>;

        auto GetPos() const noexcept -> std::pair<pos_t, pos_t>;

        auto GetPosX() const noexcept -> pos_t;
        auto GetPosY() const noexcept -> pos_t;

        auto EventBufferIsEmpty() const noexcept -> bool;

        auto IsInWindow() const noexcept -> bool;

        auto LeftIsPressed()  const noexcept -> bool;
        auto RightIsPressed() const noexcept -> bool;
        auto WheelIsPressed() const noexcept -> bool;


    protected:


    private:
        void OnMouseMove_(int x, int y);
        void OnMouseEnter_();
        void OnMouseLeave_();

        void OnLeftPressed_();
        void OnLeftReleased_();
        void OnRightPressed_();
        void OnRightReleased_();
        void OnWheelPressed_();
        void OnWheelReleased_();
        void OnWheelUp_();
        void OnWheelDown_();
        void OnWheelDelta_(int delta);

        void TrimBuffer_() noexcept;


    private:
        static constexpr auto scx_bufferSize_ = 16u;


    private:
        std::queue<MouseEvent> m_event_buffer_;

        pos_t m_pos_x_{};
        pos_t m_pos_y_{};

        pos_t m_wheel_delta_carry_{};

        bool m_is_in_window_{};

        bool m_left_is_pressed_{};
        bool m_right_is_pressed_{};
        bool m_wheel_is_pressed_{};
    };
}