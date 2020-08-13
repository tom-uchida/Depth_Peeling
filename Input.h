#pragma once
#include <kvs/CommandLine>
#include <kvs/RGBColor>
#include <ctime>


namespace local
{

struct Input
{
private:
    kvs::CommandLine m_commandline;

public:
    size_t width;
    size_t height;
    size_t nrepeats;
    size_t npeels;
    size_t npolygons;
    float opacity;
    kvs::RGBColor color;
    kvs::RGBColor background;
    bool offscreen;
    size_t seed;

public:
    Input( int argc, char** argv ):
        width( 512 ),
        height( 512 ),
        nrepeats( 1 ),
        npeels( 1 ),
        npolygons( 10 ),
        opacity( 0.5 ),
        color( kvs::RGBColor::Black() ),
        background( kvs::RGBColor::White() ),
        offscreen( false ),
        seed( time(NULL) )
    {
        m_commandline = kvs::CommandLine( argc, argv );
        m_commandline.addHelpOption();
        m_commandline.addOption( "width", "Screen width. (defulat: 512)", 1, false );
        m_commandline.addOption( "height", "Screen height. (defulat: 512)", 1, false );
        m_commandline.addOption( "nrepeats", "Number of repetitions for IPBR. (defulat: 1)", 1, false );
        m_commandline.addOption( "npeels", "Number of peels for DP. (defulat: 1)", 1, false );
        m_commandline.addOption( "npolygons", "Number of polygons. (defulat: 10)", 1, false );
        m_commandline.addOption( "opacity", "Opacity value for polygon object. (default: 0.5)", 1, false );
        m_commandline.addOption( "color", "Color value for polygon object. (default: 0, 0, 0)", 3, false );
        m_commandline.addOption( "background", "Background color. (default: 255, 255, 255)", 3, false );
        m_commandline.addOption( "offscreen", "Offscreen rendering [0:disable, 1:enable]. (default: 0)", 1, false );
        m_commandline.addOption( "seed", "Seed point for generating polygon object. (default: time)", 1, false );
    }

    bool parse()
    {
        if ( !m_commandline.parse() ) { return false; }

        if ( m_commandline.hasOption( "width" ) )
        {
            width = m_commandline.optionValue<size_t>( "width" );
        }

        if ( m_commandline.hasOption( "height" ) )
        {
            height = m_commandline.optionValue<size_t>( "height" );
        }

        if ( m_commandline.hasOption( "nrepeats" ) )
        {
            nrepeats = m_commandline.optionValue<size_t>( "nrepeats" );
        }

        if ( m_commandline.hasOption( "npeels" ) )
        {
            npeels = m_commandline.optionValue<size_t>( "npeels" );
        }

        if ( m_commandline.hasOption( "npolygons" ) )
        {
            npolygons = m_commandline.optionValue<size_t>( "npolygons" );
        }

        if ( m_commandline.hasOption( "opacity" ) )
        {
            opacity = m_commandline.optionValue<float>( "opacity" );
        }

        if ( m_commandline.hasOption( "color" ) )
        {
            const int r = m_commandline.optionValue<int>( "color", 0 );
            const int g = m_commandline.optionValue<int>( "color", 1 );
            const int b = m_commandline.optionValue<int>( "color", 2 );
            color = kvs::RGBColor( r, g, b );
        }

        if ( m_commandline.hasOption( "background" ) )
        {
            const int r = m_commandline.optionValue<int>( "background", 0 );
            const int g = m_commandline.optionValue<int>( "background", 1 );
            const int b = m_commandline.optionValue<int>( "background", 2 );
            background = kvs::RGBColor( r, g, b );
        }

        if ( m_commandline.hasOption( "offscreen" ) )
        {
            offscreen = ( m_commandline.optionValue<int>( "offscreen" ) != 0 );
        }

        if ( m_commandline.hasOption( "seed" ) )
        {
            seed = m_commandline.optionValue<size_t>( "seed" );
        }

        return true;
    }
};

} // end of namespace local
