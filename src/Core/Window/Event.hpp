#pragma once

class GLFWwindow;
namespace BL
{

    class Event
    {
        // Variables
        public:
        private:
            bool is_window_resizing;
            bool is_mouse_moving;

        // Functions
        public:
            Event();
            ~Event();

            void setEventsCallbacks(GLFWwindow* window);

            void reset();


            inline bool isMouseMoving() {return is_mouse_moving;}
            inline bool isWindowResizing() {return is_window_resizing;}

        private:

            static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
            static void window_size_callback(GLFWwindow* window, int width, int height);
    };
}