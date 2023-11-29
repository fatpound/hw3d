#pragma once

#include <bitset>
#include <queue>

class Keyboard
{
    friend class Window;
    
public:
    Keyboard() = default;
    ~Keyboard() = default;
    Keyboard(const Keyboard& src) = delete;
    Keyboard(Keyboard&& src) = delete;
    Keyboard& operator = (const Keyboard& src) = delete;
    Keyboard& operator = (Keyboard&& src) = delete;


public:
    class Event
    {
    public:
        enum class Type
        {
            Press,
            Release,
            Invalid
        };

    public:
        Event() noexcept;

        Event(Type in_type, unsigned char in_code) noexcept;

        unsigned char GetCode() const noexcept;

        bool IsPress() const noexcept;
        bool IsRelease() const noexcept;
        bool IsInvalid() const noexcept;

    protected:

    private:
        Type type;

        unsigned char code;
    };


public:
    Event ReadKeyFromBuffer() noexcept;

    char ReadCharFromBuffer() noexcept;

    bool AutoRepeatIsEnabled() const noexcept;
    bool CharBufferIsEmpty() const noexcept;
    bool KeyBufferIsEmpty() const noexcept;
    bool KeyIsPressed(unsigned char keycode) const noexcept;

    void FlushKeyBuffer() noexcept;
    void FlushCharBuffer() noexcept;
    void FlushBuffers() noexcept;

    void EnableAutoRepeat() noexcept;
    void DisableAutoRepeat() noexcept;


protected:


private:
    template <typename T>
    static void TrimBuffer(std::queue<T>& buffer) noexcept
    {
        while (buffer.size() > bufferSize)
        {
            buffer.pop();
        }
    }

    void OnKeyPressed(unsigned char keycode) noexcept;
    void OnKeyReleased(unsigned char keycode) noexcept;
    void OnChar(char character) noexcept;

    void ClearKeyStateBitset() noexcept;


private:
    static constexpr unsigned int keyCount = 256u;
    static constexpr unsigned int bufferSize = 16u;

    std::bitset<keyCount> keystates;

    std::queue<Event> keybuffer;
    std::queue<char> charbuffer;

    bool autoRepeatEnabled = false;
};