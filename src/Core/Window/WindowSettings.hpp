// Include guard to prevent multiple inclusion of the header file
#pragma once

#include <string>

// Create a namespace named BL
namespace BL
{
    // Define the WindowSettings struct
    struct WindowSettings
    {
        std::string titlebar_name{"window title"};    // Name of the window's titlebar
        bool titlebar_enabled{true};                   // Flag to indicate if the titlebar is enabled
        int width{900};                               // Width of the window
        int height{400};                               // Height of the window
        int framerate{30};
    };
}
