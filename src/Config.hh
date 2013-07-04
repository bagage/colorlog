#pragma once

#include <memory>
#include <map>

#include "RuleBox.hh"

namespace Color {

class Config
{
    public: // constants
        static const char COMMENT_SIGN = '#';
        static const boost::regex RULE_BOX_REG;
        static const boost::regex NUMBER_RULE_REG;
        static const uint8_t OMIT_FIRST_BRACKET = 1;
        static const uint8_t NUMBER_OF_BRACKETS_RULEBOX = 2;

    public: // typedefs
        typedef std::shared_ptr< Config > Ptr;
        typedef std::map< std::string, RuleBox::Ptr > RuleMap;
        typedef std::pair< std::string, RuleBox::Ptr > RuleMapElem;
        typedef std::function< Rule::Ptr( ColorName
                , const std::string&, bool ) > RuleCreator;
        typedef std::function< NumberRule::Ptr( ColorName
                , const uint8_t ) > NumberRuleCreator;
        typedef std::function< RuleBox::Ptr( void ) > RuleBoxCreator;

    public: // functions
        Config( std::istream& aFile );
        Config( std::istream& aFile
                , RuleCreator aRuleCreator
                , NumberRuleCreator aNumberRuleCreator
                , RuleBoxCreator aRuleBoxCreator );

        virtual const RuleBox::Ptr getRuleBox( const std::string& aName ) const;
        virtual const RuleMap& getAllRules() const;

    protected: // functions
        void parseConfig( std::istream& aStr );

    protected: // fields
        RuleMap m_Rules;
        RuleCreator m_CreateRule;
        NumberRuleCreator m_CreateNumberRule;
        RuleBoxCreator m_CreateRuleBox;

}; // class Config


} // namespace Color