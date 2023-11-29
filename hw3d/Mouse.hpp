#pragma once

#include <queue>

class Mouse
{
    friend class Window;

public:
    Mouse() = default;
    ~Mouse() = default;
    Mouse(const Mouse& src) = delete;
    Mouse(Mouse&& src) = delete;
    Mouse& operator = (const Mouse& src) = delete;
    Mouse& operator = (Mouse&& src) = delete;


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
            Enter,
            Move,
            Leave,
            Invalid
        };

    public:
        Event() noexcept;

        Event(Type type, const Mouse& parent) noexcept;

    public:
        std::pair<int, int> GetPos() const noexcept;

        Type GetType() const noexcept;

        int GetPosX() const noexcept;
        int GetPosY() const noexcept;

        bool IsValid() const noexcept;
        bool LeftIsPressed() const noexcept;
        bool RightIsPressed() const noexcept;
        bool WheelIsPressed() const noexcept;

    protected:

    private:
        Type type;

        int x;
        int y;

        bool leftIsPressed;
        bool rightIsPressed;
        bool wheelIsPressed;
    };


public:
    std::pair<int, int> GetPos() const noexcept;

    Event ReadFromBuffer() noexcept;

    int GetPosX() const noexcept;
    int GetPosY() const noexcept;

    bool IsInWindow() const noexcept;
    bool LeftIsPressed() const noexcept;
    bool RightIsPressed() const noexcept;
    bool WheelIsPressed() const noexcept;
    bool BufferIsEmpty() const noexcept;

    void FlushBuffer() noexcept;


protected:


private:
    void OnMouseMove(int newx, int newy) noexcept;
    void OnMouseEnter() noexcept;
    void OnMouseLeave() noexcept;
    void OnLeftPressed() noexcept;
    void OnLeftReleased() noexcept;
    void OnRightPressed() noexcept;
    void OnRightReleased() noexcept;
    void OnWheelPressed() noexcept;
    void OnWheelReleased() noexcept;
    void OnWheelUp() noexcept;
    void OnWheelDown() noexcept;
    void OnWheelDelta(int delta) noexcept;

    void TrimBuffer() noexcept;


private:
    static constexpr unsigned int bufferSize = 16u;

    std::queue<Event> buffer;

    int x = 0;
    int y = 0;
    int wheelDeltaCarry = 0;

    bool isInWindow = false;
    bool leftIsPressed = false;
    bool rightIsPressed = false;
    bool wheelIsPressed = false;
};