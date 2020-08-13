#include <kvs/glut/Application>
#include "Input.h"
#include "Data.h"
#include "Screens.h"


int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    local::Input input( argc, argv );
    if ( !input.parse() ) { return false; }

    local::Screens screens( app, input );
    if ( input.offscreen )
    {
        screens.hide();
        screens.capture();
        screens.dump( input );
        app.quit();
        return true;
    }
    else
    {
        screens.show();
        return app.run();
    }
}
