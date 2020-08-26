#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "ImportPointClouds.h"
#include "plyRead.h"
#include "xyzAsciiReader.h"
// #include "xyzBinaryReader.h"
// #include "spbr.h"
// #include "spcomment.h"

const int BUF_MAX = 1024; 

ImportPointClouds::ImportPointClouds( void ): m_hasFace( false ) {
}

ImportPointClouds::ImportPointClouds( char *filename ): m_hasFace( false ) {
    classification( filename );
}

int ImportPointClouds::breakWord( char* buf, std::string *str ) {                                             
    char* data;                                 
    int n = 0;                                  
    data = strtok( buf, " \t" );                
    while( data != NULL ) {                     
        str[n] = data;                            
        data = strtok( NULL, " \t" );             
        n++;                                      
    }                                           

    return n;                                   
}

void ImportPointClouds::classification( char* filename ) {
    std::ifstream fin( filename ); 
    if( !fin ) {
        std::cout << "ERROR: Cannot Open File: " << filename << std::endl;
        exit(1);
    } 

    SuperClass::setPolygonType( kvs::PolygonObject::UnknownPolygonType );
    SuperClass::setColorType( kvs::PolygonObject::VertexColor );
    SuperClass::setNormalType( kvs::PolygonObject::VertexNormal );
    
    size_t numVert = 0;
    char buf[ BUF_MAX];
    std::string word[ 15 ]; 
    //--- Check File Type
    fin.getline( buf, BUF_MAX, '\n' );
    std::cout << "~~~~~ " << buf << std::endl;
    breakWord( buf, word ); 
    if( !strncmp( word[0].c_str(), "ply", 3 ) ) {
        std::cout << "PLY file reading....." << std::endl;
        kvs::PolygonObject* ply = new plyRead( filename, m_hasFace );

        SuperClass::setCoords( ply->coords() );
        SuperClass::setNormals( ply->normals() ); 
        SuperClass::setColors( ply->colors() );

        numVert = ply->numberOfVertices();
        if( m_hasFace ) {
            SuperClass::setConnections( ply->connections() );
            SuperClass::setPolygonType( ply->polygonType() );
        }
    
    } else {    
        m_hasFace = false;
        std::cout << "XYZRGB file or Other type file reading....." << std::endl;
        xyzAsciiReader* ply = new xyzAsciiReader( filename );
        m_ft = ply->featureData();
        SuperClass::setCoords( ply->coords() ); 
        SuperClass::setNormals( ply->normals() ); 
        SuperClass::setColors( ply->colors() );         
        numVert = ply->numberOfVertices();
    }

    //  std::cout << "Number of Popints : " << numVert << std::endl;
    fin.close();
}
