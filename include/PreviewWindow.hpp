#pragma once
#ifndef id68C60174_0140_4DE1_B7255EFF557A74F9
#define id68C60174_0140_4DE1_B7255EFF557A74F9

#include <Window.hpp>
#include <MyDropTarget.hpp>

#include <commctrl.h>
#include <shellapi.h>

#define WM_CHANGETEXT (WM_USER+1)

class PreviewWindow : public Window {
public:
    virtual LPCTSTR ClassName() { return L"Zalgo_Preview"; }
    static PreviewWindow *Create(LPCTSTR szTitle);
    void Destroy() { DestroyWindow(m_hwnd); }

    void ChangeText(LPWSTR text, bool padding = true);
    void SetFont(HFONT hFont);
    void SetFont(const LOGFONT &lf);
protected:
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnCreate();
    LRESULT OnDestroy();
    void OnPaint();

    virtual HICON GetIcon();
private:
    HFONT hFont;
    bool used_logfont;
    HBRUSH hBrush;
    int xChar, yChar, xUpper, xClient, yClient, xClientMax, xPos, yPos;
    LPTSTR *lpLines;
    int lines;
    bool empty;
};

#endif // header
