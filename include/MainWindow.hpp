#pragma once
#ifndef id68C60171_0140_4DE1_B7255EFF557A74F9
#define id68C60171_0140_4DE1_B7255EFF557A74F9

#include <Window.hpp>
#include <MyDropTarget.hpp>
#include <PreviewWindow.hpp>

#include <commctrl.h>
#include <shellapi.h>

class MainWindow : public Window {
public:
    virtual LPCTSTR ClassName() { return TEXT("Zalgo_Main"); }
    static MainWindow *Create(LPCTSTR szTitle);
protected:
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnCreate();
    LRESULT OnDestroy();
    void OnPaint();

    virtual HICON GetIcon();
    
    HWND m_message;
    HWND m_goUp, m_goMiddle, m_goDown;
    HWND m_messMini, m_messNormal, m_messMax, m_messCustom;
    HWND m_messLevel, m_messUpDown;
    HWND m_mess, m_unmess, m_previewShow;
    HWND m_nfc, m_nfd;
    MyDropTarget m_dropTarget;
    PreviewWindow *m_preview;
private:
    HFONT hFont, hFontMono;
    HBRUSH hBrush;
    UDACCEL *udaSecondAccel;
};

#endif // header
