#pragma once
#include <kvs/PolygonObject>
#include <kvs/RGBColor>
#include <kvs/ValueArray>
#include <kvs/Vector3>
#include <kvs/MersenneTwister>
#include "Input.h"


namespace local
{

inline kvs::PolygonObject Data( const local::Input& input )
{
    const size_t npolygons = input.npolygons;
    const kvs::RGBColor color = input.color;
    const kvs::UInt8 opacity = int( input.opacity * 255 );

    const size_t nvertices = npolygons * 3;
    kvs::ValueArray<kvs::Real32> coords( nvertices * 3 );
    kvs::ValueArray<kvs::Real32> normals( npolygons * 3 );
    kvs::Real32* pcoords = coords.data();
    kvs::Real32* pnormals = normals.data();

    kvs::MersenneTwister rand( input.seed );
    for ( size_t i = 0; i < npolygons; i++ )
    {
        kvs::Vec3 p0( rand(), rand(), rand() );
        kvs::Vec3 p1( rand(), rand(), rand() );
        kvs::Vec3 p2( rand(), rand(), rand() );
        kvs::Vec3 n = ( p1 - p0 ).cross( p2 - p0 );

        *(pcoords++) = p0.x();
        *(pcoords++) = p0.y();
        *(pcoords++) = p0.z();
        *(pcoords++) = p1.x();
        *(pcoords++) = p1.y();
        *(pcoords++) = p1.z();
        *(pcoords++) = p2.x();
        *(pcoords++) = p2.y();
        *(pcoords++) = p2.z();
        *(pnormals++) = n.x();
        *(pnormals++) = n.y();
        *(pnormals++) = n.z();
    }

    kvs::PolygonObject object;
    object.setPolygonTypeToTriangle();
    object.setNormalTypeToPolygon();
    object.setCoords( coords );
    object.setNormals( normals );
    object.setColor( color );
    object.setOpacity( opacity );
    return object;
}

} // end of namespace local
