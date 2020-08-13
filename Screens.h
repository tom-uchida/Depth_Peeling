#pragma once
#include <cmath>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/File>
#include <kvs/Light>
#include <kvs/PolygonObject>
#include <kvs/StochasticPolygonRenderer>
#include "DepthPeelingRenderer.h"
#include "Input.h"
#include "Data.h"
#include "Label.h"


namespace local
{

class Screens
{
private:
    kvs::glut::Screen m_screen_pbr;
    kvs::glut::Screen m_screen_dpr;
    kvs::glut::Screen m_screen_tru;
    local::Label m_label_pbr;
    local::Label m_label_dpr;
    local::Label m_label_tru;
    kvs::ColorImage m_image_pbr;
    kvs::ColorImage m_image_dpr;
    kvs::ColorImage m_image_tru;

public:
    Screens( kvs::glut::Application& app, local::Input& input ):
        m_screen_pbr( &app ),
        m_screen_dpr( &app ),
        m_screen_tru( &app ),
        m_label_pbr( &m_screen_pbr, m_screen_pbr.scene() ),
        m_label_dpr( &m_screen_dpr, m_screen_dpr.scene() ),
        m_label_tru( &m_screen_tru, m_screen_tru.scene() )
    {
        kvs::PolygonObject polygon = local::Data( input );

        m_screen_pbr.setTitle( "Particle Based Rendering" );
        m_screen_pbr.setPosition( 0, 0 );
        this->setup_pbr( input, polygon );

        m_screen_dpr.setTitle( "Depth Peeling Rendering" );
        m_screen_dpr.setPosition( m_screen_pbr.x() + m_screen_pbr.width(), m_screen_pbr.y() );
        this->setup_dpr( input, polygon );

        m_screen_tru.setTitle( "Truth" );
        m_screen_tru.setPosition( m_screen_dpr.x() + m_screen_dpr.width(), m_screen_dpr.y() );
        this->setup_tru( input, polygon );
    }

    void show()
    {
        m_label_pbr.show();
        m_label_dpr.show();
        m_label_tru.show();
        m_screen_pbr.show();
        m_screen_dpr.show();
        m_screen_tru.show();
    }

private:
    void setup_pbr( local::Input& input, kvs::PolygonObject& polygon )
    {
        typedef kvs::PolygonObject Object;
        typedef kvs::StochasticPolygonRenderer Renderer;

        m_screen_pbr.setSize( input.width, input.height );
        m_screen_pbr.setBackgroundColor( input.background );

        Object* object = new Object();
        object->shallowCopy( polygon );

        Renderer* renderer = new Renderer();
        renderer->setName("PBR");
        renderer->setRepetitionLevel( input.nrepeats );

        m_screen_pbr.registerObject( object, renderer );
        m_screen_pbr.create();
        kvs::Light::SetModelTwoSide( true );

        m_label_pbr.setInput( input );
        m_label_pbr.show();
    }

    void setup_dpr( local::Input& input, kvs::PolygonObject& polygon )
    {
        typedef kvs::PolygonObject Object;
        typedef local::DepthPeelingRenderer Renderer;

        m_screen_dpr.setSize( input.width, input.height );
        m_screen_dpr.setBackgroundColor( input.background );

        Object* object = new Object();
        object->shallowCopy( polygon );

        Renderer* renderer = new Renderer();
        renderer->setName("DPR");
        renderer->setBackgroundColor( input.background );
        renderer->setNumberOfPeels( input.npeels );

        m_screen_dpr.registerObject( object, renderer );
        m_screen_dpr.create();
        kvs::Light::SetModelTwoSide( true );

        m_label_dpr.setInput( input );
        m_label_dpr.show();
    }

    void setup_tru( local::Input& input, kvs::PolygonObject& polygon )
    {
        typedef kvs::PolygonObject Object;
        typedef local::DepthPeelingRenderer Renderer;

        m_screen_dpr.setSize( input.width, input.height );
        m_screen_dpr.setBackgroundColor( input.background );

        Object* object = new Object();
        object->shallowCopy( polygon );

        Renderer* renderer = new Renderer();
        renderer->setName("TRU");
        renderer->setBackgroundColor( input.background );
        renderer->setNumberOfPeels( input.npolygons );

        m_screen_tru.registerObject( object, renderer );
        m_screen_tru.create();
        kvs::Light::SetModelTwoSide( true );

        m_label_tru.setInput( input );
        m_label_tru.show();
    }

    kvs::ColorImage diff_image(
        const kvs::ColorImage& image1,
        const kvs::ColorImage& image2,
        kvs::Real32& mse,
        kvs::Real32& psnr )
    {
        const float max_length = kvs::Vec3::Constant(1).length();
        kvs::ColorImage image( image1.width(), image1.height() );
        kvs::Real32 sum = 0.0f;
        for ( size_t index = 0; index < image.numberOfPixels(); index++ )
        {
            const kvs::Vec3 p1 = image1.pixel( index ).toVec3();
            const kvs::Vec3 p2 = image2.pixel( index ).toVec3();
            const kvs::Real32 length = ( p2 - p1 ).length();
            const kvs::Vec3 pixel = kvs::Vec3::Constant( 1 - length / max_length );
            image.setPixel( index, kvs::RGBColor( pixel ) );
            sum += length * length;
        }

        mse = sum / ( 3.0f * image.numberOfPixels() );
        psnr = 10.0f * std::log10( 1.0 / mse );

        return image;
    }
};

} // end of namespace local
