#include <MainWindow.hpp>

#include <resource.h>
#include <commdlg.h>
#include <windowsx.h>

void MainWindow::OnLoadFile()
{
    OPENFILENAME ofn = { sizeof(OPENFILENAME), 0 };
    WCHAR path[MAX_PATH] = { 0 };
    HANDLE file = INVALID_HANDLE_VALUE, mapping = 0;
    LPSTR data = NULL;
    LPWSTR storage = NULL;
    int codepage = 0
    DWORD size = 0, length = 0;
    
    ofn.hwndOwner = m_hwnd;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = L"Load Text/Zalgo File";
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
    
    if (!GetOpenFileName(&ofn))
        return;
    
    file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
                      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        TellError(L"Failed to Load File", m_hwnd);
        return;
    }
    
    length = GetFileSize(file, NULL);
    if (!length) {
        MessageBox(m_hwnd, L"You have to choose a file with contents!",
                   L"Zero size file", MB_ICONERROR);
        goto cleanup;
    }
    
    mapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
    if (!mapping) {
        TellError(L"Failed to Load File", m_hwnd);
        goto cleanup;
    }
    
    data = (LPSTR) MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
    if (!data) {
        TellError(L"Failed to Load File into Memory", m_hwnd);
        goto cleanup;
    }
    codepage = IsDlgButtonChecked(m_hwnd, ZALGO_USE_UTF8) ? CP_UTF8 : CP_ACP;
    size = MultiByteToWideChar(codepage, 0, data, -1, NULL, 0);
    storage = (LPWSTR) HeapAlloc(GetProcessHeap(), 0, size * sizeof(WCHAR));

    if (!storage) {
        MessageBox(m_hwnd, L"Out of memory/address space!",
                   L"Can't Allocate Memory", MB_ICONERROR);
        goto cleanup;
    }
    
    if (!MultiByteToWideChar(codepage, 0, data, -1, storage, size)) {
        TellError(L"Failed to Convert to UTF-16", m_hwnd);
        goto cleanup;
    }
    
    if (!Edit_SetText(m_message, storage)) {
        MessageBox(m_hwnd, L"Failed to load file into editor",
                   L"Failed to Load File", MB_ICONERROR);
    }
    
    cleanup:
    if (storage)
        HeapFree(GetProcessHeap(), 0, storage);
    if (data)
        UnmapViewOfFile(data);
    if (mapping)
        CloseHandle(mapping);
    if (file != INVALID_HANDLE_VALUE)
        CloseHandle(file);
}

void MainWindow::OnSaveFile()
{
    OPENFILENAME ofn = { sizeof(OPENFILENAME), 0 };
    WCHAR path[MAX_PATH] = { 0 };
    LPWSTR data = NULL;
    LPSTR storage = NULL;
    HANDLE file = INVALID_HANDLE_VALUE;
    HLOCAL buf = NULL;
    int codepage = 0;
    DWORD length = 0, size = 0, written = 0;

    ofn.hwndOwner = m_hwnd;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = L"Save Text/Zalgo File";
    ofn.Flags = OFN_EXPLORER;
    ofn.lpstrDefExt = L"txt";
    
    if (!GetSaveFileName(&ofn))
        return;
    
    buf = Edit_GetHandle(m_message);
    data = (LPWSTR) LocalLock(buf);
    
    codepage = IsDlgButtonChecked(m_hwnd, ZALGO_USE_UTF8) ? CP_UTF8 : CP_ACP;
    size = WideCharToMultiByte(codepage, 0, data, -1, NULL, 0, NULL, NULL);
    storage = (LPSTR) HeapAlloc(GetProcessHeap(), 0, size);

    if (!storage) {
        MessageBox(m_hwnd, L"Out of memory/address space!",
                   L"Can't Allocate Memory", MB_ICONERROR);
        goto cleanup;
    }
    
    if (!WideCharToMultiByte(codepage, 0, data, -1, storage, size, NULL, NULL)) {
        TellError(L"Failed to Convert to Multibyte", m_hwnd);
        goto cleanup;
    }
    
    file = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        TellError(L"Failed to Create Save File", m_hwnd);
        goto cleanup;
    }
    
    length = lstrlenA(storage);
    if (!WriteFile(file, storage, length, &written, NULL))
        TellError(L"Failed to Write to Save File", m_hwnd);
    
    if (written != length)
        MessageBox(m_hwnd, L"File might not be written fully.", L"Warning", MB_ICONWARNING);

    cleanup:
    if (file != INVALID_HANDLE_VALUE)
        CloseHandle(file);
    if (storage)
        HeapFree(GetProcessHeap(), 0, storage);
    if (data)
        LocalUnlock(buf);
}
