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
    // local::Label m_label; // m_label( &_screen, _screen.scene() )
    kvs::ColorImage m_image_dpr;

public:
    Screens( kvs::Screen& _screen, local::Input& _input, const kvs::PolygonObject* _ply )
    {
        // Create some polygons to rendering
        // kvs::PolygonObject object = local::Data( _input );

        kvs::PolygonObject object = *_ply;

        // Set up screen   
        this->setup_screen( _screen, _input, object );

        // Draw label
        // m_label.setInput( _input );
        // m_label.show();

        kvs::Light::SetModelTwoSide( true );
    }

private:

    void setup_screen( kvs::Screen& _screen, local::Input& _input, kvs::PolygonObject& _object )
    {
        _screen.setTitle( "Depth Peeling Rendering" );
        _screen.setPosition( 0, 0 );
        _screen.setSize( _input.width, _input.height );
        _screen.setBackgroundColor( _input.background ); 
        _screen.create();

        kvs::PolygonObject* object = new kvs::PolygonObject();
        object->shallowCopy( _object );

        local::DepthPeelingRenderer* renderer = new local::DepthPeelingRenderer();
        renderer->setName("DPR");
        // renderer->setBackgroundColor( _input.background );
        renderer->setNumberOfPeels( _input.npeels );

        _screen.registerObject( object, renderer );
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
