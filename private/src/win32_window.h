#pragma once
#include "interface/window/window.h"
#include "base/interface/interface.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

#include <windows.h>
#include <string>
#include <wrl.h>
#include <shellapi.h>
namespace Arieo
{
    class Win32Window final
        : public Interface::Window::IWindow
    {
    public:
        Win32Window(Base::Interface<Interface::Window::IWindowManager> window_manager, HWND&& hwnd)
            : m_window_manager(window_manager), m_hwnd(std::move(hwnd))
        {

        }

        std::uint64_t getWindowHandle() override
        {
            return reinterpret_cast<std::uint64_t>(m_hwnd);
        }

        Base::Math::Rect<std::uint32_t> getWindowRect() override
        {
            ::RECT w_rect;
            ::GetWindowRect(m_hwnd, &w_rect);

            return Base::Math::Rect<std::uint32_t>(
                w_rect.left,
                w_rect.top,
                w_rect.right - w_rect.left,
                w_rect.bottom - w_rect.top
            );
        }
       
        Base::Math::Vector<std::uint32_t, 2> getFramebufferSize() override
        {
            ::RECT w_rect;
            ::GetWindowRect(m_hwnd, &w_rect);
           
            UINT dpi = ::GetDpiForWindow(m_hwnd);
            float scaling = dpi / 96.0f;

            int width = static_cast<int>(w_rect.right * scaling);
            int height = static_cast<int>(w_rect.bottom * scaling);
            
            return Base::Math::Vector<std::uint32_t, 2>(width, height);
        }        

        bool isClosed() override
        {
            return ::IsWindow(m_hwnd) == false;
        }

        Base::Interface<Interface::Window::IWindowManager> getWindowManager() override
        {
            return m_window_manager;
        }

        Base::StringID getWindowPlatform() override
        {
            return Base::MakeStringID("win32");
        } 
    private:
        friend class Win32WindowManager;
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        Base::Interface<Interface::Window::IWindowManager> m_window_manager;
        HWND m_hwnd;
    };

    class Win32WindowManager final
        : public Interface::Window::IWindowManager
    {
    protected:
        std::unordered_set<Base::Interface<Interface::Window::IWindow>> m_win32_window_set;
    public:
        void initialize();
        void finalize();

        void* getDisplay() override;

        Base::Interface<Interface::Window::IWindow> createWindow(std::uint16_t pos_x, std::uint16_t pos_y, std::uint16_t width, std::uint16_t height) override;
        void destroyWindow(Base::Interface<Interface::Window::IWindow>) override;

        Base::Interface<Interface::Window::IWindow> getMainWindow() override;
    public:
        void onInitialize() override;
        void onTick() override;
        void onDeinitialize() override;

    public:
        void setSelf(Base::Interface<Interface::Window::IWindowManager> self) { m_self = self; }
    private:
        Base::Interface<Interface::Window::IWindowManager> m_self;
    };
}