#include "CLHandler.hh"
#include "Config.hh"
#include "RuleBox.hh"
#include <fstream>
#include <iostream>

using std::shared_ptr;
using std::function;
using std::string;
using std::istream;

namespace Colorlog {

    static Config::Ptr confCreator( std::istream& aConfFile )
    {
        return Config::Ptr( new Config( aConfFile ) );
    }

    CLHandler::CLHandler( int aArgc, char* aArgv[] )
        : m_ConfFinder( &findConfig )
          , m_ConfCreator( &confCreator )
    {
        static const size_t OMIT_PROGRAM_NAME = 1;
        for ( int lI( OMIT_PROGRAM_NAME ) ; lI < aArgc ; ++lI )
        {
            m_Args.push_back( aArgv[ lI ] );
        }
    }

    RuleBox::Ptr CLHandler::produceRules()
    {
        initConfiguration();

        // read command line arguments
        RuleBox::Ptr lRules;
        while ( !m_Args.empty() )
        {
            std::string lArg( m_Args.front() );
            m_Args.pop_front();

            if ( !isCommandLineOption( lArg ) )
            {
                addRuleBoxes( lRules, m_Conf->getRuleBox( lArg ) );
            }
            else if ( handleCommand( lArg, m_Args.front(), lRules ) )
            {
                m_Args.pop_front();
            }
        }
        // if not found take first one
        if ( !lRules && m_Conf->getFirstRuleBox() )
            lRules = RuleBox::Ptr( m_Conf->getFirstRuleBox() );
        return lRules;
    }

    void CLHandler::setCustomBehaviour( ConfFinder& aConfFinder
            , ConfigCreator& aConfigCreator )
    {
        m_ConfFinder = aConfFinder;
        m_ConfCreator = aConfigCreator;
    }

    void CLHandler::initConfiguration()
    {
        std::ifstream lStr( "colorlog.conf" );
        m_ConfFinder( lStr );
        m_Conf = m_ConfCreator( lStr );
    }

    void CLHandler::addRuleBoxes( RuleBox::Ptr& aCurrent
                                , const RuleBox::Ptr& aAdded )
    {
        if ( aCurrent )
        {
            aCurrent->addBox( std::ref( *aAdded ) );
        }
        else
        {
            aCurrent = aAdded;
        }
    }

    bool CLHandler::isCommandLineOption( const string& aOpt )
    {
        return ( aOpt.size() ) && ( aOpt.at( 0 ) == '-' );
    }

    bool CLHandler::handleCommand( const std::string& aCommand
                      , const std::string& aValue
                      , RuleBox::Ptr& aCurrent )
    {
        if ( aCommand == "-h" || aCommand == "--help" )
        {
            displayHelp();
            exit(0);
        }
        else if ( aCommand == "-s" || aCommand == "--schemes" )
        {
            std::ifstream ifs;
            if (findConfig(ifs))
            {
                Config config(ifs);
                for (auto & kv : config.getAllRules())
                {
                    std::cout << kv.first << std::endl;
                }
            }
            exit(0);
        }
        else if ( aCommand == "-r" || aCommand == "--regex" )
        {
            if ( !m_Args.empty() )
            {
                RuleBox::Ptr lRegexBox( new RuleBox );
                lRegexBox->addRule(
                    Rule::Ptr( new Rule( DEFAULT_COLOR, aValue ) ) );
                addRuleBoxes( aCurrent, lRegexBox );
                return true;
            }
            else
            {
                throw std::runtime_error( "Command line "
                        "argument lacks value '" + aCommand + "'" );
            }


        }
        throw std::runtime_error( "Not supported argument '"
                            "" + aCommand + "' (value '"
                            "" + aValue + "')" );
    }

}
