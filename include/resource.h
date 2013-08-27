#define RID_ICON 1
#define RID_XSAMPA 2
#define RID_INIT 3
#define RID_LOOSE 4

inline void GetMessageFont(LOGFONT &lf) {
    NONCLIENTMETRICS ncmMetrics = { sizeof(NONCLIENTMETRICS) };
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncmMetrics, 0);
    memcpy(&lf, &ncmMetrics.lfMessageFont, sizeof lf);
}

inline void TellError(LPWSTR title, HWND hwnd = NULL, DWORD error = (DWORD) -1) {
    LPTSTR message;
    if (error == (DWORD) -1)
        error = GetLastError();
    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, error, 0, (LPWSTR) &message, 0, NULL) == 0) {
        TCHAR str[25];
        wsprintf(str, L"0x%x", error);
        MessageBox(hwnd, str, title, MB_ICONERROR);
    } else {
        MessageBox(hwnd, message, title, MB_ICONERROR);
        LocalFree(message);
    }
}
