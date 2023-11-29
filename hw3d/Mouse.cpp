#include "FatWin.hpp"
#include "Mouse.hpp"

std::pair<int, int> Mouse::GetPos() const noexcept
{
    return { x, y };
}

Mouse::Event Mouse::ReadFromBuffer() noexcept
{
    if ( buffer.size() > 0u )
    {
        Mouse::Event e = buffer.front();
        buffer.pop();

        return e;
    }

    return Mouse::Event();
}

int Mouse::GetPosX() const noexcept
{
    return x;
}
int Mouse::GetPosY() const noexcept
{
    return y;
}

bool Mouse::IsInWindow() const noexcept
{
    return isInWindow;
}
bool Mouse::LeftIsPressed() const noexcept
{
    return leftIsPressed;
}
bool Mouse::RightIsPressed() const noexcept
{
    return rightIsPressed;
}
bool Mouse::WheelIsPressed() const noexcept
{
    return wheelIsPressed;
}
bool Mouse::BufferIsEmpty() const noexcept
{
    return buffer.empty();
}

void Mouse::FlushBuffer() noexcept
{
    buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int newx, int newy) noexcept
{
    x = newx;
    y = newy;
    
    buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    TrimBuffer();
}
void Mouse::OnMouseEnter() noexcept
{
    isInWindow = true;

    buffer.push( Mouse::Event( Mouse::Event::Type::Enter, *this ) );
    TrimBuffer();
}
void Mouse::OnMouseLeave() noexcept
{
    isInWindow = false;

    buffer.push( Mouse::Event( Mouse::Event::Type::Leave, *this ) );
    TrimBuffer();
}
void Mouse::OnLeftPressed() noexcept
{
    leftIsPressed = true;

    buffer.push( Mouse::Event( Mouse::Event::Type::LPress, *this ) );
    TrimBuffer();
}
void Mouse::OnLeftReleased() noexcept
{
    leftIsPressed = false;

    buffer.push( Mouse::Event( Mouse::Event::Type::LRelease, *this ) );
    TrimBuffer();
}
void Mouse::OnRightPressed() noexcept
{
    rightIsPressed = true;

    buffer.push( Mouse::Event( Mouse::Event::Type::RPress, *this ) );
    TrimBuffer();
}
void Mouse::OnRightReleased() noexcept
{
    rightIsPressed = false;

    buffer.push( Mouse::Event( Mouse::Event::Type::RRelease, *this ) );
    TrimBuffer();
}
void Mouse::OnWheelPressed() noexcept
{
    wheelIsPressed = true;

    buffer.push( Mouse::Event( Mouse::Event::Type::WheelPress, *this ) );
    TrimBuffer();
}
void Mouse::OnWheelReleased() noexcept
{
    wheelIsPressed = false;

    buffer.push( Mouse::Event( Mouse::Event::Type::WheelRelease, *this ) );
    TrimBuffer();
}
void Mouse::OnWheelUp() noexcept
{
    buffer.push( Mouse::Event( Mouse::Event::Type::WheelUp, *this ) );
    TrimBuffer();
}
void Mouse::OnWheelDown() noexcept
{
    buffer.push( Mouse::Event( Mouse::Event::Type::WheelDown, *this ) );
    TrimBuffer();
}
void Mouse::OnWheelDelta(int delta) noexcept
{
    wheelDeltaCarry += delta;

    while (wheelDeltaCarry >= WHEEL_DELTA)
    {
        wheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp();
    }

    while (wheelDeltaCarry <= -WHEEL_DELTA)
    {
        wheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown();
    }
}

void Mouse::TrimBuffer() noexcept
{
    while ( buffer.size() > bufferSize )
    {
        buffer.pop();
    }
}

// Event

Mouse::Event::Event() noexcept
    :
    type(Type::Invalid),
    leftIsPressed(false),
    rightIsPressed(false),
    wheelIsPressed(false),
    x(0),
    y(0)
{

}

Mouse::Event::Event(Type type, const Mouse& parent) noexcept
    :
    type(type),
    leftIsPressed(parent.leftIsPressed),
    rightIsPressed(parent.rightIsPressed),
    wheelIsPressed(parent.wheelIsPressed),
    x(parent.x),
    y(parent.y)
{

}

std::pair<int, int> Mouse::Event::GetPos() const noexcept
{
    return { x, y };
}

Mouse::Event::Type Mouse::Event::GetType() const noexcept
{
    return type;
}

int Mouse::Event::GetPosX() const noexcept
{
    return x;
}
int Mouse::Event::GetPosY() const noexcept
{
    return y;
}

bool Mouse::Event::IsValid() const noexcept
{
    return type != Event::Type::Invalid;
}
bool Mouse::Event::LeftIsPressed() const noexcept
{
    return leftIsPressed;
}
bool Mouse::Event::RightIsPressed() const noexcept
{
    return rightIsPressed;
}
bool Mouse::Event::WheelIsPressed() const noexcept
{
    return wheelIsPressed;
}