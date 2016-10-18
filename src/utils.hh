#pragma once

#include <string>

namespace Color {

enum ColorName {
    RESET = 0,
    BLACK, BOLD_BLACK,
    RED, BOLD_RED,
    GREEN, BOLD_GREEN,
    YELLOW, BOLD_YELLOW,
    BLUE, BOLD_BLUE,
    MAGENTA, BOLD_MAGENTA,
    CYAN, BOLD_CYAN,
    GRAY, BOLD_GRAY,

    // 256 colors mapping - only vte (http://misc.flogisoft.com/bash/tip_colors_and_formatting)
    ORANGE, BG_ORANGE,
    PINK,
};

struct ColorStruct {
    const char* name;
    ColorName ename;
    const char* ascii_code;
};

static const ColorStruct supported_colors[] = {
    { "RESET",        RESET,       "\033[0;0m"      },
    { "BLACK",        BLACK,       "\033[0;30m"     },
    { "BOLD_BLACK",   BOLD_BLACK,  "\033[1;30m"     },
    { "RED",          RED,         "\033[0;31m"     },
    { "BOLD_RED",     BOLD_RED,    "\033[1;31m"     },
    { "GREEN",        GREEN,       "\033[0;32m"     },
    { "BOLD_GREEN",   BOLD_GREEN,  "\033[1;32m"     },
    { "YELLOW",       YELLOW,      "\033[0;33m"     },
    { "BOLD_YELLOW",  BOLD_YELLOW, "\033[1;33m"     },
    { "BLUE",         BLUE,        "\033[0;34m"     },
    { "BOLD_BLUE",    BOLD_BLUE,   "\033[1;34m"     },
    { "MAGENTA",      MAGENTA,     "\033[0;35m"     },
    { "BOLD_MAGENTA", BOLD_MAGENTA,"\033[1;35m"     },
    { "CYAN",         CYAN,        "\033[0;36m"     },
    { "BOLD_CYAN",    BOLD_CYAN,   "\033[1;36m"     },
    { "GRAY",         GRAY,        "\033[0;37m"     },
    { "BOLD_GRAY",    BOLD_GRAY,   "\033[1;37m"     },
    { "ORANGE",       ORANGE,      "\033[38;5;208m" },
    { "BG_ORANGE",    BG_ORANGE,   "\033[48;5;208m" },
    { "PINK",         PINK,        "\033[38;5;218m" },
};

void color( ColorName aCol, const std::string& aTxt, std::ostream& aStream, ColorName resetCol = RESET );

void displayHelp();
bool findConfig( std::ifstream& aStr );


} // namespace Color
