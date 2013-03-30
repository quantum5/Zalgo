#include <MainWindow.hpp>
#include <Converter.hpp>

#include <resource.h>
#include <commctrl.h>
#include <commdlg.h>
#include <windowsx.h>
#include <shlwapi.h>
#include <shlobj.h>

#include <limits.h>

#define BGCOLOUR RGB(0xF0, 0xF0, 0xF0)
#define LEFT(x, y, cx, cy) x, y, cx, cy
#define RIGHT(x, y, cx, cy) (x - cx), y, cx, cy
#define BOTTOM(x, y, cx, cy) x, (y - cy), cx, cy
#define BOTTOMRIGHT(x, y, cx, cy) (x - cx), (y - cy), cx, cy
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

#define ZALGO_INITIAL  (L"To invoke the hive-mind representing chaos.\r\n"\
                        L"Invoking the feeling of chaos.\r\n"\
                        L"With out order.\r\n"\
                        L"The Nezperdian hive-mind of chaos. Zalgo.\r\n"\
                        L"He who Waits Behind The Wall.\r\n"\
                        L"ZALGO!\r\n\r\n"\
                        L"WARNING: Resulting text size is roughly original "\
                        L"* (1 + fuck up level). Thou hast been warned.\r\n"\
                        L"\r\n"\
                        L"NOTE: If this program crashes on thee, blame "\
                        L"thyself for fucking up a piece of text that is too "\
                        L"big with a very high fuck up level.\r\n\r\n"\
                        L"Do blame Windows for not able to show proper text "\
                        L"in the edit control, for now, use the preview "\
                        L"button.\r\n\r\n"\
                        L"Bonus Geek Info: NFD text will lose all "\
                        L"diacritics. Thou hast been warned.\r\n")

UINT ZALGO_MESS_LEVEL_[3] = {6, 10, 14};
#define ZALGO_MESS_LEVEL_OF(type) (ZALGO_MESS_LEVEL_[type-0xDEAD])

#ifndef FNERR_INVALIDFILENAME
#   define FNERR_INVALIDFILENAME 0x3002
#endif

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "comdlg32.lib")

WNDPROC wpOrigEditProc;

LRESULT APIENTRY EditSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_KEYDOWN) {
        if (wParam == L'A' && GetKeyState(VK_CONTROL) < 0) {
            Edit_SetSel(hwnd, 0, -1);
            return 0;
        }
    }
    return CallWindowProc(wpOrigEditProc, hwnd, uMsg,
        wParam, lParam);
}

