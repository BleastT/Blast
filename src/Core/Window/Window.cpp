// Include the Window.hpp header file
#include "Window.hpp"


// Include necessary libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "./PlatformSpecificFunctions.hpp"



// Create a namespace named BL
namespace BL
{
    // Define the constructor for the Window class
    // Parameters:
    // - ws: WindowSettings object that contains the window settings
    Window::Window(WindowSettings ws)
    : were_event_handled(false), m_activepage("none")
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

        // Initialize the window
        m_Win = glfwCreateWindow(m_ws.width, m_ws.height, m_ws.titlebar_name.c_str(), NULL, NULL);

        // disableTitlebar(m_Win);
        if(!m_Win)
        {
            return false;
        }

        // Make the OpenGL context current
        glfwMakeContextCurrent(m_Win);

        // TODO: implement custom framerate control since enabling vsync seems to make the window flicker when resizing
        glfwSwapInterval(1);

        // Initialize GLEW
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize OpenGL" << std::endl;
            return false;
        }

        std::cout << glGetString(GL_VERSION) << std::endl;


        // Set the window existence flag to true
        m_Window_exist = true;

        m_Renderer = new Renderer();
        m_Event = new Event();

        m_Width = m_ws.width;
        m_Height = m_ws.height;
        glfwGetFramebufferSize(m_Win, &m_Fwidth, &m_Fheight);

        m_Renderer->UpdateProjection(m_Fwidth, m_Fheight);

        glfwSetWindowUserPointer(m_Win, m_Event);
        m_Event->setEventsCallbacks(m_Win);

       
        glEnable(GL_SCISSOR_TEST);

        // Unbind the current OpenGL context
        glfwMakeContextCurrent(NULL);

        return true;
    }

    // Check if the window should close
    // Returns:
    // - true if the window should close, false otherwise
    bool Window::DoesWindowExit()
    {
        return m_Window_exist;
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
                while (this->DoesWindowExit())
                {

                    glfwMakeContextCurrent(this->m_Win);

                    if(!m_pages.empty())
                    {
                        if(m_activepage == "none")
                        {
                            m_activepage = m_pages.begin()->first;

                            std::cout << "no active page selected, using " << m_activepage << " instead\n";
                        }


                        this->m_Renderer->Update(&m_pages[m_activepage], &m_stylecollections[m_activestylecollection]);
                    }

                    this->handleEvents();
                    this->resetEvents();

                    glfwSwapBuffers(this->m_Win);
                    glfwMakeContextCurrent(NULL);

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

            m_Renderer->UpdateProjection(m_Fwidth, m_Fheight);

            // std::cout << m_Fwidth << " " << m_Fheight << std::endl;
        }

        were_event_handled = true;
    }

    void Window::resetEvents()
    {
        if(were_event_handled)
        {
            m_Event->reset();
            were_event_handled = false;
        }
    }



    void Window::appendNewPage(std::string ref_name, Component page)
    {
        if(m_pages.find(ref_name) == m_pages.end())
        {
            m_pages[ref_name] = page;
            return;
        }

        std::cout << "The page " << ref_name << " already exists \n";

    }


    void Window::setPageActive(std::string ref_name)
    {
        if(m_pages.find(ref_name) != m_pages.end())
        {
            m_activepage = ref_name;
            return;
        }

        std::cout << "The page " << ref_name << " does not exist \n"; 
    }


    void Window::appendNewStyleCollection(std::string ref_name, StyleCollection collection)
    {
        if(m_stylecollections.find(ref_name) == m_stylecollections.end())
        {
            m_stylecollections[ref_name] = collection;
            return;
        }

        std::cout << "The style collection " << ref_name << " already exist \n";
    }

    void Window::setStyleCollectionActive(std::string ref_name)
    {
        if(m_stylecollections.find(ref_name) != m_stylecollections.end())
        {
            m_activestylecollection = ref_name;
            return;
        }

        std::cout << "The style collection " << ref_name << " already exist \n";
    }
}