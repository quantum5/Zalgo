#define _CRT_SECURE_NO_WARNINGS
#include <ZalgoLib.h>
#include <random>
#include <string>

namespace Data {
    typedef std::basic_string<wchar_t> wstring;
#   include <ConversionData.inc>
    const wstring zalgo_up   = Data::zalgo_up_;
    const wstring zalgo_down = Data::zalgo_down_;
    const wstring zalgo_mid  = Data::zalgo_mid_;
}
namespace Zalgo {
    std::uniform_int_distribution<> char_up  (0, Data::zalgo_up.length()   - 1);
    std::uniform_int_distribution<> char_mid (0, Data::zalgo_mid.length()  - 1);
    std::uniform_int_distribution<> char_down(0, Data::zalgo_down.length() - 1);
}

ZalgoProcess::ZalgoProcess(const std::vector<std::string> &args) :
        input("-"), output("-")
{
    switch (args.size()) {
        case 0:
            break;
        case 2:
            output = args[1];
        case 1:
            input = args[0];
            break;
        default:
            throw std::string("Error: You may only specify one input file and another output file\n");
    }

}

ZalgoProcess &ZalgoProcess::open() {
    if (input == "-")
        infile = stdin;
    else
        infile = std::fopen(input.c_str(), "r,ccs=UTF-8");
    if (infile == NULL)
        throw std::string("Can't open input file: ") + input;
    
    if (output == "-")
        outfile = stdout;
    else
        outfile = std::fopen(output.c_str(), "w,ccs=UTF-8");
    if (outfile == NULL)
        throw std::string("Can't open output file: ") + output;
    return *this;
}

ZalgoProcess &ZalgoProcess::zalgo(int level, bool up, bool mid, bool down) {
    static std::mt19937 rng;
    std::uniform_int_distribution<> dist_up(level / 3, level);
    std::uniform_int_distribution<> dist_mid(level / 6, level / 2);
    std::uniform_int_distribution<> dist_down(level / 3, level);
    bool initial = true;

    while (true) {
        wint_t ch = fgetwc(infile);
        if (ch == WEOF)
            return *this;
        
        if (ch == 0xFEFF)
            continue;
        if (ch == L'?' && initial) {
            initial = false;
            continue;
        }
        
        fputwc(ch, outfile);
        switch (ch) {
            case L' ':
            case L'\t':
            case L'\r':
            case L'\n':
            case L'\f':
                continue;
        }
#define IMPLEMENT_ZALGO(type) \
        if (type) { \
            int times = dist_##type(rng); \
            for (int i = 0; i < times; ++i) \
                fputwc(Data::zalgo_##type[Zalgo::char_##type(rng)], outfile); \
        }
        IMPLEMENT_ZALGO(up);
        IMPLEMENT_ZALGO(mid);
        IMPLEMENT_ZALGO(down);
    }
}

ZalgoProcess &ZalgoProcess::unzalgo() {
    bool initial = true;
    while (true) {
        wint_t ch = fgetwc(infile);
        if (ch == WEOF)
            return *this;
        
        if (ch == 0xFEFF)
            continue;
        if (ch == L'?' && initial) {
            initial = false;
            continue;
        }
        
        if (ch >= 0x0300 && ch < 0x0370 || ch == 0x489)
            continue;
        fputwc(ch, outfile);
    }
}

ZalgoProcess &ZalgoProcess::close() {
    if (input != "-")
        fclose(infile);
    if (output != "-")
        fclose(outfile);
    return *this;
}
