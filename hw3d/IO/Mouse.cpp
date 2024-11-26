#include "Mouse.hpp"

#include <FatWin32.hpp>

namespace fatpound::io
{
    auto Mouse::GetEvent() noexcept -> std::optional<MouseEvent>
    {
        if (m_event_buffer_.empty())
        {
            return std::nullopt;
        }

        auto mouseE = m_event_buffer_.front();
        m_event_buffer_.pop();

        return mouseE;
    }

    auto Mouse::GetPos() const noexcept -> std::pair<pos_t, pos_t>
    {
        return { m_pos_x_, m_pos_y_ };
    }

    auto Mouse::GetPosX() const noexcept -> pos_t
    {
        return m_pos_x_;
    }
    auto Mouse::GetPosY() const noexcept -> pos_t
    {
        return m_pos_y_;
    }

    auto Mouse::EventBufferIsEmpty() const noexcept -> bool
    {
        return m_event_buffer_.empty();
    }

    auto Mouse::IsInWindow() const noexcept -> bool
    {
        return m_is_in_window_;
    }

    auto Mouse::LeftIsPressed() const noexcept -> bool
    {
        return m_left_is_pressed_;
    }
    auto Mouse::RightIsPressed() const noexcept -> bool
    {
        return m_right_is_pressed_;
    }
    auto Mouse::WheelIsPressed() const noexcept -> bool
    {
        return m_wheel_is_pressed_;
    }

    void Mouse::OnMouseMove_(int x, int y)
    {
        m_pos_x_ = x;
        m_pos_y_ = y;

        m_event_buffer_.push(MouseEvent{ .type = MouseEvent::Type::Move, .pos_x = m_pos_x_, .pos_y = m_pos_y_ });

        TrimBuffer_();
    }
    void Mouse::OnMouseEnter_()
    {
        m_is_in_window_ = true;

        m_event_buffer_.push(MouseEvent{ MouseEvent::Type::Enter });

        TrimBuffer_();
    }
    void Mouse::OnMouseLeave_()
    {
        m_is_in_window_ = false;

        m_event_buffer_.push(MouseEvent{ MouseEvent::Type::Leave });

        TrimBuffer_();
    }

    void Mouse::OnLeftPressed_()
    {
        m_left_is_pressed_ = true;

        m_event_buffer_.push(MouseEvent{ .type = MouseEvent::Type::LPress, .pos_x = m_pos_x_, .pos_y = m_pos_y_ });

        TrimBuffer_();
    }
    void Mouse::OnLeftReleased_()
    {
        m_left_is_pressed_ = false;

        m_event_buffer_.push(MouseEvent{ MouseEvent::Type::LRelease });

        TrimBuffer_();
    }
    void Mouse::OnRightPressed_()
    {
        m_right_is_pressed_ = true;

        m_event_buffer_.push(MouseEvent{ MouseEvent::Type::RPress });

        TrimBuffer_();
    }
    void Mouse::OnRightReleased_()
    {
        m_right_is_pressed_ = false;

        m_event_buffer_.push(MouseEvent{ MouseEvent::Type::RRelease });

        TrimBuffer_();
    }
    void Mouse::OnWheelPressed_()
    {
        m_wheel_is_pressed_ = true;

        m_event_buffer_.push(MouseEvent{ MouseEvent::Type::WheelPress });

        TrimBuffer_();
    }
    void Mouse::OnWheelReleased_()
    {
        m_wheel_is_pressed_ = false;

        m_event_buffer_.push(MouseEvent{ MouseEvent::Type::WheelRelease });

        TrimBuffer_();
    }
    void Mouse::OnWheelUp_()
    {
        m_event_buffer_.push(MouseEvent{ MouseEvent::Type::WheelUp });

        TrimBuffer_();
    }
    void Mouse::OnWheelDown_()
    {
        m_event_buffer_.push(MouseEvent{ MouseEvent::Type::WheelDown });

        TrimBuffer_();
    }
    void Mouse::OnWheelDelta_(int delta)
    {
        m_wheel_delta_carry_ += delta;

        while (m_wheel_delta_carry_ >= WHEEL_DELTA)
        {
            m_wheel_delta_carry_ -= WHEEL_DELTA;

            OnWheelUp_();
        }

        while (m_wheel_delta_carry_ <= -WHEEL_DELTA)
        {
            m_wheel_delta_carry_ += WHEEL_DELTA;

            OnWheelDown_();
        }
    }

    void Mouse::TrimBuffer_() noexcept
    {
        while (m_event_buffer_.size() > scx_bufferSize_)
        {
            m_event_buffer_.pop();
        }
    }
}