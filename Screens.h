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
    kvs::Real32 m_time_pbr;
    kvs::Real32 m_time_dpr;
    kvs::Real32 m_time_tru;

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

    void hide()
    {
        m_label_pbr.hide();
        m_label_dpr.hide();
        m_label_tru.hide();
        m_screen_pbr.hide();
        m_screen_dpr.hide();
        m_screen_tru.hide();
    }

    void capture()
    {
        m_screen_pbr.paintEvent();
        m_screen_pbr.paintEvent();
        m_image_pbr = m_screen_pbr.scene()->camera()->snapshot();

        m_screen_dpr.paintEvent();
        m_screen_dpr.paintEvent();
        m_image_dpr = m_screen_dpr.scene()->camera()->snapshot();

        m_screen_tru.paintEvent();
        m_screen_tru.paintEvent();
        m_image_tru = m_screen_tru.scene()->camera()->snapshot();

        m_time_pbr = m_screen_pbr.scene()->renderer("PBR")->timer().msec();
        m_time_dpr = m_screen_dpr.scene()->renderer("DPR")->timer().msec();
        m_time_tru = m_screen_tru.scene()->renderer("TRU")->timer().msec();
    }

    void dump( local::Input& input )
    {
        kvs::Real32 pbrd_mse = 0.0f;
        kvs::Real32 pbrd_psnr = 0.0f;
        kvs::ColorImage pbrd_img = this->diff_image( m_image_tru, m_image_pbr, pbrd_mse, pbrd_psnr );

        kvs::Real32 dprd_mse = 0.0f;
        kvs::Real32 dprd_psnr = 0.0f;
        kvs::ColorImage dprd_img = this->diff_image( m_image_tru, m_image_dpr, dprd_mse, dprd_psnr );

        std::stringstream basename;
        basename << std::setw(3) << std::setfill('0') << input.width;
        basename << "x";
        basename << std::setw(3) << std::setfill('0') << input.height;
        basename << "_";
        basename << std::setw(5) << std::setfill('0') << input.npolygons;
        basename << "_";
        basename << std::setw(3) << std::setfill('0') << int( input.opacity * 100 );

        std::stringstream basename_pbr;
        basename_pbr <<  std::setw(4) << std::setfill('0') << input.nrepeats;

        std::stringstream basename_dpr;
        basename_dpr <<  std::setw(4) << std::setfill('0') << input.npeels;

        std::stringstream basename_tru;
        basename_tru <<  std::setw(4) << std::setfill('0') << input.npolygons;

        std::string filename_pbr = "pbr_" + basename.str() + "_" + basename_pbr.str() + ".bmp";
        std::string filename_dpr = "dpr_" + basename.str() + "_" + basename_dpr.str() + ".bmp";
        std::string filename_tru = "tru_" + basename.str() + "_" + basename_tru.str() + ".bmp";
        std::string filename_pbrd = "pbrd_" + basename.str() + "_" + basename_pbr.str() + ".bmp";
        std::string filename_dprd = "dprd_" + basename.str() + "_" + basename_dpr.str() + ".bmp";
        m_image_pbr.write( filename_pbr );
        m_image_dpr.write( filename_dpr );
        m_image_tru.write( filename_tru );
        pbrd_img.write( filename_pbrd );
        dprd_img.write( filename_dprd );

        std::stringstream log_label;
        log_label << "width,";
        log_label << "height,";
        log_label << "npolygons,";
        log_label << "opacity,";
        log_label << "nrepeats,";
        log_label << "npeels,";
        log_label << "time [msec] (PBR),";
        log_label << "time [msec] (DPR),";
        log_label << "time [msec] (TRU),";
        log_label << "MSE (PBR),";
        log_label << "MSE (DPR),";
        log_label << "PSNR (PBR),";
        log_label << "PSNR (DPR),";
        log_label << "PBR image file,";
        log_label << "DPR image file,";
        log_label << "TRU image file,";
        log_label << "PBR diff image file,";
        log_label << "DPR diff image file";

        std::stringstream log_data;
        log_data << input.width << ",";
        log_data << input.height << ",";
        log_data << input.npolygons << ",";
        log_data << input.opacity << ",";
        log_data << input.nrepeats << ",";
        log_data << input.npeels << ",";
        log_data << m_time_pbr << ",";
        log_data << m_time_dpr << ",";
        log_data << m_time_tru << ",";
        log_data << pbrd_mse << ",";
        log_data << dprd_mse << ",";
        log_data << pbrd_psnr << ",";
        log_data << dprd_psnr << ",";
        log_data << filename_pbr << ",";
        log_data << filename_dpr << ",";
        log_data << filename_tru << ",";
        log_data << filename_pbrd << ",";
        log_data << filename_dprd;

        kvs::File file( "log.csv" );
        if ( file.exists() )
        {
            std::ofstream ofs( "log.csv", std::ios::app );
            ofs << log_data.str() << std::endl;
        }
        else
        {
            std::ofstream ofs( "log.csv", std::ios::out );
            ofs << log_label.str() << std::endl;
            ofs << log_data.str() << std::endl;
        }
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
        const float max_length = kvs::Vec3::All(1).length();
        kvs::ColorImage image( image1.width(), image1.height() );
        kvs::Real32 sum = 0.0f;
        for ( size_t index = 0; index < image.numberOfPixels(); index++ )
        {
            const kvs::Vec3 p1 = image1.pixel( index ).toVec3();
            const kvs::Vec3 p2 = image2.pixel( index ).toVec3();
            const kvs::Real32 length = ( p2 - p1 ).length();
            const kvs::Vec3 pixel = kvs::Vec3::All( 1 - length / max_length );
            image.setPixel( index, kvs::RGBColor( pixel ) );
            sum += length * length;
        }

        mse = sum / ( 3.0f * image.numberOfPixels() );
        psnr = 10.0f * std::log10( 1.0 / mse );

        return image;
    }
};

} // end of namespace local
