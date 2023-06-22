#pragma once

#include <iostream>

#ifdef BLAST_WIN
#define GLFW_EXPOSE_NATIVE_WIN32
#define WIN32_LEAN_AND_MEAN 
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Windows.h>

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

            // makes the aero snap menu visible on windows 11 when you hover
            // return HTMAXBUTTON;

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

#endif

#ifdef BLAST_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>

void disableTitlebar(GLFWwindow* window)
{
    Display* display = glfwGetX11Display();
    Window x11Window = glfwGetX11Window(window);

    // Disable title bar
    XSetWindowAttributes attributes;
    Atom atom = XInternAtom(display, "_MOTIF_WM_HINTS", False);

    if (atom != None) {
        long hints[5] = {2, 0, 0, 0, 0}; // Set the flags to disable title bar
        XChangeProperty(display, x11Window, atom, atom, 32, PropModeReplace, (unsigned char *)hints, 5);
    }

    // Set window shape and rounded corners using XShape extension
    int shapeEventBase, shapeErrorBase;
    if (XShapeQueryExtension(display, &shapeEventBase, &shapeErrorBase)) {
        XRectangle rectangle;

        int width;
        int height;

        glfwGetWindowSize(window, &width, &height);

        rectangle.x = 0;
        rectangle.y = 0;
        rectangle.width = width; // Replace with your desired window width
        rectangle.height = height; // Replace with your desired window height

        XShapeCombineRectangles(display, x11Window, ShapeBounding, 0, 0, &rectangle, 1, ShapeSet, YXBanded);
        XShapeCombineRectangles(display, x11Window, ShapeInput, 0, 0, &rectangle, 1, ShapeSet, YXBanded);
        XShapeCombineRectangles(display, x11Window, ShapeClip, 0, 0, &rectangle, 1, ShapeSet, YXBanded);
    }

}

  
#endif

#ifdef BLAST_MAC
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>

void disableTitlebar(GLFWwindow* window)
{
    // Get the native Cocoa window handle
    NSWindow* cocoaWindow = glfwGetCocoaWindow(glfwWindow);

    // Set the window style mask to remove the titlebar
    [cocoaWindow setStyleMask:NSWindowStyleMaskResizable | NSWindowStyleMaskFullSizeContentView];
    
    // Set rounded corners
    [[cocoaWindow contentView] setWantsLayer:YES];
    [[[cocoaWindow contentView] layer] setCornerRadius:10.0];
    [[[cocoaWindow contentView] layer] setMasksToBounds:YES];

    // Make the window visible
    [cocoaWindow makeKeyAndOrderFront:nil];
}

#endif

#if defined(BLAST_WIN) || defined(BLAST_MAC) || defined(BLAST_LINUX)
#else
#include <GLFW/glfw3.h>

void disableTitlebar(GLFWwindow* window){}

#endif