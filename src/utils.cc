#include "utils.hh"
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>

namespace Color {

const char* HOME_ENV_NAME = "HOME";
const std::string COLOR_HOME_CONF_DIR = "/.color/"
    , COLOR_CONF_FILENAME = "color.conf"
    , COLOR_CONF_DIR = "/etc/";

void color( ColorName aCol, const std::string& aTxt, std::ostream& aStream, ColorName resetCol )
{
    aStream << supported_colors[aCol].ascii_code << aTxt << supported_colors[resetCol].ascii_code;
}

void displayHelp()
{
    std::cerr << "Usage: [some-program] | color [color-scheme] " << std::endl
        << "Examples: " << std::endl
        << "$ cat /var/log/syslog | color syslog" << std::endl
        << "$ make 2>&1 | color gcc" << std::endl << std::endl
        << "Enter man color for more information" << std::endl;
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
    return std::string( lHomePath + COLOR_HOME_CONF_DIR + COLOR_CONF_FILENAME );
}

bool findConfig( std::ifstream& aStr )
{
    static const size_t PATHS_SIZE = 2;
    static const std::string lPaths[ PATHS_SIZE ] = { getHomeConfigPath(), COLOR_CONF_DIR + COLOR_CONF_FILENAME };
    size_t lI( 0 );
    while ( lI < PATHS_SIZE && !aStr.is_open() )
    {
        aStr.open( lPaths[ lI++ ] );
    }
    return aStr.is_open();
}

} // namespace Color
