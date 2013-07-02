#pragma once

#include <tr1/memory>

#include "utils.hh"
#include "Rules.hh"

namespace Color {

class RuleBox
{
    public: // typedefs
        typedef std::tr1::shared_ptr< RuleBox > Ptr;

    public: // functions
        RuleBox();
        void addRule( const IRule::Ptr& aRule );
        std::string process( const std::string& aText, const uint64_t aLineNumber = 0 );

    protected: // functions
        void singleProcess( const IRule::Ptr& aRule
                        , IntermediateResult& aResult
                        , const std::string& aLine
                        , const uint64_t aLineNumber );
        void removeFirstOccurence( const ColorName aColor
                , std::list< ColorName >& aColors );

    protected: // fields
        std::list< IRule::Ptr > m_Rules;
}; // class RuleBox

} // namespace Color
