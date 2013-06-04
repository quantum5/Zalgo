#include <MainWindow.hpp>
#include <nlswrap.hpp>
#include <windowsx.h>
#include <commctrl.h>

#include <string>
#include <unordered_map>


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

const std::pair<wstring, wchar_t> latin_cyrillic_[] = {
    std::make_pair(L"a",  0x0430), std::make_pair(L"b",  0x0431),
    std::make_pair(L"c",  0x0446), std::make_pair(L"ch", 0x0447),
    std::make_pair(L"d",  0x0434), std::make_pair(L"e",  0x0435),
    std::make_pair(L"f",  0x0444), std::make_pair(L"g",  0x0433),
    std::make_pair(L"gh", 0x0445), std::make_pair(L"h",  0x0445),
    std::make_pair(L"i",  0x0438), std::make_pair(L"j",  0x0439),
    std::make_pair(L"k",  0x043A), std::make_pair(L"kh", 0x0445),
    std::make_pair(L"l",  0x043B), std::make_pair(L"m",  0x043C),
    std::make_pair(L"n",  0x043D), std::make_pair(L"o",  0x043E),
    std::make_pair(L"p",  0x043F), std::make_pair(L"q",  0x043A),
    std::make_pair(L"r",  0x0440), std::make_pair(L"s",  0x0441),
    std::make_pair(L"sh", 0x0448), std::make_pair(L"t",  0x0442),
    std::make_pair(L"ts", 0x0446), std::make_pair(L"u",  0x0443),
    std::make_pair(L"v",  0x0432), std::make_pair(L"w",  0x0432),
    std::make_pair(L"x",  0x0449), std::make_pair(L"y",  0x044B),
    std::make_pair(L"ya", 0x044F), std::make_pair(L"z",  0x0437),
    std::make_pair(L"zh", 0x0436),
    std::make_pair(L"A",  0x0410), std::make_pair(L"B",  0x0411),
    std::make_pair(L"C",  0x0426), std::make_pair(L"D",  0x0414),
    std::make_pair(L"E",  0x0415), std::make_pair(L"F",  0x0424),
    std::make_pair(L"G",  0x0413), std::make_pair(L"H",  0x0425),
    std::make_pair(L"I",  0x0418), std::make_pair(L"J",  0x0419),
    std::make_pair(L"K",  0x041A), std::make_pair(L"L",  0x041B),
    std::make_pair(L"M",  0x041C), std::make_pair(L"N",  0x041D),
    std::make_pair(L"O",  0x041E), std::make_pair(L"P",  0x041F),
    std::make_pair(L"Q",  0x041A), std::make_pair(L"R",  0x0420),
    std::make_pair(L"S",  0x0421), std::make_pair(L"T",  0x0422),
    std::make_pair(L"U",  0x0423), std::make_pair(L"V",  0x0412),
    std::make_pair(L"W",  0x0412), std::make_pair(L"X",  0x0429),
    std::make_pair(L"Y",  0x042B), std::make_pair(L"Z",  0x0417),
    std::make_pair(L"CH", 0x0427), std::make_pair(L"GH", 0x0425),
    std::make_pair(L"KH", 0x0425), std::make_pair(L"SH", 0x0428),
    std::make_pair(L"TS", 0x0426), std::make_pair(L"YA", 0x042F),
    std::make_pair(L"ZH", 0x0416),
    std::make_pair(L"Ch", 0x0427), std::make_pair(L"Gh", 0x0425),
    std::make_pair(L"Kh", 0x0425), std::make_pair(L"Sh", 0x0428),
    std::make_pair(L"Ts", 0x0426), std::make_pair(L"Ya", 0x042F),
    std::make_pair(L"Zh", 0x0416),
};

const std::unordered_map<wstring, wchar_t> latin_cyrillic(latin_cyrillic_,
    latin_cyrillic_ + ARRAYSIZE(latin_cyrillic_));

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

inline bool IsZalgo(wchar_t ch)
{
    return ch >= 0x0300 && ch < 0x0370 || ch == 0x489;
}

void MainWindow::OnZalgoComes()
{
    HLOCAL buf = Edit_GetHandle(m_message);
    wstring zalgo;
    bool up   = IsDlgButtonChecked(m_hwnd, ZALGO_GO_UP) != 0,
         mid  = IsDlgButtonChecked(m_hwnd, ZALGO_GO_CENTER) != 0,
         down = IsDlgButtonChecked(m_hwnd, ZALGO_GO_DOWN) != 0;
    int level = GetDlgItemInt(m_hwnd, ZALGO_MESS_LEVEL, NULL, FALSE);
    LPWSTR text = (LPWSTR) LocalLock(buf);

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
    LocalUnlock(buf);
    Edit_SetText(m_message, zalgo.c_str());
}

void MainWindow::OnZalgoGoes()
{
    HLOCAL buf = Edit_GetHandle(m_message);
    wstring text;
    LPWSTR zalgo = (LPWSTR) LocalLock(buf);

    for (; *zalgo; ++zalgo)
        if (!IsZalgo(*zalgo))
            text.push_back(*zalgo);
    LocalUnlock(buf);
    Edit_SetText(m_message, text.c_str());
}

#define ZALGO_NORMALIZE(name, type)  \
    wchar_t *orig, *name = NULL; \
    int bufsize; \
    int textlen; \
    \
    textlen = Edit_GetTextLength(m_message) + 1; \
    orig = new wchar_t[textlen]; \
    Edit_GetText(m_message, orig, textlen); \
    \
    bufsize = ZalgoNormalizeString(type, orig, NULL, 0); \
    if (bufsize <= 0) \
        goto name##cleanup; \
    name = new wchar_t[bufsize]; \
    if (ZalgoNormalizeString(type, orig, name, bufsize) <= 0) \
        goto name##cleanup; \
    Edit_SetText(m_message, name); \
    \
    name##cleanup: \
    if (name) \
        delete name; \
    delete orig;


void MainWindow::OnTextNFC()
{
    ZALGO_NORMALIZE(nfc, MAP_PRECOMPOSED);
}

void MainWindow::OnTextNFD()
{
    ZALGO_NORMALIZE(nfd, MAP_COMPOSITE);
}

void MainWindow::OnTextCyrillic()
{
    HLOCAL buf = Edit_GetHandle(m_message);
    wstring cyrillic, temp;
    LPWSTR text = (LPWSTR) LocalLock(buf);
    
    for (; *text; ++text) {
        temp = wstring(text, 2);
        if (latin_cyrillic.count(temp)) {
            cyrillic.push_back(latin_cyrillic.at(temp));
            ++text;
        } else if (temp.pop_back(), latin_cyrillic.count(temp))
            cyrillic.push_back(latin_cyrillic.at(temp));
        else
            cyrillic.push_back(*text);

        // shch handling
        if (cyrillic.back() == 0x447 || cyrillic.back() == 0x427) {
            wchar_t ch = cyrillic.back();
            cyrillic.pop_back();
            if (cyrillic.back() == 0x448) {
                cyrillic.pop_back();
                cyrillic.push_back(0x449);
            } else if (cyrillic.back() == 0x428) {
                cyrillic.pop_back();
                cyrillic.push_back(0x429);
            } else
                cyrillic.push_back(ch);
        }
    }
    LocalUnlock(buf);
    Edit_SetText(m_message, cyrillic.c_str());
}

void MainWindow::OnTextLatin()
{
}

void MainWindow::OnTextGreek()
{
}
