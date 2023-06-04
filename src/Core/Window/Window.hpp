// Include guard to prevent multiple inclusion of the header file
#pragma once


#include <iostream>
#include <thread>
#include <chrono>

// Include the WindowSettings.hpp header file
#include "./WindowSettings.hpp"
#include "../Rendering/Renderer.hpp"
#include "./Event.hpp"


class GLFWwindow;
// Create a namespace named BL
namespace BL
{
    // Define the Window class
    class Window
    {
        // Variables
    public:
        // Public variables accessible from outside the class
    private:
        // Private variables accessible only within the class
        WindowSettings m_ws;        // Window settings object
        GLFWwindow* m_Win;          // Pointer to GLFW window
        bool m_Window_exist;        // Flag to indicate if the window exists

        std::thread m_Render_thread;        // Thread for rendering
        bool m_Render_thread_started;       // Flag to indicate if the render thread has started

        Renderer* m_Renderer;
        Event* m_Event;


        // Events informations
        int m_Width;
        int m_Height;

        int m_Fwidth;
        int m_Fheight;

        double m_mousex;
        double m_mousey;

        // Functions
    public:
        // Public member functions accessible from outside the class

        // Constructor for the Window class
        // Parameters:
        // - ws: WindowSettings object that contains the window settings
        Window(WindowSettings ws);

        // Destructor for the Window class
        ~Window();

        // Initialize the window and the OpenGL context
        // Returns:
        // - true if initialization is successful, false otherwise
        bool initialize();

        // Check if the window should close
        // Returns:
        // - true if the window should close, false otherwise
        bool shouldClose();

        // Start the rendering thread
        void startRenderThread();

        // End the rendering thread
        void endRenderThread();

        // Destroy the GLFW window
        void destroyWindow();

    private:
        // Private member functions accessible only within the class
        void handleEvents();
        void resetEvents();
    };
}