#include <windows.h>

typedef int (WINAPI *FN_NORMALIZESTRING)(UINT NormForm, LPCWSTR lpSrcString,
                                        int cwSrcLength, LPWSTR lpDstString,
                                        int cwDstLength);
FN_NORMALIZESTRING f_NormalizeString = (FN_NORMALIZESTRING)
            GetProcAddress(LoadLibrary(L"normaliz.dll"), "NormalizeString");

UINT NormalizeStringForm(DWORD form)
{
    switch (form) {
        case MAP_PRECOMPOSED:
            return 0x1; // NormalizationC
        case MAP_COMPOSITE:
            return 0x2; // NormalizationD
        default:
            return 0;
    }
}

int ZalgoNormalizeString(DWORD form, LPCWSTR src, LPWSTR dst, int bufsize)
{
    if (f_NormalizeString)
        return f_NormalizeString(NormalizeStringForm(form), src, -1, dst, bufsize);
    else
        return FoldString(form, src, -1, dst, bufsize);
}
