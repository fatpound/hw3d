#pragma once

#include <queue>
#include <bitset>

class Keyboard
{
    friend class Window;

private:
    template <typename T>
    static void TrimBuffer(std::queue<T>& buffer) noexcept;

    void OnKeyPressed( unsigned char keycode ) noexcept;
    void OnKeyReleased( unsigned char keycode ) noexcept;
    void OnChar( char character ) noexcept;
    void ClearState() noexcept;

    static constexpr unsigned int nKeys = 256u;
    static constexpr unsigned int bufferSize = 16u;

    std::bitset<nKeys> keystates;

    bool autoRepeatEnabled = false;


protected:


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

        Event()
            :
            type( Type::Invalid ),
            code( 0u )
        {}
        Event( Type type, unsigned char code ) noexcept
            :
            type( type ),
            code( code)
        {

        }

    private:
        Type type;
        unsigned char code;

    protected:

    
    };

    Keyboard() = default;
    Keyboard( const Keyboard& src ) = delete;
    Keyboard& operator = ( const Keyboard& src ) = delete;

    Event ReadKey() noexcept;
    bool KeyIsEmpty() const noexcept;
    bool KeyIsPressed( unsigned char keycode ) const noexcept;
    void FlushKey() noexcept;

    char ReadChar() noexcept;
    bool CharIsEmpty() const noexcept;
    void FlushChar() noexcept;
    void Flush() noexcept;
    
    bool AutoRepeatIsEnabled() const noexcept;
    void EnableAutoRepeat() noexcept;
    void DisableAutoRepeat() noexcept;
};