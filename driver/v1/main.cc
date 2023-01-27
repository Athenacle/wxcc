/* main function
* Wang Xiaochun
* zjjhwxc@gmail.com
* January 20, 2013
*
*/

#include "system.h"

#include <fmt/core.h>

#include "lex/lexer.h"
#include "lex/output.h"

#include "driver.h"
#include "parser.h"
#include "symbol.h"
#include "tools.h"
#include "type.h"

int errorCount   = 0;
int warningCount = 0;

USING_V1

void initCompiler(void)
{
    Type::initTO2c();
    NS_BASE_TYPE::InitBaseTypes();
}

FILE* openOutput(const CommandOptions& opt)
{
    FILE* ret = nullptr;

    if (opt.output.length() == 0 || opt.output == "-") {
        ret = stdout;
    } else {
        ret = fopen(opt.output.c_str(), "wb");
        if (ret == nullptr) {
            auto msg =
                fmt::format(FMT("open output file {} failed: {}"), opt.output, strerror(errno));
            fatalError(msg.c_str());
        }
    }
    return ret;
}

int main(int argc, const char* argv[])
{
    CommandOptions opt;
    parseCommandOptions(opt, argc, argv);
    if (wxccErr == nullptr) {
        wxccErr = stdout;
    }

    lex::FileLexInputSource input;
    input.openFile(opt.filename.c_str());

    lex::Lex lexer(&input);

    if (opt.lexerDebug) {
        auto fp = openOutput(opt);
        lex::LexOutput out(lexer, fp);
        out.output();
        fclose(fp);
    } else {
        initCompiler();
        Parser par(lexer);
        Env* globalEnv = new Env();
        globalEnv->setGlobal();
        par.begin_parse(*globalEnv);
    }
    return 0xdeadbeef;
}
