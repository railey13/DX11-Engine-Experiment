#include "Window.h"
#include "EngineTime.h"
#include <iostream>
#include "../Settings.h"

Window::Window() {

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_CREATE: {
            // event fired when the window is created 
            // (updated code)
            Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
            // this part stores for later lookup
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
            window->setHWND(hwnd);
            window->onCreate();
            break;
        }
        case WM_SETFOCUS: {

            // Event fired when the window get focus
            Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            window->onFocus();
            break;
        }
        case WM_KILLFOCUS: {

            // Event fired when the windowlost focus
            Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            window->onKillFocus();
            break;
        }
        case WM_DESTROY: {
            // event fired when the window will be destroyed
            // (updated code)
            Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            window->onDestroy();
            ::PostQuitMessage(0);
            break;
        }
        default: {
            return ::DefWindowProc(hwnd, msg, wparam, lparam);
        }
    }
}


bool Window::init() {
    // starting up the WNDCLASSEX object and define the window attributes
    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpszClassName = "MyWindowClass";
    wc.lpszMenuName = "";
    wc.style = NULL;
    wc.lpfnWndProc = &WndProc;

    // if the registration of class will fail, return false
    if (!::RegisterClassEx(&wc)) {
        return false;
    }

    // create the window
    m_hwnd=::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, 
        "MyWindowClass", "DirectX Application", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT ,CW_USEDEFAULT, 
        Settings::WindowWidth, Settings::WindowHeight,
        NULL,NULL,NULL,this);

    // if window creation failed, return false
    if (!m_hwnd) {
        return false;
    }

    // show the window
    ::ShowWindow(m_hwnd, SW_SHOW);
    ::UpdateWindow(m_hwnd);

    // set to true to indicate the window is running properly
    m_is_run = true;
    return true;
}

bool Window::broadcast() {
    EngineTime::LogFrameStart();    

    this->onUpdate();

    MSG msg;
    
    while (::PeekMessage(&msg,NULL,0,0,PM_REMOVE)>0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }   

    EngineTime::LimitFPS(Settings::FrameRateLimit);
    EngineTime::UpdateFPSCounter();

    return true;
}

bool Window::release() {
    // destroy the window
    if (!::DestroyWindow(m_hwnd)) {
        return false;
    }
    return false;
}

bool Window::isRun() {

    return m_is_run;
}

RECT Window::getClientWindowRect() {
    RECT rc;
    ::GetClientRect(this->m_hwnd, &rc);

    return rc;
}

void Window::setHWND(HWND hwnd) {
    this->m_hwnd = hwnd;    
}

void Window::onDestroy(){
    m_is_run = false;
}

void Window::onFocus() {

}

void Window::onKillFocus() {

}

Window::~Window() {

}
