#ifndef BOUNDINGBOX3D_HPP
#define BOUNDINGBOX3D_HPP


#include <iostream>
#include <vector>


#include <QString>
#include <QVector3D>


#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"


#include "model_new/BoundaryRenderable.h"



class BoundingBox3D: public BoundaryRenderable
{

	public:
	

        BoundingBox3D( float min_x, float min_y, float min_z, float w, float h, float d = 0.5f );
		
        void initData();
        void init();
        void create();


		void resetBuffers();
		void initBuffers();
        void loadBuffers( const std::vector< float >& wireframe );
		
		void resetShaders();
		void initShaders();
		void reloadShaders();
		
				
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w, const int& h );
		
		
        inline void setCurrentDirectory( const std::string& directory ){ shader_directory = QString( directory.c_str() ); }
        inline bool initialized(){ return is_initialized;}
		
        inline float getWidth(){ return width; }
        inline float getHeight(){ return height; }
        inline float getDepth(){ return depth;  }

        inline void getMinimum( float& min_x, float& min_y, float& min_z )
        {
            min_x = minx;
            min_y = miny;
            min_z = minz;
        }

        inline void getDimension( float& w, float& h, float& d )
        {
            w = width;
            h = height;
            d = depth;
        }


        void update();

    protected:


        float width;
        float height;
        float depth;

		Tucano::Shader* shader_boundingbox;
		
		GLuint va_boundingbox;
		GLuint vb_vertices;
		GLuint slot_vertices;
        GLuint number_of_lines;

        QString shader_directory;
        bool is_initialized;


        float minx;
        float miny;
        float minz;
		
};

#endif
