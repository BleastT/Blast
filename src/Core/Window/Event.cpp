#include "Event.hpp"

#include <GLFW/glfw3.h>

namespace BL
{

    Event::Event()
    {
        reset();
    }

    Event::~Event()
    {

    }


    void Event::setEventsCallbacks(GLFWwindow* window)
    {
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetWindowSizeCallback(window, window_size_callback);    
    }

    void Event::reset()
    {
        is_mouse_moving = false;
        is_window_resizing = false;
    }


    void Event::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        static_cast<Event*>(glfwGetWindowUserPointer(window))->is_mouse_moving = true;
    }

    void Event::window_size_callback(GLFWwindow* window, int width, int height)
    {
        static_cast<Event*>(glfwGetWindowUserPointer(window))->is_window_resizing = true;
    }

}