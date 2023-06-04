#pragma once

#ifdef BLAST_WIN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
// #pragma comment(lib, "Dwmapi.lib")
#include <Windows.h>
#include <dwmapi.h>

namespace BL
{
    void disableTitlebar(GLFWwindow* window)
    {
        HWND hWnd = glfwGetWin32Window(window);

        // Remove the title bar
        LONG_PTR lStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
        lStyle &= ~WS_CAPTION;
        SetWindowLongPtr(hWnd, GWL_STYLE, lStyle);

        // Set the window shape and rounded corners
        DWMNCRENDERINGPOLICY policy = DWMNCRP_ENABLED;
        DwmSetWindowAttribute(hWnd, DWMWA_NCRENDERING_POLICY, &policy, sizeof(policy));

        // Extend the frame into the client area
        MARGINS margins = { 0 };
        DwmExtendFrameIntoClientArea(hWnd, &margins);

        // Adjust the window size to remove the thin frame at the top
        RECT windowRect;
        GetWindowRect(hWnd, &windowRect);
        SetWindowPos(hWnd, NULL, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_FRAMECHANGED | SWP_NOMOVE);
    }
}
#endif

#ifdef BLAST_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <X11/Xlib.h>

namespace BL
{
    void disableTitlebar(GLFWwindow* window)
    {
        // Get the X11 display and window
        Display* display = glfwGetX11Display();
        Window x11Window = glfwGetX11Window(window);

        // Disable window decorations (title bar, borders, etc.)
        Atom wmHintsAtom = XInternAtom(display, "_MOTIF_WM_HINTS", False);
        if (wmHintsAtom != None)
        {
            struct MotifWmHints
            {
                unsigned long flags;
                unsigned long functions;
                unsigned long decorations;
                long inputMode;
                unsigned long status;
            };

            MotifWmHints hints;
            hints.flags = 2;  // MWM_HINTS_DECORATIONS
            hints.decorations = 0;

            XChangeProperty(display, x11Window, wmHintsAtom, wmHintsAtom, 32, PropModeReplace, reinterpret_cast<const unsigned char*>(&hints), 5);
        }

        // Set the window shape and rounded corners
        XWindowAttributes windowAttributes;
        XGetWindowAttributes(display, x11Window, &windowAttributes);

        XRectangle rect;
        rect.x = 0;
        rect.y = 0;
        rect.width = windowAttributes.width;
        rect.height = windowAttributes.height;

        XserverRegion region = XCreateRegion();
        XUnionRectWithRegion(&rect, region, region);

        XShapeCombineRegion(display, x11Window, ShapeBounding, 0, 0, region, ShapeSet);
        XShapeCombineRegion(display, x11Window, ShapeInput, 0, 0, region, ShapeSet);
        XDestroyRegion(region);
    }
}
  
#endif

#ifdef BLAST_MAC
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Cocoa/Cocoa.h>
namespace BL
{
    void disableTitlebar(GLFWwindow* window)
    {
        // Get the NSWindow from the GLFW window
        void* cocoaWindow = glfwGetCocoaWindow(window);

        // Set the window style mask to remove the title bar
        id nsWindow = reinterpret_cast<id>(cocoaWindow);
        [nsWindow setStyleMask:[nsWindow styleMask] & ~NSWindowStyleMaskTitled];

        // Set the window shape and rounded corners
        [nsWindow setMovableByWindowBackground:NO];
        [nsWindow setOpaque:NO];
        [nsWindow setBackgroundColor:NSColor.clearColor];
        [nsWindow setHasShadow:YES];
    }
}
#endif

#if defined(GLFW_EXPOSE_NATIVE_WIN32) || defined(GLFW_EXPOSE_NATIVE_X11) || defined(GLFW_EXPOSE_NATIVE_COCOA)
#else
#include <GLFW/glfw3.h>
namespace BL
{
    void disableTitlebar(GLFWwindow* window){}
}
#endif