LRESULT MainWindow::OnCreate()
{
    NONCLIENTMETRICS ncmMetrics = { sizeof(NONCLIENTMETRICS) };
    RECT client;

    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncmMetrics, 0);
    GetClientRect(m_hwnd, &client);
    
    hFont = CreateFontIndirect(&ncmMetrics.lfMessageFont);
    hBrush = CreateSolidBrush(BGCOLOUR);
    hFontMono = CreateFont(0, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
                           OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           CLEARTYPE_QUALITY, FF_MODERN, TEXT("Consolas"));
    
    // Children
    m_message = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT,
            ZALGO_INITIAL, WS_CHILDWINDOW | WS_VISIBLE | ES_LEFT |
                  ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
            0, 0, 0, 0, m_hwnd, (HMENU) ZALGO_MESSAGE, GetInstance(), NULL);
    
    m_goUp = CreateWindow(WC_BUTTON, L"fuck up going &up",
            WS_CHILDWINDOW | WS_VISIBLE | BS_CHECKBOX, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_GO_UP, GetInstance(), NULL);
    m_goMiddle = CreateWindow(WC_BUTTON, L"fuck up the &middle",
            WS_CHILDWINDOW | WS_VISIBLE | BS_CHECKBOX, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_GO_CENTER, GetInstance(), NULL);
    m_goDown = CreateWindow(WC_BUTTON, L"fuck up going &down",
            WS_CHILDWINDOW | WS_VISIBLE | BS_CHECKBOX, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_GO_DOWN, GetInstance(), NULL);
    
    m_messMini = CreateWindow(WC_BUTTON, L"m&ini fuck up",
            WS_CHILDWINDOW | WS_VISIBLE | BS_RADIOBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_MINI_MESS, GetInstance(), NULL);
    m_messNormal = CreateWindow(WC_BUTTON, L"&normal fuck up",
            WS_CHILDWINDOW | WS_VISIBLE | BS_RADIOBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_NORMAL_MESS, GetInstance(), NULL);
    m_messMax = CreateWindow(WC_BUTTON, L"m&axi fuck up",
            WS_CHILDWINDOW | WS_VISIBLE | BS_RADIOBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_MAX_MESS, GetInstance(), NULL);
    m_messCustom = CreateWindow(WC_BUTTON, L"c&ustom fuck up",
            WS_CHILDWINDOW | WS_VISIBLE | BS_RADIOBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_CUSTOM_MESS, GetInstance(), NULL);
    
    m_messLevel = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL,
            WS_CHILDWINDOW | WS_VISIBLE | ES_NUMBER | ES_LEFT | ES_READONLY,
            0, 0, 0, 0, m_hwnd, (HMENU) ZALGO_MESS_LEVEL, GetInstance(), NULL);
    m_messUpDown = CreateWindow(UPDOWN_CLASS, NULL,
            WS_CHILDWINDOW | WS_VISIBLE | UDS_AUTOBUDDY | UDS_SETBUDDYINT |
                UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_HOTTRACK,
            0, 0, 0, 0, m_hwnd, NULL, GetInstance(), NULL);
    
    m_mess = CreateWindow(WC_BUTTON, L"HE &COMES!!!",
            WS_CHILDWINDOW | WS_VISIBLE | BS_DEFPUSHBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_HE_COMES, GetInstance(), NULL);
    m_unmess = CreateWindow(WC_BUTTON, L"HE &GOES!!!",
            WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_HE_GOES, GetInstance(), NULL);
    m_previewShow = CreateWindow(WC_BUTTON, L"&Preview",
            WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_PREVIEW, GetInstance(), NULL);

    PostMessage(m_messUpDown, UDM_SETRANGE32, 1, ZALGO_MESS_LEVEL_OF(ZALGO_MAX_MESS));
    PostMessage(m_messUpDown, UDM_SETPOS32, 0, ZALGO_MESS_LEVEL_OF(ZALGO_NORMAL_MESS));
#define SETFONT(hwnd) PostMessage(hwnd, WM_SETFONT, (WPARAM) hFont, (LPARAM) FALSE)
    SETFONT(m_message);
    SETFONT(m_goUp);
    SETFONT(m_goMiddle);
    SETFONT(m_goDown);
    SETFONT(m_messMini);
    SETFONT(m_messNormal);
    SETFONT(m_messMax);
    SETFONT(m_messCustom);
    SETFONT(m_messLevel);
    SETFONT(m_mess);
    SETFONT(m_unmess);
    SETFONT(m_previewShow);
#undef SETFONT
    
    Button_SetCheck(m_goUp, 1);
    Button_SetCheck(m_goMiddle, 1);
    Button_SetCheck(m_goDown, 1);
    Button_SetCheck(m_messNormal, 1);
    Edit_Enable(m_messLevel, 0);
    
    if (!m_dropTarget.DragDropRegister(m_hwnd))
        MessageBox(m_hwnd, TEXT("Failed to register Drag and Drop handler"),
                   TEXT("Zalgo has COME!!!"), MB_ICONERROR);
    
    m_preview = PreviewWindow::Create(L"Text Preview");
    
    // Subclassing
    wpOrigEditProc = (WNDPROC) SetWindowLongPtr(m_message,
                        GWL_WNDPROC, (LONG_PTR) EditSubclassProc);
    
    return 0;
}

