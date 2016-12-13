#pragma once

#include <memory>
#include <list>
#include <functional>

#include "utils.hh"
namespace Colorlog {

class RuleBox;
class Rule;
class NumberRule;
class Config;

class CLHandler
{
    public: // constant
        static const ColorName DEFAULT_COLOR = RED;
    public: // typedefs
        typedef std::shared_ptr< CLHandler > Ptr;

        // used for tests
        typedef std::function< bool( std::ifstream& ) >ConfFinder;
        typedef std::function<
            std::shared_ptr< Config >( std::istream& ) > ConfigCreator;


    public: /// functions
        CLHandler( int* aArgc, char* aArgv[] );
        std::shared_ptr< RuleBox > produceRules();

        // used mainly for tests
        void setCustomBehaviour( ConfFinder& aConfFinder
                        , ConfigCreator& aConfigCreator );

        bool handleCommand( const std::string& aCommand
                          , const std::string& aValue
                          , std::shared_ptr< RuleBox> & aCurrent );
    protected: /// functions

        void initConfiguration();
        bool isCommandLineOption( const std::string& aOpt );
        void addRuleBoxes( std::shared_ptr< RuleBox >& aCurrent
                         , const std::shared_ptr< RuleBox >& aAdded );
    protected: /// fields
        std::list< std::string > m_Args;

        std::shared_ptr< Config > m_Conf;


        // used mainly for unit tests purposes
        ConfFinder m_ConfFinder;
        ConfigCreator m_ConfCreator;
        int *m_ArgConsumed;

}; // class CLHandler

}
