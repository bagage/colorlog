#include <iostream>

#include "CLHandler.hh"
#include "RuleBox.hh"
#include "PipedChild.hh"

#include <sys/wait.h>

using namespace Color;
int main( int argc, char** argv)
{
    RuleBox::Ptr lAppliedRules;
    size_t lI( 0 );
    try
    {
        // program can be invoked 2 different ways:
        // 1) if color is invoked via color <executable>
        if (isatty(fileno(stdin))) {
            //TODO: yet no parameter is read from color so you cannot suggest scheme to use...
            CLHandler handler( 1, argv );
            lAppliedRules = handler.produceRules();

            PipedChild pc(argv[1], argc-1, &argv[1], NULL);
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
        // 2) if color is invoked via pipe: <executable> | color
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
