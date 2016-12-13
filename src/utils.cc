#include "utils.hh"
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>

namespace Colorlog {

const char* HOME_ENV_NAME = "HOME";
const std::string COLORLOG_HOME_CONF_FILENAME = "/.colorlog.conf"
    , COLORLOG_CONF_FILENAME = "colorlog.conf"
    , COLORLOG_CONF_DIR = "/etc/";

void color( ColorName aCol, const std::string& aTxt, std::ostream& aStream, ColorName resetCol )
{
    aStream << supported_colors[aCol].ascii_code << aTxt << supported_colors[resetCol].ascii_code;
}

void displayHelp()
{
    std::cerr << "Usage: [some-program] | colorlog [colorlog-scheme] " << std::endl
        << "Examples: " << std::endl
        << "$ cat /var/log/syslog | colorlog syslog" << std::endl
        << "$ make 2>&1 | colorlog gcc" << std::endl << std::endl
        << "Enter man colorlog for more information" << std::endl;
}

std::string getHomePath()
{
    char* lPath = getenv( HOME_ENV_NAME );
    if ( lPath != NULL )
    {
        return lPath;
    }

    struct passwd *lPwd = getpwuid(getuid());
    return lPwd->pw_dir;
}

std::string getHomeConfigPath()
{
    std::string lHomePath( getHomePath() );
    return std::string( lHomePath + COLORLOG_HOME_CONF_FILENAME );
}

bool findConfig( std::ifstream& aStr )
{
    static const size_t PATHS_SIZE = 2;
    static const std::string lPaths[ PATHS_SIZE ] = { getHomeConfigPath(), COLORLOG_CONF_DIR + COLORLOG_CONF_FILENAME };
    size_t lI( 0 );
    while ( lI < PATHS_SIZE && !aStr.is_open() )
    {
        aStr.open( lPaths[ lI++ ] );
    }
    return aStr.is_open();
}

}
