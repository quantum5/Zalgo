#include <MainWindow.hpp>

#include <resource.h>
#include <commctrl.h>
#include <commdlg.h>
#include <windowsx.h>
#include <shlwapi.h>
#include <shlobj.h>

#include <limits.h>

#define LEFT(x, y, cx, cy) x, y, cx, cy
#define RIGHT(x, y, cx, cy) (x - cx), y, cx, cy
#define BOTTOM(x, y, cx, cy) x, (y - cy), cx, cy
#define BOTTOMRIGHT(x, y, cx, cy) (x - cx), (y - cy), cx, cy

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
DWORD rgbWindowBackground;

wchar_t *GetResourceString(int id, HMODULE module = NULL) {
    HRSRC hRC;
    DWORD size;
    HGLOBAL hRes;
    wchar_t *data;

    hRC = FindResource(module, MAKEINTRESOURCE(id), L"ZALGO_TEXT");
    if (!hRC)
        return NULL;
    size = SizeofResource(module, hRC);
    hRes = LoadResource(module, hRC);
    data = new wchar_t[size / sizeof(wchar_t) + 1];
    memcpy(data, LockResource(hRes), size);
    data[size / sizeof(wchar_t)] = 0;
    return data;
}

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

BOOL MainWindow::WinRegisterClass(WNDCLASS *pwc)
{
    pwc->style = CS_HREDRAW | CS_VREDRAW;
    return __super::WinRegisterClass(pwc);
}

LRESULT MainWindow::OnCreate()
{
    LOGFONT lf;
    RECT client;
    LPWSTR initial = NULL;

    GetClientRect(m_hwnd, &client);
    GetMessageFont(lf);
    hFont = CreateFontIndirect(&lf);
    hBrush = GetSysColorBrush(COLOR_WINDOW);
    rgbWindowBackground = GetSysColor(COLOR_WINDOW);

    initial = GetResourceString(RID_INIT);
    if (!initial)
        MessageBox(m_hwnd, L"Zalgo initial not found", L"Zalgo Error", MB_ICONERROR);
    // Children
    m_message = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT,
            initial, WS_CHILDWINDOW | WS_VISIBLE | ES_LEFT |
                  ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
            0, 0, 0, 0, m_hwnd, (HMENU) ZALGO_MESSAGE, GetInstance(), NULL);
    if (initial)
        delete [] initial;

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
    m_messMax = CreateWindow(WC_BUTTON, L"k&azi fuck up",
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

    m_nfc = CreateWindow(WC_BUTTON, L"N&FC",
            WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) TEXT_TO_NFC, GetInstance(), NULL);
    m_nfd = CreateWindow(WC_BUTTON, L"NF&D",
            WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) TEXT_TO_NFD, GetInstance(), NULL);

    m_latin = CreateWindow(WC_BUTTON, L"Use &Latin",
            WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_LATIN, GetInstance(), NULL);
    m_cyrillic = CreateWindow(WC_BUTTON, L"Use C&yrillic",
            WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_CYRILLIC, GetInstance(), NULL);
    m_greek = CreateWindow(WC_BUTTON, L"Use G&reek",
            WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_GREEK, GetInstance(), NULL);
    m_xsampa = CreateWindow(WC_BUTTON, L"&X-SAMPA to IPA",
            WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_XSAMPA, GetInstance(), NULL);
    m_xsampa_table = CreateWindow(WC_BUTTON, L"X-&SAMPA Table",
            WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0,
            m_hwnd, (HMENU) ZALGO_XSAMPA_TABLE, GetInstance(), NULL);

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
    SETFONT(m_nfc);
    SETFONT(m_nfd);
    SETFONT(m_latin);
    SETFONT(m_cyrillic);
    SETFONT(m_greek);
    SETFONT(m_xsampa);
    SETFONT(m_xsampa_table);
#undef SETFONT

    Button_SetCheck(m_goUp, 1);
    Button_SetCheck(m_goMiddle, 1);
    Button_SetCheck(m_goDown, 1);
    Button_SetCheck(m_messNormal, 1);
    Edit_Enable(m_messLevel, 0);

    if (!m_dropTarget.DragDropRegister(m_hwnd))
        MessageBox(m_hwnd, L"Failed to register Drag and Drop handler",
                   L"Zalgo has COME!!!", MB_ICONERROR);

    m_preview = PreviewWindow::Create(L"Text Preview");
    m_data_display = PreviewWindow::Create(L"X-SAMPA Table");
    lf.lfHeight = (LONG) (lf.lfHeight * 1.3);
    lf.lfWidth  = (LONG) (lf.lfWidth * 1.3);
    m_data_display->SetFont(lf);

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
    DestroyWindow(m_nfc);
    DestroyWindow(m_nfd);
    DestroyWindow(m_latin);
    DestroyWindow(m_cyrillic);
    DestroyWindow(m_greek);
    DestroyWindow(m_xsampa);
    DestroyWindow(m_xsampa_table);
    delete m_preview;
    delete m_data_display;
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
        HDWP hdwp;
        GetClientRect(m_hwnd, &client);
