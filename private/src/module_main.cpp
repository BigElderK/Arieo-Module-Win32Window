#include "base/prerequisites.h"
#include "core/core.h"
#include "win32_window.h"
namespace Arieo
{
    GENERATOR_MODULE_ENTRY_FUN()
    ARIEO_DLLEXPORT void ModuleMain()
    {
        Core::Logger::setDefaultLogger("win32_window");

        static struct DllLoader
        {
            Win32WindowManager win32_window_manager;

            DllLoader()
            { 
                win32_window_manager.initialize();

                Base::Interface<Interface::Main::IMainModule> main_module = Core::ModuleManager::getInterface<Interface::Main::IMainModule>();
                main_module->registerTickable(&win32_window_manager);

                Core::ModuleManager::registerInterface<Interface::Window::IWindowManager>(
                    "window_manager", 
                    &win32_window_manager
                );
            }

            ~DllLoader()
            {
                Core::ModuleManager::unregisterInterface<Interface::Window::IWindowManager>(
                    &win32_window_manager
                );
                win32_window_manager.finalize();
            }
        } dll_loader;
    }
}