#include "Config.hh"
#include "utils.hh"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


namespace Colorlog {

std::string ColorList() {
    std::ostringstream oss;
    oss << "(";
    for (ColorStruct cs : supported_colors) {
        oss  << cs.name << "|";
    }
    oss.seekp(-1, std::ios_base::end);
    oss << ")";
    return oss.str();
}

const boost::regex Config::RULE_BOX_REG = boost::regex( "\\[[a-zA-Z0-9]*\\][ ]*[#]?.*" );
const std::string GENERIC_COLOR_NAME ="(\\[.*\\],)*\\[.*\\]";
const std::string COLOR_NAME ="(\\[" + ColorList() + "\\],)*\\[" + ColorList() + "\\]";
const boost::regex Config::NUMBER_RULE_REG[2] = { boost::regex( "alternate=[0-9]*:" + GENERIC_COLOR_NAME ), boost::regex( "alternate=[0-9]*:" + COLOR_NAME ) };
const boost::regex Config::RULE_REG[2] = { boost::regex( "color=" + GENERIC_COLOR_NAME + ":.*" ), boost::regex( "color=" + COLOR_NAME + ":.*" ) };
const boost::regex Config::RULE_WHOLE_REG[2] = { boost::regex( "color_full_line=" + GENERIC_COLOR_NAME + ":.*" ), boost::regex( "color_full_line=" + COLOR_NAME + ":.*" ) };
const boost::regex Config::REF_RULE_REG = boost::regex( "scheme=[a-zA-Z0-9]*" );
// helper wrappers for constructors of rules
Rule::Ptr ruleWrapper( ColorName aColor, const std::string& aRegex
        , bool aWholeLine )
{
    return Rule::Ptr( new Rule( aColor, aRegex, aWholeLine ) );
}

NumberRule::Ptr numberRuleWrapper( ColorName aColor
        , const uint8_t aLinesNum )
{
    return NumberRule::Ptr( new NumberRule( aColor, aLinesNum ) );
}

ReferenceRule::Ptr refRuleWrapper( const std::string& aScheme
                                 , Config& aConfig )
{
    return ReferenceRule::Ptr( new ReferenceRule( aScheme, aConfig ) );
}

RuleBox::Ptr ruleBoxWrapper()
{
    return RuleBox::Ptr( new RuleBox );
}

Config::Config( std::istream& aFile )
    : m_Rules()
    , m_CreateRule( &ruleWrapper )
    , m_CreateNumberRule( &numberRuleWrapper )
    , m_CreateRuleBox( &ruleBoxWrapper )
    , m_CreateRefRule( &refRuleWrapper )
{
    parseConfig( aFile );
}

Config::Config( std::istream& aFile
        , RuleCreator aRuleCreator
        , NumberRuleCreator aNumberRuleCreator
        , ReferenceRuleCreator aRefCreator
        , RuleBoxCreator aRuleBoxCreator )
    : m_Rules()
    , m_CreateRule( aRuleCreator )
    , m_CreateNumberRule( aNumberRuleCreator )
    , m_CreateRuleBox( aRuleBoxCreator )
    , m_CreateRefRule( aRefCreator )
{
    parseConfig( aFile );
}
const RuleBox::Ptr Config::getRuleBox( const std::string& aName ) const
{
    RuleMap::const_iterator lRulesIt( m_Rules.find( aName ) );
    if ( lRulesIt == m_Rules.end() )
    {
        throw std::runtime_error( "Expected rule '" + aName + "' cannot be found in config file");
    }
    return lRulesIt->second;
}

const Config::RuleMap& Config::getAllRules() const
{
    return m_Rules;
}

void Config::parseConfig( std::istream& aStr )
{
    std::string lLine;
    RuleBox::Ptr lCurrentRuleBox( NULL );
    size_t lLineNumber( 0 );
    while ( std::getline( aStr, lLine ) )
    {
        ++lLineNumber;
        if ( isRelevant( lLine ) )
        {
            if( couldBeRuleBox( lLine ) )
            {
                handleRuleBox( lCurrentRuleBox, lLine );
            }
            else if ( couldBeNumberRule( lLine, lCurrentRuleBox ) )
            {
                handleNumberRule( lCurrentRuleBox, lLine );
            }
            else if ( couldBeRule( lLine, lCurrentRuleBox ) )
            {
                static const bool doNotColoWholeLines( false );
                handleRule( lCurrentRuleBox, lLine, doNotColoWholeLines );
            }
            else if ( couldBeWholeLineRule( lLine, lCurrentRuleBox ) )
            {
                static const bool colorWholeLines( true );
                handleRule( lCurrentRuleBox, lLine, colorWholeLines );
            }
            else if ( couldBeRefRule( lLine, lCurrentRuleBox ) )
            {
                handleRefRule( lLine, lCurrentRuleBox );
            }
            else
            {
                handleError( lLine, lLineNumber );
            }
        }
    }
    if ( m_Rules.empty() )
    {
        throw std::runtime_error( "Could not create any rules from configuration.");
    }
}

ColorName Config::matchColor( const std::string& aColorStr )
{
    //truncate brackets
    std::string color = aColorStr.substr(1, aColorStr.size()-2);

    for (ColorStruct cs : supported_colors) {
        if (cs.name == color) return cs.ename;
    }
    throw std::runtime_error( "Color '" + color + "' is not supported. Please choose in the list below:\n" + ColorList());
}
std::string Config::getDataPart( const std::string& aLine )
{
    size_t lEqualizerLocation( aLine.find_first_of( "=" ) );
    std::string lValuePart( aLine.substr( lEqualizerLocation + 1
                , aLine.size() - lEqualizerLocation ) );
    return lValuePart;
}

void Config::preprocessLine( const std::string& aLine, Words& aValues )
{
    std::string lValuePart( getDataPart( aLine ) );
    size_t lSplitter( lValuePart.find_first_of( ":" ) );
    aValues.push_back( lValuePart.substr( 0, lSplitter ) );
    aValues.push_back( lValuePart.substr(
                lSplitter + 1, lValuePart.size() - lSplitter ) );
}

void Config::handleRuleBox( RuleBox::Ptr& aCurrentRuleBox
        , const std::string& aLine )
{
    aCurrentRuleBox = m_CreateRuleBox();
    size_t lEndOfName( aLine.find_first_of( "]" ) );
    m_Rules.insert( RuleMapElem( aLine.substr(
                            OMIT_FIRST_BRACKET
                            , lEndOfName - OMIT_FIRST_BRACKET )
                , aCurrentRuleBox ) );
    if ( !m_FirstBox )
        m_FirstBox = aCurrentRuleBox;

}
void Config::handleNumberRule( RuleBox::Ptr& aCurrentRuleBox
        , const std::string& aLine )
{
    Words lValues; // unlucky name
    preprocessLine( aLine, lValues );
    uint8_t lColorLineNumber(
                boost::lexical_cast< int >( lValues[ 0 ] ) );
    Words lColorNames;
    boost::split( lColorNames, lValues[ 1 ], boost::is_any_of( "," ) );
    if ( lColorNames.size() < MIN_SIZE_NUM_RULE )
        throw std::runtime_error( "Not enough parameters in line " + aLine );

    Colors lColors( lColorNames.size() );

    std::transform( begin( lColorNames ), end( lColorNames )
            , begin( lColors )
            , std::bind( &Config::matchColor
                    , this
                    , std::placeholders::_1 ) );

    NumberRule::Ptr lRule( m_CreateNumberRule( *begin( lColors )
                , lColorLineNumber ) );

    std::for_each( ++begin( lColors ) // start from second element
                , end( lColors )
                , std::bind( &NumberRule::addColor
                        , std::ref( *lRule )
                        , std::placeholders::_1 ) );
    aCurrentRuleBox->addRule( lRule );
}
void Config::handleRule( RuleBox::Ptr& aCurrentRuleBox
        , const std::string& aLine
        , const bool aWholeL )
{
    Words lValues; // unlucky name
    preprocessLine( aLine, lValues );
    Rule::Ptr lRule( m_CreateRule( matchColor( lValues[ 0 ] )
                , lValues[ 1 ]
                , aWholeL ) );
    aCurrentRuleBox->addRule( lRule );
}

void Config::handleRefRule( const std::string& aLine
                        , RuleBox::Ptr& aCurrentRuleBox )
{
    std::string lSchemeName( getDataPart( aLine ) );
    IRule::Ptr lRule( m_CreateRefRule( lSchemeName , *this ) );
    aCurrentRuleBox->addRule( lRule );
}

void Config::handleError( const std::string& aLine
        , const size_t aLineNumber )
{
    std::stringstream lStr;
    lStr << "Did not recognize line " << aLineNumber
        << " of configuration file " << std::endl
        << "\"" << aLine << "\"" << std::endl;
    throw std::runtime_error( lStr.str() );
}

}
