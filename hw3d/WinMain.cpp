#include "Window.hpp"
#include <sstream>

int CALLBACK WinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPSTR     lpCmdLine,
    _In_     int       nShowCmd)
{
    try
    {
        Window wnd(640, 480, "hw3dfat");

        MSG msg;
        BOOL gResult;
        while ( ( gResult = GetMessage( &msg, nullptr, 0, 0 ) ) > 0)
        {
            TranslateMessage( &msg );
            DispatchMessage ( &msg );

            while ( ! wnd.mouse.BufferIsEmpty() )
            {
                const Mouse::Event e = wnd.mouse.ReadFromBuffer();

                if ( e.GetType() == Mouse::Event::Type::Move )
                {
                    std::ostringstream oss;

                    oss << "Mouse Position : (" << e.GetPosX() << "," << e.GetPosY() << ")";

                    wnd.SetTitle( oss.str() );
                }
            }
        }
        
        if ( gResult == -1 )
        {
            return -1;
        }

        return msg.wParam;
    }
    catch ( const FatException& e )
    {
        MessageBox( nullptr, e.what(), e.GetType(), MB_OK | MB_ICONERROR);
    }
    catch ( const std::exception& e )
    {
        MessageBox( nullptr, e.what(), "STD Exception", MB_OK | MB_ICONERROR);
    }
    catch ( ... )
    {
        MessageBox( nullptr, "No Details Available", "Standard Exception", MB_OK | MB_ICONERROR);
    }

    return -1;
}