#define REPOS(hwnd, k) hdwp = DeferWindowPos(hdwp, hwnd, 0, k, SWP_NOACTIVATE|SWP_NOZORDER)
        hdwp = BeginDeferWindowPos(14);
        REPOS(m_message,      LEFT(12, 12, client.right - 24, client.bottom - 124));
        REPOS(m_goUp,         BOTTOM(12, client.bottom - 59, 140, 20));
        REPOS(m_goMiddle,     BOTTOM(12, client.bottom - 34, 140, 20));
        REPOS(m_goDown,       BOTTOM(12, client.bottom - 9,  140, 20));
        REPOS(m_messMini,     BOTTOM(160, client.bottom - 59, 120, 20));
        REPOS(m_messNormal,   BOTTOM(160, client.bottom - 34, 120, 20));
        REPOS(m_messMax,      BOTTOM(160, client.bottom - 9,  120, 20));
        REPOS(m_messCustom,   BOTTOM(280, client.bottom - 59, 120, 20));
        REPOS(m_messLevel,    BOTTOM(280, client.bottom - 38, 84, 20));
        REPOS(m_messUpDown,   BOTTOM(362, client.bottom - 38, 18, 20));
        REPOS(m_mess,         BOTTOMRIGHT(client.right - 12, client.bottom - 41, 100, 25));
        REPOS(m_unmess,       BOTTOMRIGHT(client.right - 12, client.bottom - 12, 100, 25));
        REPOS(m_nfc,          BOTTOMRIGHT(client.right - 117, client.bottom - 41, 50, 25));
        REPOS(m_nfd,          BOTTOMRIGHT(client.right - 117, client.bottom - 12, 50, 25));
        REPOS(m_previewShow,  BOTTOMRIGHT(client.right - 172, client.bottom - 12, 100, 25));
        REPOS(m_latin,        BOTTOM(12, client.bottom - 84, 100, 25));
        REPOS(m_cyrillic,     BOTTOM(117, client.bottom - 84, 100, 25));
        REPOS(m_greek,        BOTTOM(222, client.bottom - 84, 100, 25));
        REPOS(m_xsampa,       BOTTOM(327, client.bottom - 84, 100, 25));
        REPOS(m_xsampa_table, BOTTOM(432, client.bottom - 84, 100, 25));
        EndDeferWindowPos(hdwp);
#undef REPOS
        return 0;
    }
    case WM_COMMAND:
        switch (HIWORD(wParam)) {
        case BN_CLICKED:
            switch (LOWORD(wParam)) {
            case ZALGO_HE_COMES:
                OnZalgoComes();
                break;
            case ZALGO_HE_GOES:
                OnZalgoGoes();
                break;
            case ZALGO_PREVIEW: {
                wchar_t *text;
                int textlen;
                textlen = Edit_GetTextLength(m_message) + 1;
                text = new wchar_t[textlen];
                Edit_GetText(m_message, text, textlen);

                m_preview->ChangeText(text);
                m_preview->ShowWindow(SW_SHOW);
                break;
            }
            case TEXT_TO_NFC:
                OnTextNFC();
                break;
            case TEXT_TO_NFD:
                OnTextNFD();
                break;
            case ZALGO_LATIN:
                OnTextLatin();
                break;
            case ZALGO_CYRILLIC:
                OnTextCyrillic();
                break;
            case ZALGO_GREEK:
                OnTextGreek();
                break;
            case ZALGO_XSAMPA:
                OnTextXSAMPA();
                break;
            case ZALGO_GO_UP:
            case ZALGO_GO_CENTER:
            case ZALGO_GO_DOWN:
                Button_SetCheck((HWND) lParam, !IsDlgButtonChecked(m_hwnd, LOWORD(wParam)));
                break;
            case ZALGO_XSAMPA_TABLE: {
                wchar_t *text;

                SetWindowText(m_data_display->GetHWND(), L"X-SAMPA Table");
                text = GetResourceString(RID_XSAMPA);
                if (!text) {
                    MessageBox(m_hwnd, L"X-SAMPA table not found", L"X-SAMPA Error", MB_ICONERROR);
                    break;
                }
                m_data_display->ChangeText(text, false);
                m_data_display->ShowWindow(SW_SHOW);
                SetForegroundWindow(*m_data_display);
                delete [] text;
                break;
            }
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
        SetBkColor((HDC) wParam, rgbWindowBackground);
        return (LRESULT) hBrush;
    }
    case WM_KEYDOWN:
        if (wParam == 'K' && GetKeyState(VK_CONTROL) < 0 &&
                GetKeyState(VK_SHIFT) < 0 && GetKeyState(VK_MENU) < 0) {
            static bool unlocked = false;
            LPWSTR text = GetResourceString(RID_LOOSE);
            // A user has tried to go beyond max, let it work
            if (!unlocked && MessageBox(m_hwnd, text,L"About to Unlock Secret",
                    MB_YESNO | MB_ICONQUESTION) == IDYES) {
                PostMessage(m_messUpDown, UDM_SETRANGE32, 1, LONG_MAX);
                PostMessage(m_messLevel, EM_SETREADONLY, 0, 0);

                unlocked = true;
            }
            delete [] text;
            return 0;
        }
        break;
    case WM_DROPTEXT:
        SendMessage(m_message, EM_REPLACESEL, 0, lParam);
        return 0;
    case WM_ENTERSIZEMOVE:
        SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, GetWindowLongPtr(m_hwnd, GWL_EXSTYLE) | WS_EX_COMPOSITED);
        return 0;
    case WM_EXITSIZEMOVE:
        SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, GetWindowLongPtr(m_hwnd, GWL_EXSTYLE) & ~WS_EX_COMPOSITED);
        return 0;
    }
    return __super::HandleMessage(uMsg, wParam, lParam);
}

MainWindow *MainWindow::Create(LPCTSTR szTitle)
{
    MainWindow *self = new MainWindow();
    if (self &&
        self->WinCreateWindow(0,
                szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
                NULL, NULL)) {
        return self;
    }
    delete self;
    return NULL;
}
