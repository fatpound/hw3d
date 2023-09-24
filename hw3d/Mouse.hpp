#pragma once

#include <queue>

class Mouse
{
    friend class Window;

public:
    class Event
    {
    public:
        enum class Type
        {
            LPress,
            LRelease,
            RPress,
            RRelease,
            WheelPress,
            WheelRelease,
            WheelUp,
            WheelDown,
            Move,
            Invalid
        };

        Event() noexcept
            :
            type ( Type::Invalid ),
            leftIsPressed( false ),
            rightIsPressed( false ),
            wheelIsPressed( false ),
            x( 0 ),
            y( 0 )
        {}
        Event( Type type, const Mouse& parent ) noexcept
            :
            type( type ),
            leftIsPressed( parent.leftIsPressed ),
            rightIsPressed( parent.rightIsPressed ),
            wheelIsPressed( parent.wheelIsPressed ),
            x( parent.x ),
            y( parent.y )
        {}

        std::pair<int, int> GetPos() const noexcept
        {
            return { x, y };
        }
        Type GetType() const noexcept
        {
            return type;
        }
        int GetPosX() const noexcept
        {
            return x;
        }
        int GetPosY() const noexcept
        {
            return y;
        }
        bool IsValid() const noexcept
        {
            return type != Event::Type::Invalid;
        }
        bool LeftIsPressed() const noexcept
        {
            return leftIsPressed;
        }
        bool RightIsPressed() const noexcept
        {
            return rightIsPressed;
        }
        bool WheelIsPressed() const
        {
            return wheelIsPressed;
        }

    protected:

    private:
        Type type;
        int x;
        int y;
        bool leftIsPressed;
        bool rightIsPressed;
        bool wheelIsPressed;
    };

    Mouse() = default;
    Mouse( const Mouse& src) = delete;
    Mouse& operator = ( const Mouse& src ) = delete;

    std::pair<int, int> GetPos() const noexcept;
    Event ReadFromBuffer() noexcept;
    int GetPosX() const noexcept;
    int GetPosY() const noexcept;

    bool BufferIsEmpty() const noexcept;
    bool LeftIsPressed() const noexcept;
    bool RightIsPressed() const noexcept;
    bool WheelIsPressed() const noexcept;

    void FlushBuffer() noexcept;


protected:


private:
    void OnMouseMove( int newx, int newy ) noexcept;
    void OnLeftPressed() noexcept;
    void OnLeftReleased() noexcept;
    void OnRightPressed() noexcept;
    void OnRightReleased() noexcept;
    void OnWheelPressed() noexcept;
    void OnWheelReleased() noexcept;
    void OnWheelUp() noexcept;
    void OnWheelDown() noexcept;

    void TrimBuffer() noexcept;

    static constexpr unsigned int bufferSize = 16u;
    std::queue<Event> buffer;
    int x;
    int y;
    bool leftIsPressed = false;
    bool rightIsPressed = false;
    bool wheelIsPressed = false;
};