#pragma once
#ifndef id8B5E818D_F6E4_43FA_889FF93BB24D8D62
#define id8B5E818D_F6E4_43FA_889FF93BB24D8D62

typedef int (WINAPI *FN_NORMALIZESTRING)(UINT NormForm, LPCWSTR lpSrcString,
                                        int cwSrcLength, LPWSTR lpDstString,
                                        int cwDstLength);
FN_NORMALIZESTRING f_NormalizeString = NULL;

inline void ZalgoNormalizeInit()
{
    HMODULE module = LoadLibrary(L"normaliz.dll");
    FARPROC proc = GetProcAddress(module, "NormalizeString");
    if (proc)
        f_NormalizeString = (FN_NORMALIZESTRING) proc;
}

inline UINT NormalizeStringForm(DWORD form)
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

inline int ZalgoNormalizeString(DWORD form, LPCWSTR src, LPWSTR dst, int bufsize)
{
    if (f_NormalizeString)
        return f_NormalizeString(NormalizeStringForm(form), src, -1, dst, bufsize);
    else
        return FoldString(form, src, -1, dst, bufsize);
}

#endif // header
