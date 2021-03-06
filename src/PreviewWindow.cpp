#include <PreviewWindow.hpp>

#include <resource.h>
#include <windowsx.h>

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

LRESULT PreviewWindow::OnCreate()
{
    LOGFONT lf;
    RECT client;
    GetClientRect(m_hwnd, &client);

    GetMessageFont(lf);
    hFont = CreateFontIndirect(&lf);
    SendMessage(m_hwnd, WM_SETFONT, (WPARAM) hFont, FALSE);

    lpLines = NULL;
    return 0;
}

LRESULT PreviewWindow::OnDestroy()
{
    return 0;
}

HICON PreviewWindow::GetIcon()
{
    return LoadIcon(GetInstance(), MAKEINTRESOURCE(RID_ICON));
}

void PreviewWindow::OnPaint()
{
    PAINTSTRUCT ps;
    SCROLLINFO si;
    RECT rect;
    int FirstLine, LastLine;
    
    BeginPaint(m_hwnd, &ps);
    GetClientRect(m_hwnd, &rect);
    
    if (empty)
        goto done;
    
    si.cbSize = sizeof(si);
    si.fMask  = SIF_POS;
    GetScrollInfo(m_hwnd, SB_VERT, &si);
    yPos = si.nPos;
    
    SelectObject(ps.hdc, hFont);
    SetBkMode(ps.hdc, TRANSPARENT);
    // Find painting limits.
    FirstLine = max(0, yPos + ps.rcPaint.top / yChar);
    LastLine = min(lines - 1, yPos + ps.rcPaint.bottom / yChar);

    // Get horizontal scroll bar position.
    GetScrollInfo(m_hwnd, SB_HORZ, &si);
    xPos = si.nPos;
    for (int i = FirstLine; i <= LastLine; i++) {
        int x = xChar * (1 - xPos);
        int y = yChar * (i - yPos);
        rect.top = y;
        rect.left = x;
        DrawText(ps.hdc, lpLines[i], -1, &rect, DT_EXPANDTABS | DT_NOCLIP | DT_NOPREFIX);
    }
    done:
    EndPaint(m_hwnd, &ps);
}

void PreviewWindow::ChangeText(LPWSTR text, bool padding)
{
    if (lpLines) {
        for (int i = 0; i < lines; ++i)
            delete [] lpLines[i];
        delete [] lpLines;
    }
    
    int l = 0, longest = 0, maxlen = 0;
    LPWSTR str = text;
    
    if (*str == L'\0') {
        empty = true;
        lpLines = NULL;
    } else empty = false;
    
    while (*str++)
        if (*str == L'\n')
            ++l;
    
    if (*(str-1) != L'\n')
        ++l;
    
    lines = l;
    if (padding)
        lines += 8;
    lpLines = new LPTSTR[lines];
    if (padding) {
        for (int i = 0; i < 4; ++i) {
            lpLines[i] = new TCHAR[1];
            lpLines[i][0] = L'\0';
        }
    }

    for (int i = 0; i < l; ++i) {
        LPTSTR start = text, end, buf;
        int len;
        while (*text++ != L'\n' && *(text-1) != L'\0');
        end = text - 1;
        len = end - start;
        if (len > maxlen) {
            longest = i + (padding ? 4 : 0);
            maxlen = len;
        }
        buf = new TCHAR[len + 1];
        memcpy(buf, start, len*sizeof(TCHAR));
        buf[len] = L'\0';
        lpLines[i + (padding ? 4 : 0)] = buf;
    }
    if (padding) {
        for (int i = l + 4; i < lines; ++i) {
            lpLines[i] = new TCHAR[1];
            lpLines[i][0] = L'\0';
        }
    }
    
    int upper = 0, lower = 0;
    for (LPTSTR i = lpLines[longest]; *i; ++i) {
        if (isupper(*i))
            ++upper;
        else if (!(*i >= 0x0300 && *i < 0x0370 || *i == 0x489))
            ++lower;
    }
    xClientMax = lower * xChar + upper * xUpper;
    
    PostMessage(m_hwnd, WM_SIZE, 0, 0);
}

void PreviewWindow::SetFont(HFONT hFont)
{
    if (used_logfont)
        DeleteFont(this->hFont);
    used_logfont = false;
    SendMessage(m_hwnd, WM_SETFONT, (WPARAM) hFont, TRUE);
}

void PreviewWindow::SetFont(const LOGFONT &lf)
{
    if (used_logfont)
        DeleteFont(this->hFont);
    used_logfont = true;
    hFont = CreateFontIndirect(&lf);
    SendMessage(m_hwnd, WM_SETFONT, (WPARAM) hFont, TRUE);
}

