#include <MyDropTarget.hpp>
#include <shellapi.h>

DWORD MyDropTarget::OnDragEnter(POINTL point, DWORD effect, DWORD state,
                                IDataObject *data)
{
    FORMATETC format = {CF_TEXT, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
    
    if (SUCCEEDED(data->QueryGetData(&format)))
        return DROPEFFECT_COPY;
    
    format.cfFormat = CF_HDROP;
    if (SUCCEEDED(data->QueryGetData(&format)))
        return DROPEFFECT_COPY;
    return DROPEFFECT_NONE;
}

DWORD MyDropTarget::OnDrop(POINTL point, DWORD effect, DWORD state,
                           IDataObject *data)
{
    FORMATETC format = {CF_TEXT, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
    STGMEDIUM medium;
    
#ifdef UNICODE
    format.cfFormat = CF_UNICODETEXT;
#endif
    {
        if (FAILED(data->GetData(&format, &medium)))
            goto ansi;
        LPTSTR str = (LPTSTR) GlobalLock(medium.hGlobal);
        SendMessage(m_hwnd, WM_DROPTEXT, 0, (LPARAM) str);
        GlobalUnlock(medium.hGlobal);
        return DROPEFFECT_COPY;
    }
    
    ansi:
#ifdef UNICODE
    {
        format.cfFormat = CF_TEXT;
        if (FAILED(data->GetData(&format, &medium)))
            goto hdrop;
        LPSTR str = (LPSTR) GlobalLock(medium.hGlobal);
        DWORD dwLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0) + 1;
        LPWSTR wstr = new WCHAR[dwLen];
        MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, dwLen);
        SendMessage(m_hwnd, WM_DROPTEXT, 0, (LPARAM) wstr);
        delete [] wstr;
        GlobalUnlock(medium.hGlobal);
        return DROPEFFECT_COPY;
    }
#endif
    
    hdrop:
    {
        format.cfFormat = CF_HDROP;
        if (FAILED(data->GetData(&format, &medium)))
            goto fail;
        TCHAR file[MAX_PATH] = {0};
        DragQueryFile((HDROP) medium.hGlobal, 0, file, MAX_PATH);
        SendMessage(m_hwnd, WM_DROPFILE, (WPARAM) medium.hGlobal, (LPARAM) file);
        return DROPEFFECT_COPY;
    }
    
    fail:
    return DROPEFFECT_NONE;
}
