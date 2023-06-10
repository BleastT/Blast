#pragma once

#include <iostream>

#ifdef BLAST_WIN
#define GLFW_EXPOSE_NATIVE_WIN32
#define WIN32_LEAN_AND_MEAN 
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Windows.h>
// #include <dwmapi.h>

namespace BL
{
    WNDPROC original_proc;

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
            case WM_NCCALCSIZE:
            {
                // Remove the window's standard sizing border
                if (wParam == TRUE && lParam != NULL)
                {
                    NCCALCSIZE_PARAMS* pParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
                    pParams->rgrc[0].top += 1;
                    pParams->rgrc[0].right -= 1;
                    pParams->rgrc[0].bottom -= 1;
                    pParams->rgrc[0].left += 1;
                }
                return 0;
            }
            case WM_NCPAINT:
            {
                // Prevent the non-client area from being painted
                return 0;
            }
            case WM_NCHITTEST:
            {
                // Expand the hit test area for resizing
                const int borderWidth = 8; // Adjust this value to control the hit test area size

                POINTS mousePos = MAKEPOINTS(lParam);
                POINT clientMousePos = { mousePos.x, mousePos.y };
                ScreenToClient(hWnd, &clientMousePos);

                RECT windowRect;
                GetClientRect(hWnd, &windowRect);

                if (clientMousePos.y >= windowRect.bottom - borderWidth)
                {
                    if (clientMousePos.x <= borderWidth)
                        return HTBOTTOMLEFT;
                    else if (clientMousePos.x >= windowRect.right - borderWidth)
                        return HTBOTTOMRIGHT;
                    else
                        return HTBOTTOM;
                }
                else if (clientMousePos.y <= borderWidth)
                {
                    if (clientMousePos.x <= borderWidth)
                        return HTTOPLEFT;
                    else if (clientMousePos.x >= windowRect.right - borderWidth)
                        return HTTOPRIGHT;
                    else
                        return HTTOP;
                }
                else if (clientMousePos.x <= borderWidth)
                {
                    return HTLEFT;
                }
                else if (clientMousePos.x >= windowRect.right - borderWidth)
                {
                    return HTRIGHT;
                }

                break;
            }
            case WM_NCACTIVATE:
            {
                // Prevent non-client area from being redrawn during window activation
                return TRUE;
            }
        }
        
        return CallWindowProc(original_proc, hWnd, uMsg, wParam, lParam);
    }
    void disableTitlebar(GLFWwindow* window)
    {
        HWND hWnd = glfwGetWin32Window(window);

        LONG_PTR lStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
        lStyle |= WS_THICKFRAME;
        lStyle &= ~WS_CAPTION;
        SetWindowLongPtr(hWnd, GWL_STYLE, lStyle);

        RECT windowRect;
        GetWindowRect(hWnd, &windowRect);
        int width = windowRect.right - windowRect.left;
        int height = windowRect.bottom - windowRect.top;

        original_proc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc));
        SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_FRAMECHANGED | SWP_NOMOVE);
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