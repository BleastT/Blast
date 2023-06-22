// Include the Window.hpp header file
#include "Window.hpp"


// Include necessary libraries
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <thread>

#include "./PlatformSpecificFunctions.hpp"



// Create a namespace named BL
namespace BL
{
    // Define the constructor for the Window class
    // Parameters:
    // - ws: WindowSettings object that contains the window settings
    Window::Window(WindowSettings ws)
    : m_activepage("none"), m_activestylecollection("none")
    {
        // Initialize member variables
        m_ws = ws;

        m_Window_exist = false;
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

        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);

        // Initialize the window
        m_Win = glfwCreateWindow(m_ws.width, m_ws.height, m_ws.titlebar_name.c_str(), NULL, NULL);

        disableTitlebar(m_Win);
        if(!m_Win)
        {
            return false;
        }

        // Make the OpenGL context current
        glfwMakeContextCurrent(m_Win);

        // TODO: implement custom framerate control since enabling vsync seems to make the window flicker when resizing
        glfwSwapInterval(0);

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

        glfwSetWindowUserPointer(m_Win, m_Event);
        m_Event->setEventsCallbacks(m_Win);

        m_Renderer->UpdateProjection(m_Fwidth, m_Fheight);

        // int count;
        refreshrate = glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
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


        if(!m_Window_exist)
        {
            initialize();
        }



        // Start the render thread
        m_Render_thread = std::thread([this] {
            while (DoesWindowExit())
            {
                if(!m_pages.empty())
                {
                    glfwMakeContextCurrent(m_Win);

                    if(m_activepage == "none")
                    {
                        setPageActive(m_pages.begin()->first);

                        std::cout << "no active page selected, using " << m_activepage << " instead\n";
                    }
                    if(!m_stylecollections.empty())
                    {
                        if(m_activestylecollection == "none")
                        {
                            setStyleCollectionActive(m_stylecollections.begin()->first);

                            std::cout << "no style collection selected, using " << m_activestylecollection << " instead\n";
                        }
                    }

                    m_Renderer->ComputeComponent(nullptr , m_pages[m_activepage], &m_stylecollections[m_activestylecollection], 1.0f / refreshrate);

                    handleEvents();

                    glfwSwapBuffers(m_Win);
                    glfwMakeContextCurrent(NULL);

                    CapFrame(refreshrate);
                }
            }
        });
    }

    // End the rendering thread
    void Window::endRenderThread()
    {

        glfwSetWindowShouldClose(m_Win, GL_TRUE);
        m_Window_exist = false;

        // Wait for the render thread to finish
        m_Render_thread.join();  

        if(m_activepage != "none")
        {
            runComponentsQuitFunction(*m_pages[m_activepage]);
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
        }

        m_Event->reset();
    }


    void Window::appendNewPage(std::string ref_name, Component* page)
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
            runComponentsStartFunction(*m_pages[m_activepage]);
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


    void Window::CapFrame(int framerate)
    {
        double current_time = glfwGetTime();

        if(current_time - last_time < 1.0f / framerate)
        {
            long interval = ((1.0f / framerate) - (current_time - last_time)) * 1000.0f;

            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }

        last_time = glfwGetTime();
    }


    void Window::runComponentsStartFunction(Component& component)
    {
        component.render();
        component.start();

        for(Component& child : component.getChildren())
        {
            runComponentsStartFunction(child);
        }
    }

    void Window::runComponentsQuitFunction(Component& component)
    {
        component.quit();

        for(Component& child : component.getChildren())
        {
            runComponentsQuitFunction(child);
        }
    }
}