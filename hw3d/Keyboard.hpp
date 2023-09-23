#pragma once

#include <queue>
#include <bitset>

class Keyboard
{
    friend class Window;

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

        Event() noexcept
            :
            type( Event::Type::Invalid ),
            code( 0u )
        {}
        Event( Event::Type type, unsigned char code ) noexcept
            :
            type( type ),
            code( code )
        {}

        bool IsPress() const noexcept
        {
            return type == Event::Type::Press;
        }
        bool IsRelease() const noexcept
        {
            return type == Event::Type::Release;
        }
        bool IsInvalid() const noexcept
        {
            return type == Event::Type::Invalid;
        }
        unsigned char GetCode() const noexcept
        {
            return code;
        }

    protected:

    private:
        Event::Type type;
        unsigned char code;
    };

    Keyboard() = default;
    Keyboard( const Keyboard& src ) = delete;
    Keyboard& operator = ( const Keyboard& src ) = delete;

    Event ReadKeyFromBuffer() noexcept;
    bool KeyBufferIsEmpty() const noexcept;
    bool KeyIsPressed( unsigned char keycode ) const noexcept;
    void FlushKeyBuffer() noexcept;

    char ReadCharFromBuffer() noexcept;
    bool CharBufferIsEmpty() const noexcept;
    void FlushCharBuffer() noexcept;

    void FlushBuffers() noexcept;

    bool AutoRepeatIsEnabled() const noexcept;
    void EnableAutoRepeat() noexcept;
    void DisableAutoRepeat() noexcept;


protected:


private:
    template <typename T>
    static void TrimBuffer(std::queue<T>& buffer) noexcept;

    void OnKeyPressed( unsigned char keycode ) noexcept;
    void OnKeyReleased( unsigned char keycode ) noexcept;
    void OnChar( char character ) noexcept;
    void ClearKeyStateBitset() noexcept;

    static constexpr unsigned int nKeys = 256u;
    static constexpr unsigned int bufferSize = 16u;

    std::bitset<nKeys> keystates;

    std::queue<Event> keybuffer;
    std::queue<char> charbuffer;

    bool autoRepeatEnabled = false;
};

template<typename T>
void Keyboard::TrimBuffer( std::queue<T>& buffer ) noexcept
{
    while ( buffer.size() > bufferSize )
    {
        buffer.pop();
    }
}