LRESULT PreviewWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
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
        SCROLLINFO si;
        
        GetClientRect(m_hwnd, &client);
        yClient = client.bottom - client.top;
        xClient = client.right - client.left;

        // Set the vertical scrolling range and page size
        si.cbSize = sizeof(si);
        si.fMask  = SIF_RANGE | SIF_PAGE;
        si.nMin   = 0;
        si.nMax   = lines - 1;
        si.nPage  = yClient / yChar;
        SetScrollInfo(m_hwnd, SB_VERT, &si, TRUE);

        // Set the horizontal scrolling range and page size.
        si.cbSize = sizeof(si);
        si.fMask  = SIF_RANGE | SIF_PAGE;
        si.nMin   = 0;
        si.nMax   = 2 + xClientMax / xChar;
        si.nPage  = xClient / xChar;
        SetScrollInfo(m_hwnd, SB_HORZ, &si, TRUE);
        return 0;
    }
    case WM_HSCROLL: {
        SCROLLINFO si;
        // Get all the vertial scroll bar information.
        si.cbSize = sizeof(si);
        si.fMask  = SIF_ALL;

        // Save the position for comparison later on.
        GetScrollInfo(m_hwnd, SB_HORZ, &si);
        xPos = si.nPos;
        switch(LOWORD(wParam)) {
        // User clicked the left arrow.
        case SB_LINELEFT:
            si.nPos -= 1;
            break;
        // User clicked the right arrow.
        case SB_LINERIGHT:
            si.nPos += 1;
            break;
        // User clicked the scroll bar shaft left of the scroll box.
        case SB_PAGELEFT:
            si.nPos -= si.nPage;
            break;
        // User clicked the scroll bar shaft right of the scroll box.
        case SB_PAGERIGHT:
            si.nPos += si.nPage;
            break;
        // User dragged the scroll box.
        case SB_THUMBTRACK:
            si.nPos = si.nTrackPos;
            break;
        }
        // Set the position and then retrieve it.  Due to adjustments
        // by Windows it may not be the same as the value set.
        si.fMask = SIF_POS;
        SetScrollInfo(m_hwnd, SB_HORZ, &si, TRUE);
        GetScrollInfo(m_hwnd, SB_HORZ, &si);

        // If the position has changed, scroll the window.
        if(si.nPos != xPos)
            ScrollWindow(m_hwnd, xChar *(xPos - si.nPos), 0, NULL, NULL);
        return 0;
    }
    case WM_VSCROLL: {
        SCROLLINFO si;
        // Get all the vertial scroll bar information.
        si.cbSize = sizeof(si);
        si.fMask  = SIF_ALL;
        GetScrollInfo(m_hwnd, SB_VERT, &si);

        // Save the position for comparison later on.
        yPos = si.nPos;
        switch(LOWORD(wParam)) {
        // User clicked the HOME keyboard key.
        case SB_TOP:
            si.nPos = si.nMin;
            break;
        // User clicked the END keyboard key.
        case SB_BOTTOM:
            si.nPos = si.nMax;
            break;
        // User clicked the top arrow.
        case SB_LINEUP:
            si.nPos -= 1;
            break;
        // User clicked the bottom arrow.
        case SB_LINEDOWN:
            si.nPos += 1;
            break;
        // User clicked the scroll bar shaft above the scroll box.
        case SB_PAGEUP:
            si.nPos -= si.nPage;
            break;
        // User clicked the scroll bar shaft below the scroll box.
        case SB_PAGEDOWN:
            si.nPos += si.nPage;
            break;
        // User dragged the scroll box.
        case SB_THUMBTRACK:
            si.nPos = si.nTrackPos;
            break;
        }
        // Set the position and then retrieve it.  Due to adjustments
        // by Windows it may not be the same as the value set.
        si.fMask = SIF_POS;
        SetScrollInfo(m_hwnd, SB_VERT, &si, TRUE);
        GetScrollInfo(m_hwnd, SB_VERT, &si);

        // If the position has changed, scroll window and update it.
        if(si.nPos != yPos) {
            ScrollWindow(m_hwnd, 0, yChar *(yPos - si.nPos), NULL, NULL);
            UpdateWindow(m_hwnd);
        }
        return 0;
    }
    case WM_MOUSEWHEEL: {
        SCROLLINFO si;
        // Get all the vertial scroll bar information.
        si.cbSize = sizeof(si);
        si.fMask  = SIF_ALL;
        GetScrollInfo(m_hwnd, SB_VERT, &si);
        yPos = si.nPos;
        
        si.nPos -= GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
        si.fMask = SIF_POS;
        SetScrollInfo(m_hwnd, SB_VERT, &si, TRUE);
        GetScrollInfo(m_hwnd, SB_VERT, &si);
        
        if(si.nPos != yPos) {
            ScrollWindow(m_hwnd, 0, yChar *(yPos - si.nPos), NULL, NULL);
            UpdateWindow(m_hwnd);
        }
        return 0;
    }
    case WM_CHANGETEXT:
        ChangeText((LPTSTR) lParam, (wParam & 1) == 0);
        return 0;
    case WM_SETFONT: {
        HDC hdc = GetDC(m_hwnd);
        hFont = (HFONT) wParam;
        HFONT hOldFont = SelectFont(hdc, hFont);
        
        TEXTMETRIC tm;
        // Extract font dimensions from the text metrics.
        GetTextMetrics(hdc, &tm);
        xChar = tm.tmAveCharWidth;
        xUpper =(tm.tmPitchAndFamily & 1 ? 3 : 2) * xChar/2;
        yChar = tm.tmHeight + tm.tmExternalLeading;
        // Set an arbitrary maximum width for client area.
        //(xClientMax is the sum of the widths of 48 average
        // lowercase letters and 12 uppercase letters.)
        xClientMax = 48 * xChar + 12 * xUpper;
        
        SelectFont(hdc, hOldFont);
        ReleaseDC(m_hwnd, hdc);
        break;
    }
    case WM_CLOSE:
        ShowWindow(SW_HIDE);
        return 0;
    }

    return Window::HandleMessage(uMsg, wParam, lParam);
}

PreviewWindow *PreviewWindow::Create(LPCTSTR szTitle)
{
    PreviewWindow *self = new PreviewWindow();
    if(self &&
        self->WinCreateWindow(0,
                szTitle, WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
                NULL, NULL)) {
        return self;
    }
    delete self;
    return NULL;
}
