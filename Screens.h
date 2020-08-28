#pragma once
#include <cmath>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <kvs/Application>
#include <kvs/Screen>
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
    kvs::Screen m_screen_dpr;
    local::Label m_label_dpr;
    kvs::ColorImage m_image_dpr;

public:
    Screens( kvs::Application& _app, local::Input& _input, const kvs::PolygonObject* _ply ):
        m_screen_dpr( &_app ),
        m_label_dpr( &m_screen_dpr, m_screen_dpr.scene() )
    {
        // Create some polygons to rendering
        // kvs::PolygonObject polygon = local::Data( _input );

        kvs::PolygonObject polygon = *_ply;

        // Set up
        m_screen_dpr.setTitle( "Depth Peeling Rendering" );
        m_screen_dpr.setPosition( 0, 0 );
        this->setup_dpr( _input, polygon );
    }

    void show()
    {
        m_screen_dpr.show();
        m_label_dpr.show();
    }

private:

    void setup_dpr( local::Input& _input, kvs::PolygonObject& _polygon )
    {
        typedef kvs::PolygonObject Object;
        typedef local::DepthPeelingRenderer Renderer;

        Object* object = new Object();
        object->shallowCopy( _polygon );

        Renderer* renderer = new Renderer();
        renderer->setName("DPR");
        renderer->setBackgroundColor( _input.background );
        renderer->setNumberOfPeels( _input.npeels );

        m_screen_dpr.setSize( _input.width, _input.height );
        m_screen_dpr.setBackgroundColor( _input.background );
        m_screen_dpr.registerObject( object, renderer );
        m_screen_dpr.create();

        m_label_dpr.setInput( _input );
        m_label_dpr.show();

        kvs::Light::SetModelTwoSide( true );
    }

    kvs::ColorImage diff_image(
        const kvs::ColorImage& _image1,
        const kvs::ColorImage& _image2,
        kvs::Real32& _mse,
        kvs::Real32& _psnr )
    {
        const float max_length = kvs::Vec3::Constant(1).length();
        kvs::ColorImage image( _image1.width(), _image1.height() );
        kvs::Real32 sum = 0.0f;
        for ( size_t index = 0; index < image.numberOfPixels(); index++ )
        {
            const kvs::Vec3 p1 = _image1.pixel( index ).toVec3();
            const kvs::Vec3 p2 = _image2.pixel( index ).toVec3();
            const kvs::Real32 length = ( p2 - p1 ).length();
            const kvs::Vec3 pixel = kvs::Vec3::Constant( 1 - length / max_length );
            image.setPixel( index, kvs::RGBColor( pixel ) );
            sum += length * length;
        }

        _mse = sum / ( 3.0f * image.numberOfPixels() );
        _psnr = 10.0f * std::log10( 1.0 / _mse );

        return image;
    }
};

} // end of namespace local
