#pragma once
#ifndef id68C60171_0140_4DE1_B7255EFF557A74F9
#define id68C60171_0140_4DE1_B7255EFF557A74F9

#include <Window.hpp>
#include <MyDropTarget.hpp>
#include <PreviewWindow.hpp>

#include <commctrl.h>
#include <shellapi.h>

#define ZALGO_GO_UP         0xAB01
#define ZALGO_GO_CENTER     0xAB02
#define ZALGO_GO_DOWN       0xAB03
#define ZALGO_MINI_MESS     0xDEAD
#define ZALGO_NORMAL_MESS   0xDEAE
#define ZALGO_MAX_MESS      0xDEAF
#define ZALGO_CUSTOM_MESS   0xDEAC
#define ZALGO_MESS_LEVEL    0xDEAB
#define ZALGO_HE_COMES      0xBEEF
#define ZALGO_HE_GOES       0xBEEE
#define ZALGO_PREVIEW       0xBEED
#define ZALGO_MESSAGE       0xDEED
#define TEXT_TO_NFC         0xA551
#define TEXT_TO_NFD         0xA552
#define ZALGO_LATIN         0xA554
#define ZALGO_CYRILLIC      0xA555
#define ZALGO_GREEK         0xA556
#define ZALGO_XSAMPA        0xA557
#define ZALGO_XSAMPA_TABLE  0xA558
#define ZALGO_LOAD          0xA559
#define ZALGO_SAVE          0xA55A
#define ZALGO_USE_UTF8      0xA55B

class MainWindow : public Window {
public:
    virtual LPCTSTR ClassName() { return L"Zalgo_Main"; }
    static MainWindow *Create(LPCTSTR szTitle);
protected:
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnCreate();
    LRESULT OnDestroy();
    void OnPaint();
    BOOL WinRegisterClass(WNDCLASS *pwc);
    virtual HICON GetIcon();

    void OnZalgoComes();
    void OnZalgoGoes();
    void OnTextNFC();
    void OnTextNFD();
    void OnTextCyrillic();
    void OnTextLatin();
    void OnTextGreek();
    void OnTextXSAMPA();
    
    void OnLoadFile();
    void OnSaveFile();

    HWND m_message, m_settings;
    HWND m_goUp, m_goMiddle, m_goDown;
    HWND m_messMini, m_messNormal, m_messMax, m_messCustom;
    HWND m_messLevel, m_messUpDown;
    HWND m_mess, m_unmess, m_previewShow;
    HWND m_nfc, m_nfd;
    HWND m_load, m_save, m_use_utf8;
    HWND m_latin, m_cyrillic, m_greek, m_xsampa, m_xsampa_table;
    MyDropTarget m_dropTarget;
    PreviewWindow *m_preview, *m_data_display;
private:
    HFONT hFont;
    HBRUSH hBrush;
    UDACCEL *udaSecondAccel;
};

#endif // header
