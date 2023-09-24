#include "Keyboard.hpp"

Keyboard::Event Keyboard::ReadKeyFromBuffer() noexcept
{
    if ( keybuffer.size() > 0u )
    {
        Keyboard::Event e = keybuffer.front();
        keybuffer.pop();

        return e;
    }
    
    return Keyboard::Event();
}
bool Keyboard::KeyBufferIsEmpty() const noexcept
{
    return keybuffer.empty();
}
bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
    return keystates[keycode];
}
void Keyboard::FlushKeyBuffer() noexcept
{
    keybuffer = std::queue<Event>();
}

char Keyboard::ReadCharFromBuffer() noexcept
{
    if ( charbuffer.size() > 0u )
    {
        unsigned char charcode = charbuffer.front();
        charbuffer.pop();

        return charcode;
    }
    else
    {
        return 0;
    }
}
bool Keyboard::CharBufferIsEmpty() const noexcept
{
    return charbuffer.empty();
}
void Keyboard::FlushCharBuffer() noexcept
{
    charbuffer = std::queue<char>();
}

void Keyboard::FlushBuffers() noexcept
{
    FlushKeyBuffer();
    FlushCharBuffer();
}

bool Keyboard::AutoRepeatIsEnabled() const noexcept
{
    return autoRepeatEnabled;
}
void Keyboard::EnableAutoRepeat() noexcept
{
    autoRepeatEnabled = true;
}
void Keyboard::DisableAutoRepeat() noexcept
{
    autoRepeatEnabled = false;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    keystates[keycode] = true;
    keybuffer.push( Keyboard::Event( Keyboard::Event::Type::Press, keycode ) );

    TrimBuffer( keybuffer );
}
void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
    keystates[keycode] = false;
    keybuffer.push( Keyboard::Event( Keyboard::Event::Type::Release, keycode ) );

    TrimBuffer(keybuffer);
}
void Keyboard::OnChar(char character) noexcept
{
    charbuffer.push( character );

    TrimBuffer( charbuffer );
}
void Keyboard::ClearKeyStateBitset() noexcept
{
    keystates.reset();
}