#include <OptionParser.h>
#include <ZalgoLib.h>

int main(int argc, char *argv[]) {
    optparse::OptionParser parser = optparse::OptionParser();
    parser.description("Encodes a file with Zalgo!");
    parser.set_defaults("up", "1");
    parser.set_defaults("middle", "1");
    parser.set_defaults("down", "1");
    
    parser.add_option("-l", "--level").type("int").set_default("10").metavar("LEVEL")
          .help("Zalgo Level (mini=6, normal=10, kazi=14)");
    parser.add_option("-u", "--no-up").action("store_false").dest("up")
          .help("When he comes, he shall not go up");
    parser.add_option("--up").action("store_true").dest("up")
          .help("When he comes, he goes up (default)");
    parser.add_option("-m", "--no-middle").action("store_false").dest("middle")
          .help("When he comes, he shall not mess up the middle");
    parser.add_option("--middle").action("store_true").dest("middle")
          .help("When he comes, he messes up the middle (default)");
    parser.add_option("-d", "--no-down").action("store_false").dest("down")
          .help("When he comes, he shall not go down");
    parser.add_option("--down").action("store_true").dest("down")
          .help("When he comes, he goes down (default)");

    optparse::Values& options = parser.parse_args(argc, argv);
    
    bool up = options.get("up");
    bool middle = options.get("middle");
    bool down = options.get("down");
    int level = options.get("level");
    
    try {
        ZalgoProcess process(parser.args());
        process.open().zalgo(level, up, middle, down).close();
    } catch (std::string e) {
        std::cerr << "Error: " << e << std::endl;
    }
}
