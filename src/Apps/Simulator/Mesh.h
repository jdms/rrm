#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iterator>
#include <string>

#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"

class Mesh
{



    public:


        enum class TYPE{ TRIANGLES, QUADRILATERAL, TETRAHEDRAL, HEXAHEDRAL };


        Mesh();
        inline Mesh( TYPE t ){ mesh_type = t; }


        inline void setMeshType( TYPE t ){ mesh_type = t; }



        inline void setVertices( const std::vector< float >& v ){ vertices.clear();
                                                      std::copy( v.begin(), v.end(), std::back_inserter( vertices ) );  }

        inline void setVertice( int id, float x, float y, float z ){ if ( isValidVertice( id ) == false ) return;
                                                                     vertices[ 3*id ] = x ;
                                                                     vertices[ 3*id + 1 ] = y ;
                                                                     vertices[ 3*id + 2 ] = z ; }

        void getVertice( int id, float& x, float& y, float& z );

        inline std::vector< float > getVertices(){ return vertices; }




        inline void setWireframe( const std::vector< unsigned int >& w ){ wireframe.clear();
                                                              std::copy( w.begin(), w.end(), std::back_inserter( wireframe ) ); }

        void setEdge( int id, const std::vector< unsigned int >& e );


        inline void setFaces( const std::vector< unsigned int >& f ){ faces.clear();
                                                          std::copy( f.begin(), f.end(), std::back_inserter( faces ) );  }

        void setFace( int id, const std::vector< unsigned int >& f );

        inline std::vector< unsigned int > getFaces(){ return faces; }

        std::vector< unsigned int > getFace( int id );


        void setConstantColor( const float r, const float g, const float b );
        void setColor( const std::vector< float >& colors  );


        void setCrossSectionClippingEquation( const float& a, const float& b, const float& c, const float& d );
        void disableCrossSection();


        bool isValidVertice( const int id );
        bool isValidEdge( const int id );
        bool isValidFace( const int id );



        int getNumberofVerticesbyFace();

        inline int getNumberofVertices(){ return ( int ) ( vertices.size()/3 ); }

        inline int getNumberofEdges(){ return ( int ) ( wireframe.size()/2 ); }

        inline int getNumberofFaces(){
                                       int number_of_vertices = getNumberofVerticesbyFace();
                                       return (int) ( faces.size()/ number_of_vertices ); }


        void showVertices( bool status );
        void showEdges( bool status );
        void showFaces( bool status );
        void showBoundingBox( bool status );


        bool showVertices() const;
        bool showEdges() const;
        bool showFaces() const;
        bool showBoundingBox() const;


        void buildWireframeList();
        void getWireframeofTriangles();
        void getWireframeofQuadrilaterals();


        void buildBoundingBox();
        void getBoundingBox( float& xm, float& xM, float& ym, float& yM, float& zm, float& zM );

        void buildTrianglesFacesList( vector< unsigned int >& triangles );
        void getTrianglesfromRectangles( vector< unsigned int >& triangles  );
        void getTrianglesfromTetrahedros( vector< unsigned int >& triangles  );




        void load();
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P );
        void initializeShader( std::string directory );

        void clear();


    protected:

        TYPE mesh_type;

        std::vector< float > vertices;
        std::vector< unsigned int > faces;
        std::vector< unsigned int > wireframe;
        std::vector < float > normals_by_vertices;
        std::vector< float > bounding_box;

        float max[ 3 ];
        float min[ 3 ];

        Tucano::Shader* shader_mesh;

        GLuint va_mesh;
        GLuint bf_vertices_mesh;
        GLuint bf_faces_mesh;
        GLuint bf_wireframe_mesh;
        GLuint bf_colors_mesh;

        GLuint number_of_vertices;
        GLuint vector_faces_size;
        GLuint vector_wireframe_size;

        bool show_vertices;
        bool show_edges;
        bool show_faces;

        bool apply_crosssection_clipping;

        float coefACrossSectionEquation;
        float coefBCrossSectionEquation;
        float coefCCrossSectionEquation;
        float coefDCrossSectionEquation;

        Tucano::Shader* shader_bbox;

        GLuint va_bbox;
        GLuint bf_bbox_mesh;
        GLuint number_lines_bbox;

        bool show_bbox;

};

#endif // MESH_H
