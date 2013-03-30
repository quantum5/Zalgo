#pragma once
#ifndef id76AD7CF2_E519_495C_8C6B138352825E12
#define id76AD7CF2_E519_495C_8C6B138352825E12

inline bool IsZalgo(wchar_t ch)
{
    return ch >= 0x0300 && ch < 0x0370 || ch == 0x489;
}

wchar_t *ZalgoComes(const wchar_t *text, bool up, bool mid, bool down, int level);
wchar_t *ZalgoGoes(const wchar_t *zalgo);

#endif
