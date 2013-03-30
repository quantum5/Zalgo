#include <string>
#include <cstdlib>
#include <strsafe.h>

#include <Converter.hpp>

typedef std::basic_string<wchar_t> wstring;

const wchar_t zalgo_up_[] = {
    0x030d, 0x030e, 0x0304, 0x0305, 0x033f, 0x0311, 0x0306, 0x0310, 0x0352,
    0x0357, 0x0351, 0x0307, 0x0308, 0x030a, 0x0342, 0x0343, 0x0344, 0x034a,
    0x034b, 0x034c, 0x0303, 0x0302, 0x030c, 0x0350, 0x0300, 0x0301, 0x030b,
    0x030f, 0x0312, 0x0313, 0x0314, 0x033d, 0x0309, 0x0363, 0x0364, 0x0365,
    0x0366, 0x0367, 0x0368, 0x0369, 0x036a, 0x036b, 0x036c, 0x036d, 0x036e,
    0x036f, 0x033e, 0x035b, 0x0346, 0x031a, 0
};

const wchar_t zalgo_down_[] = {
    0x0316, 0x0317, 0x0318, 0x0319, 0x031c, 0x031d, 0x031e, 0x031f, 0x0320,
    0x0324, 0x0325, 0x0326, 0x0329, 0x032a, 0x032b, 0x032c, 0x032d, 0x032e,
    0x032f, 0x0330, 0x0331, 0x0332, 0x0333, 0x0339, 0x033a, 0x033b, 0x033c,
    0x0345, 0x0347, 0x0348, 0x0349, 0x034d, 0x034e, 0x0353, 0x0354, 0x0355,
    0x0356, 0x0359, 0x035a, 0x0323, 0
};

const wchar_t zalgo_mid_[] = {
    0x0315, 0x031b, 0x0340, 0x0341, 0x0358, 0x0321, 0x0322, 0x0327, 0x0328,
    0x0334, 0x0335, 0x0336, 0x034f, 0x035c, 0x035d, 0x035e, 0x035f, 0x0360,
    0x0362, 0x0338, 0x0337, 0x0361, 0x0489, 0
};

const wstring zalgo_up   = zalgo_up_;
const wstring zalgo_down = zalgo_down_;
const wstring zalgo_mid  = zalgo_mid_;

inline int randint(int min, int max)
{
    if (min == max)
        return min;
    if (min >= max)
        return 0;
    return min + rand() % (max - min);
}

inline wchar_t randchar(const wstring &str)
{
    return str[rand() % str.length()];
}

wchar_t *ZalgoComes(const wchar_t *text, bool up, bool mid, bool down, int level)
{
    wstring zalgo;
    for (; *text; ++text) {
        if (IsZalgo(*text))
            continue;
        zalgo.push_back(*text);
        if (*text < L' ')
            continue;
        int count_up = randint(level / 3, level),
            count_mid = randint(level / 6, level / 2),
            count_down = randint(level / 3, level);
#define IMPLEMENT_ZALGO(type) \
        if (type) {\
            for (int i = 0; i < count_##type; ++i) \
                zalgo.push_back(randchar(zalgo_##type)); \
        }
        IMPLEMENT_ZALGO(up);
        IMPLEMENT_ZALGO(mid);
        IMPLEMENT_ZALGO(down);
    }
    wchar_t *out = new wchar_t[zalgo.length() + 1];
    StringCchCopy(out, zalgo.length() + 1, zalgo.c_str());
    return out;
}

wchar_t *ZalgoGoes(const wchar_t *zalgo)
{
    wstring text;
    for (; *zalgo; ++zalgo)
        if (!IsZalgo(*zalgo))
            text += *zalgo;
    wchar_t *out = new wchar_t[text.length() + 1];
    StringCchCopy(out, text.length() + 1, text.c_str());
    return out;

}
