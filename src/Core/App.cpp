#include "App.hpp"

#include <GLFW/glfw3.h>

namespace BL
{
    // Define the constructor for the App class
    App::App()
    :m_lasttime(0)
    {
    }

    // Define the destructor for the App class
    App::~App()
    {
        // Destroy all windows and clear the windows map
        for (auto it = m_Windows.begin(); it != m_Windows.end(); ++it)
        {
            it->second->destroyWindow();
        }
        m_Windows.clear();

        // Terminate GLFW
        glfwTerminate();
    }

    // Create a new window
    // Parameters:
    // - name: Name of the window
    // - ws: WindowSettings object that contains the window settings
    // Returns:
    // - Pointer to the created Window object if successful, nullptr otherwise
    Window* App::createWindow(std::string name, WindowSettings ws)
    {
        // Check if a window with the given name already exists
        if (m_Windows.find(name) == m_Windows.end())
        {
            Window* win = new Window(ws);

            if (!win->initialize())
            {
                std::cout << "Failed to create " << name << " window from function: createWindow(std::string name, WindowSettings ws)" << std::endl;
                delete win;
                return nullptr;
            }

            m_Windows[name] = win;
            
            return m_Windows[name];
        }
        else
        {
            std::cout << name << " is already used from function: createWindow(std::string name, WindowSettings ws)" << std::endl;
            return nullptr;
        }
    }

    // Start rendering for all windows
    void App::start()
    {
        for (auto it = m_Windows.begin(); it != m_Windows.end(); ++it)
        {
            it->second->startRenderThread();
        }
    }

    // Start rendering for a specific window
    // Parameters:
    // - name: Name of the window to start rendering
    void App::startWindow(std::string name)
    {
        if (m_Windows.find(name) != m_Windows.end())
        {
            m_Windows[name]->startRenderThread();
        }
    }

    // Stop rendering for all windows
    void App::end()
    {
        for (auto it = m_Windows.begin(); it != m_Windows.end(); ++it)
        {
            it->second->endRenderThread();
        }
    }

    // Stop rendering for a specific window
    // Parameters:
    // - name: Name of the window to stop rendering
    void App::endWindow(std::string name)
    {
        if (m_Windows.find(name) != m_Windows.end())
        {
            m_Windows[name]->endRenderThread();
        }
    }

    // Update the application by polling events
    void App::update()
    {
        glfwWaitEvents();
    }

    // Check if all windows are still running
    // Returns:
    // - true if all windows are running, false otherwise
    bool App::allRunning()
    {
        bool result = true;

        for (auto it = m_Windows.begin(); it != m_Windows.end(); ++it)
        {
            if (it->second->shouldClose())
            {
                result = false;
                break;
            }
        }

        return result;
    }

    // Check if at least one window is still running
    // Returns:
    // - true if at least one window is running, false otherwise
    bool App::oneRunning()
    {
        bool result = false;

        for (auto it = m_Windows.begin(); it != m_Windows.end(); ++it)
        {
            if (!it->second->shouldClose())
            {
                result = true;
                break;
            }
        }

        return result;
    }

    // Check if a specific window is still running
    // Parameters:
    // - name: Name of the window to check
    // Returns:
    // - true if the window is running, false otherwise
    bool App::windowRunning(std::string name)
    {
        if (m_Windows.find(name) != m_Windows.end())
        {
            return !m_Windows[name]->shouldClose();
        }

        std::cout << name << " window does not exist from function: windowRunning(std::string name)" << std::endl;

        return false;
    }
}
