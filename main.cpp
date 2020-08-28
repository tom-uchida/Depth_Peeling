#include <kvs/Application>
#include "Input.h"
#include "Data.h"
#include "Screens.h"

#include "ImportPointClouds.h"
#include <kvs/PolygonImporter>


int main( int argc, char** argv )
{
    local::Input input( argc, argv );
    if ( !input.parse() ) { return 1; }

    // ImportPointClouds *ply = new ImportPointClouds( argv[1] );
    // ply->updateMinMaxCoords();
    // ply->setColor( kvs::RGBColor::White() );
    // ply->setPolygonTypeToTriangle();
    // ply->setNormalTypeToPolygon();

    const std::string filename( argv[1] );
    kvs::PolygonObject* ply = new kvs::PolygonImporter( filename );
    ply->setColor( kvs::RGBColor::White() );

    kvs::Application app( argc, argv );
    local::Screens screens( app, input, ply );
    screens.show();
    
    return app.run();
}