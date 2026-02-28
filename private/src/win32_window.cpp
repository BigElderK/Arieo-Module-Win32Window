
#include "base/prerequisites.h"
#include "core/core.h"

#include "win32_window.h"
#include <windows.h>

namespace Arieo
{
    LRESULT CALLBACK Win32Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
    {
        switch (uMsg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

                EndPaint(hwnd, &ps);
            }
            return 0;

        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    void Win32WindowManager::initialize()
    {

    }

    void Win32WindowManager::finalize()
    {
        for(Base::Interop::RawRef<Interface::Window::IWindow> window : std::unordered_set(m_win32_window_set))
        {
            destroyWindow(window);
        }
        m_win32_window_set.clear();
    }

    void* Win32WindowManager::getDisplay()
    {
        // In Win32, the "display" concept isn't exactly the same as X11,
        // but we can return the HINSTANCE of the current module
        return ::GetModuleHandle(nullptr);
    }
    

    Base::Interop::RawRef<Interface::Window::IWindow> Win32WindowManager::createWindow(std::uint16_t pos_x, std::uint16_t pos_y, std::uint16_t width, std::uint16_t height)
    {
        WNDCLASSEXA win_class = { 0 };
        win_class.cbSize = sizeof(WNDCLASSEXA);
        win_class.style = CS_HREDRAW | CS_VREDRAW;
        win_class.lpfnWndProc = Win32Window::WindowProc;
        win_class.cbClsExtra = 0;
        win_class.cbWndExtra = 0;
        win_class.hInstance = ::GetModuleHandle(NULL);
        win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
        win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        win_class.lpszMenuName = NULL;
        win_class.lpszClassName = "Window";
        win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

        if(!RegisterClassExA(&win_class))
        {
            Core::Logger::error("Failed to register class when create window: {}", ::GetLastError());
            return nullptr;
        }
        
        RECT windowRect = { pos_x, pos_y, width, height };
        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

        // Create the window and store a handle to it.
        HWND m_hwnd = ::CreateWindowA(
            win_class.lpszClassName,
            "",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr,        // We have no parent window.
            nullptr,        // We aren't using menus.
            nullptr,
            nullptr);

        if(!m_hwnd)
        {
            Core::Logger::error("Failed to create window");
            return nullptr;
        }

        ::ShowWindow(m_hwnd, SW_SHOW);

        Base::Interop::RawRef<Interface::Window::IWindow> win32_window = Base::Interop::RawRef<Interface::Window::IWindow>::createAs<Win32Window>(std::move(m_hwnd));
        m_win32_window_set.insert(win32_window);


        return win32_window;
    }

    Base::Interop::RawRef<Interface::Window::IWindow> Win32WindowManager::getMainWindow()
    {
        Core::Logger::error("Win32WindowManager::getMainWindow() not implemented, using createWindow instead");
        return nullptr;
    }

    void Win32WindowManager::destroyWindow(Base::Interop::RawRef<Interface::Window::IWindow> window)
    {
        Win32Window* win32_window = window.castToInstance<Win32Window>(); 
        ::DestroyWindow(win32_window->m_hwnd);

        m_win32_window_set.erase(window);
        Base::Interop::RawRef<Interface::Window::IWindow>::destroyAs<Win32Window>(std::move(window));
    }

    void Win32WindowManager::onInitialize()
    {
        
    }

    void Win32WindowManager::onTick()
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void Win32WindowManager::onDeinitialize()
    {

    }


}




