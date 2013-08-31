#include <OptionParser.h>
#include <ZalgoLib.h>

int main(int argc, char *argv[]) {
    optparse::OptionParser parser = optparse::OptionParser();
    parser.description("Decodes a file encoded with Zalgo!");
    parser.parse_args(argc, argv);
    
    try {
        ZalgoProcess process(parser.args());
        process.open().unzalgo().close();
    } catch (std::string e) {
        std::cerr << "Error: " << e << std::endl;
    }
}
