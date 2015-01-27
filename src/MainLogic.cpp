#include <MainWindow.hpp>
#include <NLSWrap.hpp>
#include <windowsx.h>
#include <commctrl.h>

#include <string>
#include <unordered_map>


typedef std::basic_string<wchar_t> wstring;

#include <ConversionData.inc>

const wstring zalgo_up   = zalgo_up_;
const wstring zalgo_down = zalgo_down_;
const wstring zalgo_mid  = zalgo_mid_;

const std::unordered_map<wstring, wchar_t> latin_cyrillic(latin_cyrillic_,
    latin_cyrillic_ + ARRAYSIZE(latin_cyrillic_));
const std::unordered_map<wchar_t, wstring> to_latin(to_latin_,
    to_latin_ + ARRAYSIZE(to_latin_));
const std::unordered_map<wstring, wchar_t> latin_greek(latin_greek_,
    latin_greek_ + ARRAYSIZE(latin_greek_));
const std::unordered_map<wstring, wchar_t> x_sampa(x_sampa_,
    x_sampa_ + ARRAYSIZE(x_sampa_));

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
        } else if (tolower(*text) == 'c') {
            // special handling of C
            int lower = tolower(*(text + 1));

            temp.clear();
            if (lower == 'k')
                continue;
            else if (lower == 'e' || lower == 'i')
                temp.push_back(*text + 16);
            else
                temp.push_back(*text + 8);
            cyrillic.push_back(latin_cyrillic.at(temp));
            continue;
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
                    latin.push_back(L'y');
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
        } else if (tolower(*text) == 'c') {
            // special handling of C
            int lower = tolower(*(text + 1));

            temp.clear();
            if (lower == 'k')
                continue;
            else if (lower == 'e' || lower == 'i')
                temp.push_back(*text + 16);
            else
                temp.push_back(*text + 8);
            greek.push_back(latin_greek.at(temp));
            continue;
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

void MainWindow::OnTextXSAMPA()
{
    HLOCAL buf = Edit_GetHandle(m_message);
    wstring ipa, temp;
    LPWSTR text = (LPWSTR) LocalLock(buf);

    for (; *text; ++text) {
        temp = wstring(text, 4);

        while (!temp.empty()) {
            if (x_sampa.count(temp)) {
                wchar_t out = x_sampa.at(temp);
                if (out)
                    ipa.push_back(out);
                text += temp.length() - 1;
                break;
            } else temp.pop_back();
        }

        if (temp.empty())
            ipa.push_back(*text);
    }
    LocalUnlock(buf);
    Edit_SetText(m_message, ipa.c_str());
}
