#include "colorlogCAPI.h"

#include "CLHandler.hh"
#include "RuleBox.hh"
#include <stdio.h>

ColorlogErrorCode g_Ret;
Colorlog::RuleBox::Ptr g_AppliedRules;

void colorlog_init( int *argc, char** argv )
{
    try
    {
        if ( argc ) // arguments were supported by user
        {
            g_Ret = OK;
            Colorlog::CLHandler lHandler(argc, argv);
            g_AppliedRules = lHandler.produceRules();
        }
        else // use default
        {
            char* lArgs[] = { const_cast< char* >( "colorlog" )
                           , const_cast< char* >( "syslog" ) }; // default
            int lArgc = 2;
            char** lArgv = lArgs;
            g_Ret = INIT_NOT_CALLED_SETTINGS_DEFAULT;
            Colorlog::CLHandler lHandler(&lArgc, lArgv);
            g_AppliedRules = lHandler.produceRules();
        }
    }
    catch ( const std::exception& e )
    {
        puts(e.what());
        g_Ret = FAILED_TO_PARSE_ARGUMENTS;
    }
}

ColorlogErrorCode colorlog_colorize( char* aCString, char* aDest, size_t aDestSize )
{
    try
    {
        static size_t lLineNum( 0 );
        std::string lLine( aCString );
        std::string lRes( g_AppliedRules->process( lLine
                                            , lLineNum++ ) );
        if ( lRes.size() > aDestSize )
        {
            return RESULT_LINE_TOO_LONG;
        }

        strcpy( aDest, lRes.c_str() );
    }
    catch ( const std::exception& e )
    {
        puts(e.what());
        return FAILED_TO_PARSE_ARGUMENTS;
    }
    return g_Ret;
}
