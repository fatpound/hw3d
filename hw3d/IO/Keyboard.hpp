#pragma once

#include <FatNamespaces.hpp>

#include "KeyEvent.hpp"

#include <queue>
#include <bitset>
#include <atomic>
#include <optional>

namespace fatpound::win32
{
    class WindowEx;
}

namespace fatpound::io
{
    class Keyboard final
    {
        friend FATSPACE_WIN32::WindowEx;

    public:
        using keycode_t = decltype(KeyEvent::code);


    public:
        explicit Keyboard() = default;
        explicit Keyboard(const Keyboard& src) = delete;
        explicit Keyboard(Keyboard&& src) = delete;

        auto operator = (const Keyboard& src) -> Keyboard& = delete;
        auto operator = (Keyboard&& src)      -> Keyboard& = delete;
        ~Keyboard() noexcept = default;


    public:
        auto GetEvent() noexcept -> std::optional<KeyEvent>;
        auto GetChar()  noexcept -> std::optional<unsigned char>;

        auto KeyIsPressed(keycode_t code) const noexcept -> bool;
        auto AutoRepeatIsEnabled() const noexcept -> bool;

        auto KeyBufferIsEmpty()  const noexcept -> bool;
        auto CharBufferIsEmpty() const noexcept -> bool;

        void EnableAutoRepeat()  noexcept;
        void DisableAutoRepeat() noexcept;


    protected:


    private:
        template <typename T>
        static void TrimBuffer_(std::queue<T>& buffer) noexcept
        {
            while (buffer.size() > scx_bufferSize_)
            {
                buffer.pop();
            }
        }


    private:
        void OnKeyPressed_(unsigned char keycode);
        void OnKeyReleased_(unsigned char keycode);
        void OnChar_(unsigned char character);

        void ClearKeyStateBitset_() noexcept;


    private:
        static constexpr unsigned int scx_bufferSize_ = 16u;


    private:
        std::queue<KeyEvent> m_key_event_queue_{};
        std::queue<unsigned char> m_char_buffer_{};

        std::bitset<std::numeric_limits<keycode_t>::max()> m_key_states_{};

        std::atomic<bool> m_auto_repeat_enabled_{};
    };
}