LRESULT MainWindow::OnDestroy()
{
    SetWindowLongPtr(m_message, GWL_WNDPROC, (LONG_PTR) wpOrigEditProc);

    DestroyWindow(m_message);
    DestroyWindow(m_goUp);
    DestroyWindow(m_goMiddle);
    DestroyWindow(m_goDown);
    DestroyWindow(m_messMini);
    DestroyWindow(m_messNormal);
    DestroyWindow(m_messMax);
    DestroyWindow(m_messCustom);
    DestroyWindow(m_messLevel);
    DestroyWindow(m_messUpDown);
    DestroyWindow(m_mess);
    DestroyWindow(m_unmess);
    DestroyWindow(m_previewShow);
    delete m_preview;
    return 0;
}

HICON MainWindow::GetIcon()
{
    return LoadIcon(GetInstance(), MAKEINTRESOURCE(RID_ICON));
}

void MainWindow::OnPaint()
{
    PAINTSTRUCT ps;
    BeginPaint(m_hwnd, &ps);
    PaintContent(&ps);
    EndPaint(m_hwnd, &ps);
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE:
        return OnCreate();
    case WM_DESTROY:
        return OnDestroy();
    case WM_NCDESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        OnPaint();
        return 0;
    case WM_SIZE: {
        RECT client;
        GetClientRect(m_hwnd, &client);
#define REPOS(hwnd, k) SetWindowPos(hwnd, 0, k, SWP_NOACTIVATE|SWP_NOZORDER)
        REPOS(m_message,    LEFT(12, 12, client.right - 24, client.bottom - 94));
        REPOS(m_goUp,       BOTTOM(12, client.bottom - 59, 140, 20));
        REPOS(m_goMiddle,   BOTTOM(12, client.bottom - 34, 140, 20));
        REPOS(m_goDown,     BOTTOM(12, client.bottom - 9,  140, 20));
        REPOS(m_messMini,   BOTTOM(160, client.bottom - 59, 120, 20));
        REPOS(m_messNormal, BOTTOM(160, client.bottom - 34, 120, 20));
        REPOS(m_messMax,    BOTTOM(160, client.bottom - 9,  120, 20));
        REPOS(m_messCustom, BOTTOM(280, client.bottom - 59, 120, 20));
        REPOS(m_messLevel,  BOTTOM(280, client.bottom - 34, 100, 20));
        REPOS(m_mess,       BOTTOMRIGHT(client.right - 12, client.bottom - 41, 100, 25));
        REPOS(m_unmess,     BOTTOMRIGHT(client.right - 12, client.bottom - 12, 100, 25));
        REPOS(m_previewShow,BOTTOMRIGHT(client.right - 117, client.bottom - 12, 100, 25));
#undef REPOS
        PostMessage(m_messUpDown, UDM_SETBUDDY, (WPARAM) m_messLevel, 0);
        return 0;
    }
    case WM_COMMAND:
        switch (HIWORD(wParam)) {
        case BN_CLICKED:
            switch (LOWORD(wParam)) {
            case ZALGO_HE_COMES:
                Button_Enable(m_mess, FALSE);
                Button_Enable(m_unmess, FALSE);
                
                {
                    wchar_t *text;
                    int textlen;
                    wchar_t *zalgo;
                    
                    textlen = Edit_GetTextLength(m_message);
                    text = new wchar_t[textlen+1];
                    Edit_GetText(m_message, text, textlen+1);
                    
                    zalgo = ZalgoComes(text,
                        IsDlgButtonChecked(m_hwnd, ZALGO_GO_UP),
                        IsDlgButtonChecked(m_hwnd, ZALGO_GO_CENTER),
                        IsDlgButtonChecked(m_hwnd, ZALGO_GO_DOWN),
                        GetDlgItemInt(m_hwnd, ZALGO_MESS_LEVEL, NULL, FALSE));
                    
                    Edit_SetText(m_message, zalgo);
                    
                    delete [] zalgo;
                    delete [] text;
                }
                
                Button_Enable(m_mess, TRUE);
                Button_Enable(m_unmess, TRUE);
                break;
            case ZALGO_HE_GOES:
                Button_Enable(m_mess, FALSE);
                Button_Enable(m_unmess, FALSE);
                
                {
                    wchar_t *text;
                    int textlen;
                    wchar_t *zalgo;
                    
                    textlen = Edit_GetTextLength(m_message);
                    zalgo = new wchar_t[textlen+1];
                    Edit_GetText(m_message, zalgo, textlen+1);
                    
                    text = ZalgoGoes(zalgo);
                    
                    Edit_SetText(m_message, text);
                    
                    delete [] text;
                    delete [] zalgo;
                }
                
                Button_Enable(m_mess, TRUE);
                Button_Enable(m_unmess, TRUE);
                break;
            case ZALGO_PREVIEW: {
                wchar_t *text;
                int textlen;
                textlen = Edit_GetTextLength(m_message);
                text = new wchar_t[textlen];
                Edit_GetText(m_message, text, textlen);
                
                SendMessage(m_preview->GetHWND(), WM_CHANGETEXT, 0, (LPARAM) text);
                ShowWindow(m_preview->GetHWND(), SW_SHOW);
            }
            case ZALGO_GO_UP:
            case ZALGO_GO_CENTER:
            case ZALGO_GO_DOWN:
                Button_SetCheck((HWND) lParam, !IsDlgButtonChecked(m_hwnd, LOWORD(wParam)));
                break;
            default:
                Button_SetCheck(GetDlgItem(m_hwnd, ZALGO_MINI_MESS),   0);
                Button_SetCheck(GetDlgItem(m_hwnd, ZALGO_NORMAL_MESS), 0);
                Button_SetCheck(GetDlgItem(m_hwnd, ZALGO_MAX_MESS),    0);
                Button_SetCheck(GetDlgItem(m_hwnd, ZALGO_CUSTOM_MESS), 0);
                Button_SetCheck((HWND) lParam, 1);
                
                switch (LOWORD(wParam)) {
                case ZALGO_MINI_MESS:
                case ZALGO_NORMAL_MESS:
                case ZALGO_MAX_MESS:
                    SetDlgItemInt(m_hwnd, ZALGO_MESS_LEVEL,
                                  ZALGO_MESS_LEVEL_OF(LOWORD(wParam)),
                                  FALSE);
                    Edit_Enable(m_messLevel, 0);
                    break;
                case ZALGO_CUSTOM_MESS:
                    Edit_Enable(m_messLevel, 1);
                }
            }
            break;
        }
        break;
    case WM_CTLCOLORSTATIC: {
        HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
        if ((HWND) lParam == m_messLevel && IsWindowEnabled(m_messLevel))
            return (LRESULT) white;
        SetBkColor((HDC) wParam, BGCOLOUR);
        return (LRESULT) hBrush;
    }
    case WM_KEYDOWN:
        if (wParam == 'K' && GetKeyState(VK_CONTROL) < 0 &&
                GetKeyState(VK_SHIFT) < 0 && GetKeyState(VK_MENU) < 0) {
            static bool unlocked = false;
            // A user has tried to go beyond max, let is work
            if (!unlocked && MessageBox(m_hwnd, L"\
Thou hast tried to loosen my limits, dost thou promise that:\r\n\
  1. thou will be responsible to use the string generated\r\n\
  2. thou will be careful to not enter a number too large that will crash me\r\n\
  3. if thou dost crash me, blame thyself for entering a number too large\r\n\
\r\n\
Dost thou agree?", L"About to Unlock Secret", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                PostMessage(m_messUpDown, UDM_SETRANGE32, 1, LONG_MAX);
                PostMessage(m_messLevel, EM_SETREADONLY, 0, 0);
                
                unlocked = true;
            }
            return 0;
        }
        break;
    case WM_DROPTEXT:
        SendMessage(m_message, EM_REPLACESEL, 0, lParam);
        return 0;
    case WM_DROPFILE:
        //SetWindowText(m_script, (LPTSTR) lParam);
        return 0;
    }
    return __super::HandleMessage(uMsg, wParam, lParam);
}

MainWindow *MainWindow::Create(LPCTSTR szTitle)
{
    MainWindow *self = new MainWindow();
    if (self &&
        self->WinCreateWindow(0,
                szTitle, WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
                NULL, NULL)) {
        return self;
    }
    delete self;
    return NULL;
}
