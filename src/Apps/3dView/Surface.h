#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <QString>

#include <iostream>
#include <vector>


#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"


#include "Model/StratigraphyRenderable.hpp"


class Surface: public StratigraphyRenderable
{

	public:
	

    int id ;

        Surface();
        Surface( int id_ );
		
		void resetBuffers();
		void initBuffers();
		void loadBuffers();
        void loadBuffers( const std::vector< float >& vertices );
		
		void resetShaders();
		void initShaders();
		void reloadShaders();

        void update( const Eigen::Vector3f& c );
		
        void initData();
        void init();


				
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height );
		
		
        inline void setCurrentDirectory( const std::string& directory ){ shader_directory = QString( directory.c_str() ); }
        inline bool initialized(){ return is_initialized; }
        inline bool visible(){ return is_visible; }


        inline void setCurrentColor( float r, float g, float b )
        {
            colors.push_back( r );
            colors.push_back( g );
            colors.push_back( b );
        }
        inline void setCurrentColor( const std::vector< float >& c ){  colors = c; }
		
		
    protected:

		
		Tucano::Shader* shader_surface;
		
		GLuint va_surface;
		
		GLuint vb_vertices;
		GLuint vb_normals;
		GLuint vb_colors;
		GLuint vb_wireframes;
		GLuint vb_faces;
		
		GLuint slot_vertices;
		GLuint slot_normals;
		GLuint slot_colors;
		
        GLuint number_of_lines;
        GLuint number_of_vertices;
        GLuint number_of_faces;

        QString shader_directory;
        bool is_initialized;
        bool is_visible;

        std::vector< float > colors;
		
		
};

#endif
