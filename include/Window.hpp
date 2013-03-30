#pragma once
#ifndef idC3714409_EABC_45DE_B5338EC60149AE87
#define idC3714409_EABC_45DE_B5338EC60149AE87

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

inline void MakeRect(RECT &rect, int x, int y, int cx, int cy)
{
    rect.left   = x;
    rect.top    = y;
    rect.right  = x + cx;
    rect.bottom = y + cy;
}

inline void MakeRectRight(RECT &rect, int x, int y, int cx, int cy)
{
    MakeRect(rect, x - cx, y, cx, cy);
}

inline void MakeRectBottom(RECT &rect, int x, int y, int cx, int cy)
{
    MakeRect(rect, x, y - cy, cx, cy);
}

inline void MakeRectBottomRight(RECT &rect, int x, int y, int cx, int cy)
{
    MakeRect(rect, x - cx, y - cy, cx, cy);
}

class Window {
public:
    HWND GetHWND() { return m_hwnd; }
    static HINSTANCE GetInstance() { return (HINSTANCE) GetModuleHandle(NULL); }
protected:
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void PaintContent(PAINTSTRUCT *pps) {}
    virtual LPCTSTR ClassName() = 0;
    virtual HICON GetIcon() { return NULL; }
    virtual BOOL WinRegisterClass(WNDCLASS *pwc) {
        return RegisterClass(pwc);
    }
    virtual ~Window();

    HWND WinCreateWindow(DWORD dwExStyle, LPCTSTR pszName,
                         DWORD dwStyle, int x, int y, int cx,
                         int cy, HWND hwndParent, HMENU hmenu);
private:
    virtual void Register();
    void OnPaint();
    void OnPrintClient(HDC hdc);

    static LRESULT CALLBACK
            s_WndProc(HWND hwnd, UINT uMsg,
                      WPARAM wParam, LPARAM lParam);
protected:
    HWND m_hwnd;
};

#endif // header
