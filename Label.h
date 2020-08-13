#pragma once
#include <string>
#include <sstream>
#include <kvs/Label>
#include "Input.h"


namespace local
{

class Label : public kvs::Label
{
private:
    local::Input* m_input;
    kvs::Scene* m_scene;

public:
    Label( kvs::ScreenBase* screen, kvs::Scene* scene ):
        kvs::Label( screen ),
        m_scene( scene )
    {
        kvs::Font font;
        font.setFamilyToSans();
        font.setStyleToBold();
        font.setEnabledShadow( true );
        font.setShadowDistance( 2.0f );
        font.setShadowBlur( 1.5f );
        font.setSize( 20 );
        font.setColor( kvs::RGBColor::White() );
        this->setFont( font );
        this->setMargin( 10 );
    }

    void setInput( local::Input& input ) { m_input = &input; }

    void screenUpdated()
    {
        std::string renderer_name = m_scene->renderer()->name();
        if ( renderer_name == "PBR" )
        {
            this->setText( "Rendering method: Particle-based Rendering" );
        }
        else if ( renderer_name == "DPR" || renderer_name == "TRU" )
        {
            this->setText( "Rendering method: Depth Peeling" );
        }

        std::stringstream resolution;
        resolution << screen()->width() << " x " << screen()->height();
        this->addText( ( "Image resolution: " + resolution.str() ).c_str() );

        std::stringstream npolygons;
        npolygons << m_input->npolygons;
        this->addText( ( "Number of polygons: " + npolygons.str() ).c_str() );

        std::stringstream opacity;
        opacity << m_input->opacity;
        this->addText( ( "Polygon opacity: " + opacity.str() ).c_str() );

        if ( renderer_name == "PBR" )
        {
            std::stringstream nrepeats;
            nrepeats << m_input->nrepeats;
            this->addText( ( "Number of repeats: " + nrepeats.str() ).c_str() );
        }
        else if ( renderer_name == "DPR" )
        {
            std::stringstream npeels;
            npeels << m_input->npeels;
            this->addText( ( "Number of peels: " + npeels.str() ).c_str() );
        }
        else if ( renderer_name == "TRU" )
        {
            std::stringstream npeels;
            npeels << m_input->npolygons;
            this->addText( ( "Number of peels: " + npeels.str() ).c_str() );
        }

        std::stringstream rendering_time;
        rendering_time << m_scene->renderer()->timer().msec();
        this->addText( ( "Rendering time [msec]: " + rendering_time.str() ).c_str() );
    }
};

} // end of namespace local
