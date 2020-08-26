#include <kvs/Application>
#include "Input.h"
#include "Data.h"
#include "Screens.h"
#include "ImportPointClouds.h"


int main( int argc, char** argv )
{
    local::Input input( argc, argv );
    if ( !input.parse() ) { return 1; }

    ImportPointClouds *ply = new ImportPointClouds( argv[1] );
    ply->updateMinMaxCoords();
    std::cout << "\n";
    std::cout << "PLY Min, Max Coords:\n";
    std::cout << "Min: " << ply->minObjectCoord() << "\n";
    std::cout << "Max: " << ply->maxObjectCoord() << "\n";
    std::cout << "Number of points: " << ply->numberOfVertices() << "\n";

    kvs::Application app( argc, argv );
    local::Screens screens( app, input, ply );
    screens.show();
    
    return app.run();
}