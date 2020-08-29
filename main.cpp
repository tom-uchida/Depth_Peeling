#include <kvs/Application>
#include "Input.h"
#include "Data.h"
#include "Screens.h"

#include "ImportPointClouds.h"
#include <kvs/PolygonImporter>
#include "event_control.h"

size_t nPeels;

int main( int argc, char** argv )
{
    local::Input input( argc, argv );
    if ( !input.parse() ) { return 1; }
    nPeels = input.npeels;

    ImportPointClouds *ply = new ImportPointClouds( argv[1] );
    ply->updateMinMaxCoords();
    ply->setColor( kvs::RGBColor::White() );
    // ply->setPolygonTypeToTriangle();
    // ply->setNormalTypeToPolygon();

    // const std::string filename( argv[1] );
    // kvs::PolygonObject* ply = new kvs::PolygonImporter( filename );
    // ply->setColor( kvs::RGBColor::White() );

    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    local::Screens screens( screen, input, ply );

    KeyPressEvent key_press_event;
    screen.addEvent( &key_press_event );
    screen.show();
    
    return app.run();
}