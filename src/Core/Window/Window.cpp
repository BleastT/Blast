// Include the Window.hpp header file
#include "Window.hpp"

// Include necessary libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./PlatformSpecificFunctions.hpp"


// Create a namespace named BL
namespace BL
{
    // Define the constructor for the Window class
    // Parameters:
    // - ws: WindowSettings object that contains the window settings
    Window::Window(WindowSettings ws)
    {
        // Initialize member variables
        m_ws = ws;

        m_Window_exist = false;
        m_Render_thread_started = false;
    }

    // Define the destructor for the Window class
    Window::~Window()
    {
        // Destructor is empty as there is no dynamic memory allocation
    }

    // Initialize the window and the OpenGL context
    // Returns:
    // - true if initialization is successful, false otherwise
    bool Window::initialize()
    {
        // Initialize GLFW
        glfwInit();

        // Set the OpenGL context version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);


        // Initialize the window
        m_Win = glfwCreateWindow(m_ws.width, m_ws.height, m_ws.titlebar_name.c_str(), NULL, NULL);

        disableTitlebar(m_Win);
        if(!m_Win)
        {
            return false;
        }

        // Make the OpenGL context current
        glfwMakeContextCurrent(m_Win);

        // Initialize GLEW
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize OpenGL" << std::endl;
            return false;
        }

        std::cout << glGetString(GL_VERSION) << std::endl;

        // Set the window existence flag to true
        m_Window_exist = true;


        m_Renderer = new Renderer();
        m_Event = new Event();

        glfwSetWindowUserPointer(m_Win, m_Event);

        m_Event->setEventsCallbacks(m_Win);

        m_Width = m_ws.width;
        m_Height = m_ws.height;

        glfwGetFramebufferSize(m_Win, &m_Fwidth, &m_Fheight);

        glViewport(0, 0, m_Fwidth, m_Fheight );
        m_Renderer->UpdateProjection(m_Fwidth, m_Fheight);

        // Unbind the current OpenGL context
        glfwMakeContextCurrent(NULL);

        return true;
    }

    // Check if the window should close
    // Returns:
    // - true if the window should close, false otherwise
    bool Window::shouldClose()
    {
        return glfwWindowShouldClose(m_Win);
    }

    // Start the rendering thread
    void Window::startRenderThread()
    {

        // Check if the render thread has already started
        if (!m_Render_thread_started)
        {
            if(!m_Window_exist)
            {
                initialize();
            }

            // Set the render thread started flag to true
            m_Render_thread_started = true;

            // Start the render thread
            m_Render_thread = std::thread([this] {
                double lasttime = glfwGetTime();
                int dt = 0;
                double plus = 0;
                while (!this->shouldClose())
                {
                    // Make the OpenGL context current
                    glfwMakeContextCurrent(this->m_Win);

                    // Clear the color buffer
                    glClear(GL_COLOR_BUFFER_BIT);
                    
                    glfwSwapBuffers(this->m_Win);

                    this->handleEvents();
                    this->resetEvents();
                    
                    glfwMakeContextCurrent(NULL);

                    dt = ((lasttime + 1.0/this->m_ws.framerate) - glfwGetTime()) * 1000;
                    if(dt > 0)
                    {
                        std::this_thread::sleep_for (std::chrono::milliseconds(dt));
                        plus = 0;
                    }
                    if(dt < 0)
                    {
                        plus = -dt;
                    }

                    lasttime += (1.0/this->m_ws.framerate) - plus;
                }
            });
        }
    }

    // End the rendering thread
    void Window::endRenderThread()
    {
        // Check if the render thread has started
        if (m_Render_thread_started)
        {
            // Set the render thread started flag to false
            m_Render_thread_started = false;

            glfwSetWindowShouldClose(m_Win, GL_TRUE);
            m_Window_exist = false;

            // Wait for the render thread to finish
            m_Render_thread.join();
        }
    }

    // Destroy the GLFW window
    void Window::destroyWindow()
    {
        // Set the window existence flag to false
        m_Window_exist = false;

        // End the render thread
        endRenderThread();

        // Destroy the GLFW window
        glfwDestroyWindow(m_Win);
    }



    void Window::handleEvents()
    {
        if(m_Event->isMouseMoving())
        {
            glfwGetCursorPos(m_Win, &m_mousex, &m_mousey);

            // std::cout << m_mousex << " " << m_mousey << std::endl;
        }

        if(m_Event->isWindowResizing())
        {   
            glfwGetWindowSize(m_Win, &m_Width, &m_Height);
            glfwGetFramebufferSize(m_Win, &m_Fwidth, &m_Fheight);

            glViewport(0, 0, m_Fwidth, m_Fheight);

            m_Renderer->UpdateProjection(m_Fwidth, m_Fheight);

            // std::cout << m_Fwidth << " " << m_Fheight << std::endl;
        }
    }
    void Window::resetEvents()
    {
        m_Event->reset();
    }
}