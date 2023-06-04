// Include guard to prevent multiple inclusion of the header file
#pragma once

#include <string>
#include <map>
#include <thread>
#include <iostream>
#include <chrono>

#include "./Window/Window.hpp"
#include "./Window/WindowSettings.hpp"

// Create a namespace named BL
namespace BL
{
    // Define the App class
    class App
    {
        // Variables
        public:
        private:
            std::map<std::string, Window*> m_Windows;    // Map to store the windows created by the application
            double m_lasttime;
        // Functions
        public:
            // Default constructor
            App();

            // Destructor
            ~App();

            // Create a new window
            // Parameters:
            // - name: Name of the window
            // - ws: WindowSettings object that contains the window settings
            // Returns:
            // - Pointer to the created Window object if successful, nullptr otherwise
            Window* createWindow(std::string name, WindowSettings ws);

            // Start rendering for all windows
            void start();

            // Start rendering for a specific window
            // Parameters:
            // - name: Name of the window to start rendering
            void startWindow(std::string name);

            // Stop rendering for all windows
            void end();

            // Stop rendering for a specific window
            // Parameters:
            // - name: Name of the window to stop rendering
            void endWindow(std::string name);

            // Update the application by polling events
            void update();

            // Check if all windows are still running
            // Returns:
            // - true if all windows are running, false otherwise
            bool allRunning();

            // Check if at least one window is still running
            // Returns:
            // - true if at least one window is running, false otherwise
            bool oneRunning();

            // Check if a specific window is still running
            // Parameters:
            // - name: Name of the window to check
            // Returns:
            // - true if the window is running, false otherwise
            bool windowRunning(std::string name);

        private:
            // No private members for now
    };
}
