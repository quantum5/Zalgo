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

const std::pair<wchar_t, wstring> to_latin_[] = {
    std::make_pair(0x0430, L"a"), std::make_pair(0x0431, L"b"),
    std::make_pair(0x0432, L"v"), std::make_pair(0x0433, L"g"),
    std::make_pair(0x0434, L"d"), std::make_pair(0x0435, L"e"),
    std::make_pair(0x0451, L"e"), std::make_pair(0x0436, L"zh"),
    std::make_pair(0x0437, L"z"), std::make_pair(0x0438, L"i"),
    std::make_pair(0x0439, L"j"), std::make_pair(0x043a, L"k"),
    std::make_pair(0x043b, L"l"), std::make_pair(0x043c, L"m"),
    std::make_pair(0x043d, L"n"), std::make_pair(0x043e, L"o"),
    std::make_pair(0x043f, L"p"), std::make_pair(0x0440, L"r"),
    std::make_pair(0x0441, L"s"), std::make_pair(0x0442, L"t"),
    std::make_pair(0x0443, L"u"), std::make_pair(0x0444, L"f"),
    std::make_pair(0x0445, L"h"), std::make_pair(0x0446, L"ts"),
    std::make_pair(0x0447, L"ch"),std::make_pair(0x0448, L"sh"),
    std::make_pair(0x044a, L"\""),std::make_pair(0x0449, L"shch"),
    std::make_pair(0x044b, L"y"), std::make_pair(0x044c, L"'"),
    std::make_pair(0x044d, L"e"), std::make_pair(0x044e, L"yu"),
    std::make_pair(0x044f, L"ya"),std::make_pair(0x0456, L"i"),
    std::make_pair(0x0473, L"f"), std::make_pair(0x0463, L"ye"),
    std::make_pair(0x0475, L"i"), std::make_pair(0x0455, L"dz"),
    std::make_pair(0x046f, L"ks"),std::make_pair(0x0471, L"ps"),
    std::make_pair(0x0461, L"o"), std::make_pair(0x046b, L"u"),
    std::make_pair(0x0410, L"A"), std::make_pair(0x0411, L"B"),
    std::make_pair(0x0412, L"V"), std::make_pair(0x0413, L"G"),
    std::make_pair(0x0414, L"D"), std::make_pair(0x0415, L"E"),
    std::make_pair(0x0401, L"E"), std::make_pair(0x0416, L"Zh"),
    std::make_pair(0x0417, L"Z"), std::make_pair(0x0418, L"I"),
    std::make_pair(0x0419, L"J"), std::make_pair(0x041a, L"K"),
    std::make_pair(0x041b, L"L"), std::make_pair(0x041c, L"M"),
    std::make_pair(0x041d, L"N"), std::make_pair(0x041e, L"O"),
    std::make_pair(0x041f, L"P"), std::make_pair(0x0420, L"R"),
    std::make_pair(0x0421, L"S"), std::make_pair(0x0422, L"T"),
    std::make_pair(0x0423, L"U"), std::make_pair(0x0424, L"F"),
    std::make_pair(0x0425, L"H"), std::make_pair(0x0426, L"Ts"),
    std::make_pair(0x0427, L"Ch"),std::make_pair(0x0428, L"Sh"),
    std::make_pair(0x042a, L"\""),std::make_pair(0x0429, L"Shch"),
    std::make_pair(0x042b, L"Y"), std::make_pair(0x042c, L"'"),
    std::make_pair(0x042d, L"E"), std::make_pair(0x042e, L"Yu"),
    std::make_pair(0x042f, L"Ya"),std::make_pair(0x0406, L"I"),
    std::make_pair(0x0472, L"F"), std::make_pair(0x0462, L"Ye"),
    std::make_pair(0x0474, L"I"), std::make_pair(0x0405, L"Dz"),
    std::make_pair(0x046e, L"Ks"),std::make_pair(0x0470, L"Ps"),
    std::make_pair(0x0460, L"O"), std::make_pair(0x046a, L"U"),
    std::make_pair(0x03b1, L"a"), std::make_pair(0x03b2, L"b"),
    std::make_pair(0x03b3, L"g"), std::make_pair(0x03b4, L"d"),
    std::make_pair(0x03b5, L"e"), std::make_pair(0x03b6, L"z"),
    std::make_pair(0x03b7, L"e"), std::make_pair(0x03b8, L"th"),
    std::make_pair(0x03b9, L"i"), std::make_pair(0x03ba, L"k"),
    std::make_pair(0x03bb, L"l"), std::make_pair(0x03bc, L"m"),
    std::make_pair(0x03bd, L"n"), std::make_pair(0x03be, L"x"),
    std::make_pair(0x03bf, L"o"), std::make_pair(0x03c0, L"p"),
    std::make_pair(0x03c1, L"r"), std::make_pair(0x03c3, L"s"),
    std::make_pair(0x03c4, L"t"), std::make_pair(0x03c5, L"u"),
    std::make_pair(0x03c6, L"f"), std::make_pair(0x03c7, L"ch"),
    std::make_pair(0x03c8, L"ps"),std::make_pair(0x03c9, L"o"),
    std::make_pair(0x0391, L"A"), std::make_pair(0x0392, L"B"),
    std::make_pair(0x0393, L"G"), std::make_pair(0x0394, L"D"),
    std::make_pair(0x0395, L"E"), std::make_pair(0x0396, L"Z"),
    std::make_pair(0x0397, L"E"), std::make_pair(0x0398, L"Th"),
    std::make_pair(0x0399, L"I"), std::make_pair(0x039a, L"K"),
    std::make_pair(0x039b, L"L"), std::make_pair(0x039c, L"M"),
    std::make_pair(0x039d, L"N"), std::make_pair(0x039e, L"X"),
    std::make_pair(0x039f, L"O"), std::make_pair(0x03a0, L"P"),
    std::make_pair(0x03a1, L"R"), std::make_pair(0x03a3, L"S"),
    std::make_pair(0x03a4, L"T"), std::make_pair(0x03a5, L"U"),
    std::make_pair(0x03a6, L"F"), std::make_pair(0x03a7, L"Ch"),
    std::make_pair(0x03a8, L"Ps"),std::make_pair(0x03a9, L"O"),
    std::make_pair(0x03c2, L"s"),
};

