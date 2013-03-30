#include <PreviewWindow.hpp>
#include <Converter.hpp>

#include <resource.h>
#include <windowsx.h>

LRESULT PreviewWindow::OnCreate()
{
    NONCLIENTMETRICS ncmMetrics = { sizeof(NONCLIENTMETRICS) };
    RECT client;

    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncmMetrics, 0);
    GetClientRect(m_hwnd, &client);
    
    hFont = CreateFontIndirect(&ncmMetrics.lfMessageFont);

    // Get the handle to the client area's device context.
    HDC hdc = GetDC(m_hwnd);
    TEXTMETRIC tm;
    // Extract font dimensions from the text metrics.
    GetTextMetrics(hdc, &tm);
    xChar = tm.tmAveCharWidth;
    xUpper =(tm.tmPitchAndFamily & 1 ? 3 : 2) * xChar/2;
    yChar = tm.tmHeight + tm.tmExternalLeading;
    // Free the device context.
    ReleaseDC(m_hwnd, hdc);
    // Set an arbitrary maximum width for client area.
    //(xClientMax is the sum of the widths of 48 average
    // lowercase letters and 12 uppercase letters.)
    xClientMax = 48 * xChar + 12 * xUpper;
    
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
    
    BeginPaint(m_hwnd, &ps);
    GetClientRect(m_hwnd, &rect);
    
    if (empty)
        goto done;
    
    si.cbSize = sizeof(si);
    si.fMask  = SIF_POS;
    GetScrollInfo(m_hwnd, SB_VERT, &si);
    yPos = si.nPos;
    
    SelectObject(ps.hdc, hFont);
    SetBkColor(ps.hdc, RGB(0xF0, 0xF0, 0xF0));
    // Find painting limits.
    int FirstLine = max(0, yPos + ps.rcPaint.top / yChar);
    int LastLine = min(lines - 1, yPos + ps.rcPaint.bottom / yChar);

    // Get horizontal scroll bar position.
    GetScrollInfo(m_hwnd, SB_HORZ, &si);
    xPos = si.nPos;
    for (int i = FirstLine; i <= LastLine; i++) {
        int x = xChar *(1 - xPos);
        int y = yChar *(i - yPos);
        rect.top = y;
        rect.left = x;
        DrawText(ps.hdc, lpLines[i], -1, &rect, DT_NOCLIP);
    }
    done:
    EndPaint(m_hwnd, &ps);
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
    case WM_CHANGETEXT: {
        if (lpLines) {
            for (int i = 0; i < lines; ++i)
                delete [] lpLines[i];
            delete [] lpLines;
        }
        
        LPTSTR str = (LPTSTR) lParam;
        int l = 0, longest = 0, maxlen = 0;
        
        if (*str == L'\0') {
            empty = true;
            lpLines = NULL;
        } else empty = false;
        
        while (*str++)
            if (*str == L'\n')
                ++l;
        
        if (*(str-1) != L'\n')
            ++l;
        
        lines = l + 8;
        lpLines = new LPTSTR[lines];
        for (int i = 0; i < 4; ++i) {
            lpLines[i] = new TCHAR[1];
            lpLines[i][0] = L'\0';
        }
        str = (LPTSTR) lParam;
        for (int i = 0; i < l; ++i) {
            LPTSTR start = str, end, buf;
            int len;
            while (*str++ != L'\n' && *(str-1) != L'\0');
            end = str - 1;
            len = end - start;
            if (len > maxlen) {
                longest = i+4;
                maxlen = len;
            }
            buf = new TCHAR[len + 1];
            memcpy(buf, start, len*sizeof(TCHAR));
            buf[len] = L'\0';
            lpLines[i+4] = buf;
        }
        for (int i = l + 4; i < lines; ++i) {
            lpLines[i] = new TCHAR[1];
            lpLines[i][0] = L'\0';
        }
        
        int upper = 0, lower = 0;
        for (LPTSTR i = lpLines[longest]; *i; ++i) {
            if (isupper(*i))
                ++upper;
            else if (!IsZalgo(*i))
                ++lower;
        }
        xClientMax = lower * xChar + upper * xUpper;
        
        PostMessage(m_hwnd, WM_SIZE, 0, 0);
        return 0;
    }
    case WM_CLOSE:
        ShowWindow(m_hwnd, SW_HIDE);
        return 0;
    }

    return __super::HandleMessage(uMsg, wParam, lParam);
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
