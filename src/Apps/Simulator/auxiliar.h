#ifndef AUXILIAR
#define AUXILIAR

#include <vector>
#include <string>

#include "Ply/ply.h"

using namespace std;

/*--------------------------------------------------------------------------*/
/** PLY Data*/
/*--------------------------------------------------------------------------*/
typedef struct PlyPoint
{
    float x, y, z;
    float nx, ny, nz;
    float r, g, b;
}PlyPoint;
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* list of property information for a PlyVertex */
static PlyProperty plyvert_props[]  = {
    {"x",  Float32, Float32, offsetof( PlyPoint, x  ), 0, 0, 0, 0},
    {"y",  Float32, Float32, offsetof( PlyPoint, y  ), 0, 0, 0, 0},
    {"z",  Float32, Float32, offsetof( PlyPoint, z  ), 0, 0, 0, 0},
    {"r", Float32, Float32, offsetof( PlyPoint, r ), 0, 0, 0, 0},
    {"g", Float32, Float32, offsetof( PlyPoint, g ), 0, 0, 0, 0},
    {"b", Float32, Float32, offsetof( PlyPoint, b ), 0, 0, 0, 0},
    {"nx", Float32, Float32, offsetof( PlyPoint, nx ), 0, 0, 0, 0},
    {"ny", Float32, Float32, offsetof( PlyPoint, ny ), 0, 0, 0, 0},
    {"nz", Float32, Float32, offsetof( PlyPoint, nz ), 0, 0, 0, 0},
};
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
typedef struct PlyFace
{
  unsigned char nverts;    /* number of Vertex indices in list */
  int *verts;              /* Vertex index list */
} PlyFace;
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* list of property information for a PlyFace */
static PlyProperty plyface_props[]  = {
{  "vertex_indices", Int32, Int32, offsetof( PlyFace,verts ),
   1, Uint8, Uint8, offsetof( PlyFace,nverts )  }
};
/*--------------------------------------------------------------------------*/


inline void readPlyFile( std::string namefile, vector< float >& vertices, vector< float >& color, vector< unsigned int >& triangles, vector< float >& normal )
{
    PlyFile *file_ply;
    FILE *file = fopen( namefile.c_str(), "r" );
    file_ply = ::read_ply( file );

    int n_elements = file_ply->num_elem_types;
    int nverts = 0;
    int ntrigs = 0;
    char* elem_name;

    PlyFace face;
    PlyPoint v;

    if( vertices.empty() == false ) vertices.clear();
    if( color.empty() == false ) color.clear();
    if( triangles.empty() == false ) triangles.clear();
    if( normal.empty() == false ) normal.clear();

    for( int i = 0; i < n_elements; ++i )
    {

        int elem_count = 0;
        elem_name = setup_element_read_ply ( file_ply, i, &elem_count );

        if ( equal_strings ( "vertex", elem_name ) )
          nverts = elem_count;
        if ( equal_strings ( "face",   elem_name ) )
          ntrigs = elem_count;
    }

    vertices.reserve( 3*nverts );
    color.reserve( 3*ntrigs );
    triangles.reserve( 3*ntrigs );
    normal.reserve( 3*nverts );

    for( int i = 0; i < n_elements; ++i )
    {

        int elem_count = 0;
        elem_name = setup_element_read_ply ( file_ply, i, &elem_count );

        if ( equal_strings ( "vertex", elem_name ) )
        {


            setup_property_ply ( file_ply, &plyvert_props[ 0 ] );
            setup_property_ply ( file_ply, &plyvert_props[ 1 ] );
            setup_property_ply ( file_ply, &plyvert_props[ 2 ] );
            setup_property_ply ( file_ply, &plyvert_props[ 3 ] );
            setup_property_ply ( file_ply, &plyvert_props[ 4 ] );
            setup_property_ply ( file_ply, &plyvert_props[ 5 ] );
            setup_property_ply ( file_ply, &plyvert_props[ 6 ] );
            setup_property_ply ( file_ply, &plyvert_props[ 7 ] );
            setup_property_ply ( file_ply, &plyvert_props[ 8 ] );

            for ( int j = 0; j < nverts; ++j )
            {

                get_element_ply ( file_ply, ( void * ) &(v) );

                vertices.push_back( v.x );
                vertices.push_back( v.y );
                vertices.push_back( v.z );

                color.push_back( v.r );
                color.push_back( v.g );
                color.push_back( v.b );


            }

        }

        else if ( equal_strings ( "face", elem_name ) )
        {

            setup_property_ply ( file_ply, &plyface_props[ 0 ] );
            for ( int j = 0; j < ntrigs; ++j )
            {
                get_element_ply ( file_ply, ( void * ) &face );
                if( face.nverts != 3 )
                {
                   printf("PLY importation: not a triangulated surface\n");
                }

                triangles.push_back( ( unsigned int ) face.verts[ 0 ] );
                triangles.push_back( ( unsigned int ) face.verts[ 1 ] );
                triangles.push_back( ( unsigned int ) face.verts[ 2 ] );

            }

        }

        else  /* all non-PlyVertex and non-PlyFace elements are grabbed here */
          get_other_element_ply ( file_ply );

    }


    if( ntrigs ){
        float *list_normals;// = new float[ 3*nverts ];
        list_normals = compute_normals( vertices.data(), nverts, triangles.data(), ntrigs, 0 );
        normal.insert( normal.end(), list_normals, list_normals+( 3*nverts ) );
    }

    close_ply( file_ply );
    free_ply ( file_ply );



}

#endif // AUXILIAR

