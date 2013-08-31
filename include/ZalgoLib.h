#pragma once
#ifndef id890CAB5C_C6A7_4753_9237A1F15D010A89
#define id890CAB5C_C6A7_4753_9237A1F15D010A89

#include <iostream>
#include <cstdio>
#include <vector>

class ZalgoProcess {
public:
    ZalgoProcess() : input("-"), output("-") {}
    ZalgoProcess(const std::string& input, const std::string& output) :
        input(input), output(output) {}
    ZalgoProcess(const std::vector<std::string> &args);
    
    ZalgoProcess &open();
    ZalgoProcess &close();

    ZalgoProcess &zalgo(int level = 10, bool up = true, bool middle = true, bool down = true);
    ZalgoProcess &unzalgo();
protected:
    std::string input, output;
    std::FILE *infile, *outfile;
};

#endif
