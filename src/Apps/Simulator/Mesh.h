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
        void setMeshType( TYPE t );//{ mesh_type = t; }



        void setVertices( const std::vector< float >& v );
        void setVertice( int id, float x, float y, float z );

        void getVertice( int id, float& x, float& y, float& z );
        inline std::vector< float > getVertices(){ return vertices; }


        void setWireframe( const std::vector< unsigned int >& w );
        void setEdge( int id, const std::vector< unsigned int >& e );


        void setFaces( const std::vector< unsigned int >& f );
        void setFace( int id, const std::vector< unsigned int >& f );
        inline std::vector< unsigned int > getFaces(){ return faces; }
        std::vector< unsigned int > getFace( int id );


        void setConstantColor( const float r, const float g, const float b );
        void setColor( const std::vector< float >& vcolors  );


		void setCrossSectionClippingEquation( const float& a, const float& b, const float& c, const float& d , Eigen::Vector3f _centre);
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
		void draw(const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const float& scale, const float width, const float height);
        void initializeShader( std::string directory );

        void reloadShader();

        void clear();

        void resetBuffers();
        void deleteShaders();
		/// get the correct depth from the model in QGraphicsScene coordinates
		float getDepth() const;


        void getTrianglesfromQuadrilaterals( unsigned int id, std::vector< unsigned int >& triangles );

        void getTrianglesfromTetrahedros( unsigned int id, std::vector< unsigned int >& triangles );


        void getTrianglesfromHexahedros( unsigned int id, std::vector< unsigned int >& triangles );


        void getCoordfromIndex( std::vector< unsigned int > triangles, std::vector< float >& vertices_coord  );


        void getColorsfromIndex( std::vector< unsigned int > triangles, std::vector< float >& vertices_coord  );


        void setColorinFace( std::vector< unsigned int > triangles, float r, float g, float b, std::vector< float >& colors_coord  );

        void loadColorsbyVertices();


        void loadColorsbyFaces();


        void loadVertices();


    protected:

        TYPE mesh_type;

        std::vector< float > vertices;
        std::vector< unsigned int > faces;
        std::vector< unsigned int > wireframe;
        std::vector < float > normals_by_vertices;
        std::vector< float > bounding_box;
        std::vector< float > colors;

        float max[ 3 ];
        float min[ 3 ];

        Tucano::Shader* shader_mesh;
        Tucano::Shader* shader_mesh_cornerpoint;
        Tucano::Shader* shader;

        GLuint va_mesh;
        GLuint bf_vertices_mesh;
        GLuint bf_faces_mesh;
        GLuint bf_wireframe_mesh;
        GLuint bf_colors_mesh;

        GLuint number_of_vertices;
        GLuint vector_triangles_size;
        GLuint vector_wireframe_size;

        bool show_vertices;
        bool show_edges;
        bool show_faces;

        bool apply_crosssection_clipping;

        float coefACrossSectionEquation;
        float coefBCrossSectionEquation;
        float coefCCrossSectionEquation;
        float coefDCrossSectionEquation;
		Eigen::Vector3f centre_;

        Tucano::Shader* shader_bbox;

        GLuint va_bbox;
        GLuint bf_bbox_mesh;
        GLuint number_lines_bbox;

        bool show_bbox;

		/// get the correctly depth value
		float depth_;

};

#endif // MESH_H