const std::pair<wstring, wchar_t> latin_greek_[] = {
    std::make_pair(L"a", 0x03b1), std::make_pair(L"b", 0x03b2),
    std::make_pair(L"c", 0x03c7), std::make_pair(L"d", 0x03b4),
    std::make_pair(L"e", 0x03b5), std::make_pair(L"f", 0x03c6),
    std::make_pair(L"g", 0x03b3), std::make_pair(L"h", 0x03c7),
    std::make_pair(L"i", 0x03b9), std::make_pair(L"j", 0x03b7),
    std::make_pair(L"k", 0x03ba), std::make_pair(L"l", 0x03bb),
    std::make_pair(L"m", 0x03bc), std::make_pair(L"n", 0x03bd),
    std::make_pair(L"o", 0x03bf), std::make_pair(L"p", 0x03c0),
    std::make_pair(L"ps", 0x03c8),std::make_pair(L"q", 0x03ba),
    std::make_pair(L"r", 0x03c1), std::make_pair(L"s", 0x03c3),
    std::make_pair(L"t", 0x03c4), std::make_pair(L"th", 0x03b8),
    std::make_pair(L"u", 0x03c5), std::make_pair(L"v", 0x03c6),
    std::make_pair(L"w", 0x03c5), std::make_pair(L"x", 0x03be),
    std::make_pair(L"y", 0x03c5), std::make_pair(L"z", 0x03b6),
    std::make_pair(L"A", 0x0391), std::make_pair(L"B", 0x0392),
    std::make_pair(L"C", 0x03a7), std::make_pair(L"D", 0x0394),
    std::make_pair(L"E", 0x0395), std::make_pair(L"F", 0x03a6),
    std::make_pair(L"G", 0x0393), std::make_pair(L"H", 0x03a7),
    std::make_pair(L"I", 0x0399), std::make_pair(L"J", 0x0397),
    std::make_pair(L"K", 0x039a), std::make_pair(L"L", 0x039b),
    std::make_pair(L"M", 0x039c), std::make_pair(L"N", 0x039d),
    std::make_pair(L"O", 0x039f), std::make_pair(L"P", 0x03a0),
    std::make_pair(L"Ps", 0x03a8),std::make_pair(L"Q", 0x039a),
    std::make_pair(L"R", 0x03a1), std::make_pair(L"S", 0x03a3),
    std::make_pair(L"U", 0x03a5), std::make_pair(L"V", 0x03a6),
    std::make_pair(L"W", 0x03a5), std::make_pair(L"X", 0x039e),
    std::make_pair(L"Y", 0x03a5), std::make_pair(L"Z", 0x0396),
    std::make_pair(L"T", 0x03a4), std::make_pair(L"Th", 0x0398),
    std::make_pair(L"PS", 0x03a8),std::make_pair(L"TH", 0x0398),
    
    std::make_pair(L"ch", 0x03c7),std::make_pair(L"CH", 0x03a7),
    std::make_pair(L"Ch", 0x03a7),
};

const std::unordered_map<wstring, wchar_t> latin_cyrillic(latin_cyrillic_,
    latin_cyrillic_ + ARRAYSIZE(latin_cyrillic_));
const std::unordered_map<wchar_t, wstring> to_latin(to_latin_,
    to_latin_ + ARRAYSIZE(to_latin_));
const std::unordered_map<wstring, wchar_t> latin_greek(latin_greek_,
    latin_greek_ + ARRAYSIZE(latin_greek_));

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
    HLOCAL buf = Edit_GetHandle(m_message);
    wstring latin, temp;
    LPWSTR text = (LPWSTR) LocalLock(buf);
    
    for (; *text; ++text) {
        if (*(text + 1) == 0x0419 || *(text + 1) == 0x0439) {
            if (*text == 0x0418 || *text == 0x0438 ||
                *text == 0x042b || *text == 0x042b) {
                // -ij to -y
                if (*text & 0x20)
                    latin.push_back(L'Y');
                else
                    latin.push_back(L'Y');
                ++text;
                continue;
            }
        } else if (to_latin.count(*text))
            latin += to_latin.at(*text);
        else
            latin.push_back(*text);
    }
    LocalUnlock(buf);
    Edit_SetText(m_message, latin.c_str());
}

void MainWindow::OnTextGreek()
{
    HLOCAL buf = Edit_GetHandle(m_message);
    wstring greek, temp;
    LPWSTR text = (LPWSTR) LocalLock(buf);
    
    for (; *text; ++text) {
        temp = wstring(text, 2);
        if (latin_greek.count(temp)) {
            greek.push_back(latin_greek.at(temp));
            ++text;
        } else if (temp.pop_back(), latin_greek.count(temp))
            greek.push_back(latin_greek.at(temp));
        else
            greek.push_back(*text);
        
        if (greek.back() == 0x03c3 && (*(text+1) < 'A' || *(text+1) > 'Z') &&
                (*(text+1) < 'a' || *(text+1) > 'z')) {
            greek.pop_back();
            greek.push_back(0x03c2);
        }
    }
    LocalUnlock(buf);
    Edit_SetText(m_message, greek.c_str());
}
