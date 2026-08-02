#include <DX3D/Window/Window.h>
//#include "EngineTime.h"
#include <IMGUI/imgui.h>

// for handling events in imgui
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    // for processing inputs in imgui
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
        return true;
    }

    switch (msg) {
        case WM_SETFOCUS: {

            // Event fired when the window get focus
            Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

            if (window) window->onFocus();
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
        case WM_MOVE: // for when moving the window
        case WM_SIZE: {
            Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (window) {
                window->onResize(window->getClientSize());
            }
            break;
        }
        case WM_CLOSE: {
            PostQuitMessage(0);
            break;
        }
        default: {
            return ::DefWindowProc(hwnd, msg, wparam, lparam);
        }
    }
}

Window::Window() {
    // starting up the WNDCLASSEX object and define the window attributes
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX); 
    wc.lpszClassName = L"MyWindowClass";
    wc.lpfnWndProc = &WndProc;

    // if the registration of class will fail, return false
    auto classID = ::RegisterClassEx(&wc);

    if(!classID) DX3DError("Window did not initiate successfully");

    RECT rc = { 0, 0, m_size.width, m_size.height };
    ::AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    // create the window
    m_hwnd = ::CreateWindowEx(NULL,
        MAKEINTATOM(classID), L"DirectX 3D Engine",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        NULL, NULL, NULL, NULL);

    // if window creation failed, return false
    if (!m_hwnd) DX3DError("Window did not initiate successfully");

    auto hwnd = static_cast<HWND>(m_hwnd);

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

    // show the window
    ::ShowWindow(hwnd, SW_MAXIMIZE);
    ::UpdateWindow(hwnd);
}

Window::~Window() {
    DestroyWindow(static_cast<HWND>(m_hwnd));
}


Rect Window::getClientSize() {
    RECT rc = {};
    auto hwnd = static_cast<HWND>(m_hwnd);

    ::GetClientRect(hwnd, &rc);
    ::ClientToScreen(hwnd, (LPPOINT)&rc.left);
    ::ClientToScreen(hwnd, (LPPOINT)&rc.right);

    return { rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };
}

Rect Window::getSizeScreen() {
    RECT rc = {};
    rc.right = ::GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);

    return { 0, 0, rc.right - rc.left, rc.bottom - rc.top };
}

void Window::onCreate() {

}

void Window::onUpdate() {

}

void Window::onDestroy() {

}

void Window::onFocus() {

}

void Window::onKillFocus() {

}

void Window::onResize(const Rect& size) {

}
