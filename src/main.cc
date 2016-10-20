#include <iostream>

#include "CLHandler.hh"
#include "RuleBox.hh"
#include "PipedChild.hh"

#include <sys/wait.h>

using namespace Colorlog;
int main( int argc, char** argv)
{
    try
    {
        RuleBox::Ptr lAppliedRules;
        size_t lI( 0 );
        // program can be invoked 2 different ways:
        // 1) if colorlog is invoked via colorlog <scheme> <executable>
        if (isatty(fileno(stdin))) {
            int child_idx = 2;
            try {
                // first try to parse the first argument as a possible <scheme>
                CLHandler handler( 2, argv );
                lAppliedRules = handler.produceRules();
            } catch ( ...) {
                // if the first argument is not a scheme, then it's the binary to launch
                child_idx = 1;
                CLHandler handler( 1, argv );
                lAppliedRules = handler.produceRules();
            }

            PipedChild pc(argv[child_idx], argc - child_idx, &argv[child_idx]);
            int fd = pc.stdout;
            FILE* file = fdopen(fd, "r");
            if (file) {
                char *line = NULL;
                size_t n = 0;
                int read;
                int status = 0;
                while (status == 0 && (read = getline(&line, &n, file)) >= 0) {
                    if (read > 0) {
                        std::cout << lAppliedRules->process( line, lI++ ) << std::flush;
                    }
                    waitpid(pc.pid, &status, WNOHANG);
                }
            }
        // 2) if colorlog is invoked via pipe: <executable> | colorlog <scheme>
        } else {
            CLHandler handler( argc, argv );
            lAppliedRules = handler.produceRules();

            std::string lLine;
            while ( std::getline( std::cin, lLine ) )
                if ( lLine.size() )
                    std::cout << lAppliedRules->process( lLine, lI++ ) << std::endl << std::flush;
            }

            return 0;
        }
        catch ( const std::exception& e )
        {
            std::cerr << "Error occurred: " << e.what() << std::endl;
            displayHelp();
        }
        catch ( ... )
        {
            displayHelp();
        }
        return 1;

    }
