#include "Rules.hh"

namespace Color {

std::string colorizeString( const boost::smatch aMatch, ColorName aColor, ColorName aResetCol = RESET )
{
    std::stringstream lStream;
    if ( aMatch[0].str().size() )
    {
        color( aColor, aMatch[0].str(), lStream, aResetCol );
    }
    return lStream.str();
}

void IntermediateResult::putMarker( size_t aIndex, const ColorName aColor )
{
    std::cout << "AAA" << std::endl;
}

void IntermediateResult::getMarkers( size_t aIndex, std::vector< const Marker >& aRules ) const
{
}

Rule::Rule( ColorName aColor, const std::string& aRegex, const RuleIndex aIndex, bool aWholeLines ) 
    : IRule( aIndex )
      , m_WholeLines( aWholeLines )
      , m_Color( aColor )
      , m_Regex( aRegex )
    {
    }


void Rule::apply( const std::string& aLine
        , IntermediateResult& aResContainer
        , uint64_t aLineNumber ) const
{
    if ( m_WholeLines )
    {
        if ( boost::regex_search( aLine, m_Regex ) )
        {
            aResContainer.putMarker( 0, m_Color );
            aResContainer.putMarker( aLine.size(), m_Color );
        }
    }
    else
    {
        boost::smatch lSearchRes;
        size_t lGlobal( 0 );
        std::string::const_iterator lStart( aLine.begin() )
                                    , lEnd( aLine.end() );
        while ( boost::regex_search( lStart, lEnd, lSearchRes, m_Regex ) )
        {
            aResContainer.putMarker( lGlobal + lSearchRes.position(), m_Color );
            aResContainer.putMarker( lGlobal + lSearchRes.position() + lSearchRes.length(), m_Color );
            lStart += lSearchRes.position() + lSearchRes.length();
            lGlobal += lSearchRes.position() + lSearchRes.length();
        }
    }
}

NumberRule::NumberRule( const ColorName aInitialColor
        , const RuleIndex aRuleIndex 
        , const uint8_t aSimilarLinesCount ) 
    : IRule( aRuleIndex )
      , m_SimilarLinesCount( aSimilarLinesCount )
      , m_Colors()
{
    if ( !m_SimilarLinesCount )
        throw std::runtime_error( "Number of similar lines cannot be zero" );
    m_Colors.push_back(aInitialColor);
}

void NumberRule::addColor( const ColorName aColor )
{
    m_Colors.push_back( aColor );
}

void NumberRule::apply( const std::string& aLine
        , IntermediateResult& aResContainer
        , uint64_t aLineNumber ) const
{
//    const uint16_t lColorIndex( ( aLineNumber / mSimilarLinesCount ) % mColors.size() );
//    const ColorName lColor( mColors[ lColorIndex ] );
//    std::stringstream lStream;
//    color( lColor, aLine, lStream );
//    aResetCol = lColor; // it will be passed to the next rule
//    return lStream.str();
}


} // namespace Color
