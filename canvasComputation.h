#ifndef CANVASCOMPUTATION_H
#define CANVASCOMPUTATION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions_4_5_Core >
#include <QMatrix4x4>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "FlowVisualizationController.h"


using namespace std;


inline std::string read_shader_file( const char *file_path )
{
    std::string content;
    std::ifstream fileStream( file_path, std::ios::in );

    if( !fileStream.is_open() ) {
        std::cerr << "Could not read file " << file_path << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while( !fileStream.eof() ) {
        std::getline( fileStream, line );
        content.append( line + "\n" );
    }

    fileStream.close();
    return content;

}


class CanvasComputation: public QGLWidget, public QOpenGLFunctions_4_5_Core
{
    Q_OBJECT


    public:

        CanvasComputation( QGLFormat format, QWidget *parent = 0 );


    protected:

        void initializeGL();
        void resizeGL( int width, int height );
        void paintGL();

        void initializeShaders();
        void deleteBuffers();

        void sendMeshGPU();
        void sendColorsGPU();


    private:

        GLuint program;

        GLuint vao_mesh;
        GLuint bf_faces;
        GLuint number_of_faces;
        GLuint number_of_vertices;

        FlowVisualizationController *flowvisualizationc;
};

#endif // CANVASCOMPUTATION_H
