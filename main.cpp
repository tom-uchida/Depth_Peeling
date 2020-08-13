#include <kvs/Application>
#include "Input.h"
#include "Data.h"
#include "Screens.h"


int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );

    local::Input input( argc, argv );
    if ( !input.parse() ) { return 1; }

    local::Screens screens( app, input );
    screens.show();
    
    return app.run();
}