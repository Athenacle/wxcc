#include "config.h"

#include <fmt/core.h>

#include <argparse/argparse.hpp>

#include "driver.h"

FILE* openOutput(const CommandOptions& opt, utils::ErrorManager& mgr)
{
    FILE* ret = nullptr;

    if (opt.output.length() == 0 || opt.output == "-") {
        ret = stdout;
    } else {
        ret = fopen(opt.output.c_str(), "wb");
        if (ret == nullptr) {
            mgr.fatal(FMT("open output file {} failed: {}"), opt.output, strerror(errno));
        }
    }
    return ret;
}

bool parseCommandOptions(CommandOptions& opt, int argc, const char* argv[])
{
    argparse::ArgumentParser prog(PROJECT_NAME,
                                  PROJECT_VERSION_STRING
#ifdef PROJECT_GIT_SHA
                                  "-" PROJECT_GIT_SHA
#endif
    );
    prog.add_description("Simple C Compiler");

    prog.add_argument("--lex-debug")
        .help("increase output verbosity")
        .default_value(false)
        .implicit_value(true);

    prog.add_argument("-o", "--output").help("Result file").implicit_value("a.out");


    prog.add_argument("filename").required().remaining().nargs(1);

    try {
        prog.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << prog;
        std::exit(1);
    }

#define IF(para, dest, type)                 \
    do {                                     \
        if (prog.is_used(para)) {            \
            opt.dest = prog.get<type>(para); \
        }                                    \
    } while (false)

    try {
        auto files     = prog.get<std::vector<std::string>>("filename");

        opt.filename   = files.at(0);

        opt.lexerDebug = prog["--lex-debug"] == true;

        IF("--output", output, std::string);

    } catch (const std::logic_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << prog;
        std::exit(1);
    }

    return true;
}
