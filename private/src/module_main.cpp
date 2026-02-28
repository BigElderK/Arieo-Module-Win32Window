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
            Base::Instance<Win32WindowManager> win32_window_manager;
            
            DllLoader()
            { 
                win32_window_manager->initialize();
                
                Base::InteropOld<Interface::Main::IMainModule> main_module = Core::ModuleManager::getInterface<Interface::Main::IMainModule>();
                main_module->registerTickable(win32_window_manager.queryInterface<Interface::Main::ITickable>());

                Core::ModuleManager::registerInstance<Interface::Window::IWindowManager, Win32WindowManager>(
                    "window_manager", 
                    win32_window_manager
                );
            }

            ~DllLoader()
            {
                Core::ModuleManager::unregisterInstance<Interface::Window::IWindowManager, Win32WindowManager>(
                    win32_window_manager
                );
                win32_window_manager->finalize();
            }
        } dll_loader;
    }
}