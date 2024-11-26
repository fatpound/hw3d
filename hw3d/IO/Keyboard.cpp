#include "Keyboard.hpp"

namespace fatpound::io
{
    auto Keyboard::GetEvent() noexcept -> std::optional<KeyEvent>
    {
        if (m_key_event_queue_.empty())
        {
            return std::nullopt;
        }

        auto keyE = m_key_event_queue_.front();

        m_key_event_queue_.pop();

        return keyE;
    }
    auto Keyboard::GetChar() noexcept -> std::optional<unsigned char>
    {
        return std::optional<unsigned char>();
    }

    auto Keyboard::KeyIsPressed(keycode_t code) const noexcept -> bool
    {
        return m_key_states_[code];
    }
    auto Keyboard::AutoRepeatIsEnabled() const noexcept -> bool
    {
        return m_auto_repeat_enabled_;
    }

    auto Keyboard::KeyBufferIsEmpty() const noexcept -> bool
    {
        return false;
    }
    auto Keyboard::CharBufferIsEmpty() const noexcept -> bool
    {
        return false;
    }

    void Keyboard::EnableAutoRepeat() noexcept
    {
        m_auto_repeat_enabled_ = true;
    }
    void Keyboard::DisableAutoRepeat() noexcept
    {
        m_auto_repeat_enabled_ = false;
    }

    void Keyboard::OnKeyPressed_(unsigned char keycode)
    {
        m_key_states_[keycode] = true;

        m_key_event_queue_.push(KeyEvent{ KeyEvent::Type::Press, keycode });

        TrimBuffer_(m_key_event_queue_);
    }
    void Keyboard::OnKeyReleased_(unsigned char keycode)
    {
        m_key_states_[keycode] = false;

        m_key_event_queue_.push(KeyEvent{ KeyEvent::Type::Release, keycode });

        TrimBuffer_(m_key_event_queue_);
    }
    void Keyboard::OnChar_(unsigned char character)
    {
        m_char_buffer_.push(character);

        TrimBuffer_(m_char_buffer_);
    }

    void Keyboard::ClearKeyStateBitset_() noexcept
    {
        m_key_states_.reset();
    }
}