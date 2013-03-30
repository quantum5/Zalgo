#include <window.hpp>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

HWND Window::WinCreateWindow(DWORD dwExStyle, LPCTSTR pszName,
                             DWORD dwStyle, int x, int y, int cx,
                             int cy, HWND hwndParent, HMENU hmenu)
{
    Register();
    return CreateWindowEx(dwExStyle, ClassName(), pszName, dwStyle,
                          x, y, cx, cy, hwndParent, hmenu, GetInstance(),
                          this);
}

Window::~Window() {}

void Window::Register()
{
    WNDCLASS wc;
    wc.style         = 0;
    wc.lpfnWndProc   = Window::s_WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetInstance();
    wc.hIcon         = GetIcon();
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(0xF0, 0xF0, 0xF0));
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = ClassName();

    WinRegisterClass(&wc);
}

LRESULT CALLBACK
Window::s_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Window *self;
    if (uMsg == WM_NCCREATE) {
        LPCREATESTRUCT lpcs = (LPCREATESTRUCT) lParam;
        self = (Window *)(lpcs->lpCreateParams);
        self->m_hwnd = hwnd;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LPARAM) self);
    } else {
        self = (Window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }
    if (self) {
        return self->HandleMessage(uMsg, wParam, lParam);
    } else {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_NCDESTROY: {
            LRESULT lres;
            lres = DefWindowProc(m_hwnd, uMsg, wParam, lParam);
            SetWindowLongPtr(m_hwnd, GWLP_USERDATA, 0);
            delete this;
            return lres;
        }
        case WM_PAINT:
            OnPaint();
            return 0;
        case WM_PRINTCLIENT:
            OnPrintClient(reinterpret_cast<HDC>(wParam));
            return 0;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void Window::OnPaint()
{
    PAINTSTRUCT ps;
    BeginPaint(m_hwnd, &ps);
    PaintContent(&ps);
    EndPaint(m_hwnd, &ps);
}

void Window::OnPrintClient(HDC hdc)
{
    PAINTSTRUCT ps;
    ps.hdc = hdc;
    GetClientRect(m_hwnd, &ps.rcPaint);
    PaintContent(&ps);
}
