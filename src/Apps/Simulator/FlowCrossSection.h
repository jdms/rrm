#ifndef FLOWCROSSSECTION_H
#define FLOWCROSSSECTION_H

#include <QVector3D>
#include <QtCore/QDebug>
#include <QtWidgets/QApplication>
#include <QDir>

#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"
#include "Tucano/BoundingBox3.hpp"

class FlowCrossSection
{
public:

    enum class DIRECTION{ X, Y, Z, ANY };
    enum class SENSE{ BACK, FRONT };

    FlowCrossSection();
    FlowCrossSection( const QVector3D& n , const QVector3D& vmin, const QVector3D & vmax, FlowCrossSection::SENSE s = FlowCrossSection::SENSE::FRONT );

    void getCenterFromBoundingBox( const QVector3D& vmin, const QVector3D& vmax );
    inline void getPlaneEquation( float &a, float &b, float &c, float &d ) const
    { a = acoef; b = bcoef; c = ccoef; d = dcoef; }

    inline void setCenter( const QVector3D& c ){ center = c; }
    inline void setNormal( const QVector3D& n ){ normal = n; normal.normalize(); }
    inline void setNormal( FlowCrossSection::DIRECTION dir, FlowCrossSection::SENSE s = FlowCrossSection::SENSE::FRONT )
    {
        direction = dir;
        sense = s;
    }


    void solvePlaneEquation();
    void getVerticesPlane();

    float angleBetween( const QVector3D& v0,  const QVector3D& v1 );
    void sortVertices();

    void updatePosition( const QVector3D& updated );
    void updatePosition( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, float dir );


    void init();
    void load();
    void draw( const Eigen::Affine3f &V, const Eigen::Matrix4f &P, const float& scale  );

    void initShader( std::string directory );
    void clear();

    void resetBuffers();
    void deleteShaders();



    protected:

    float acoef;
    float bcoef;
    float ccoef;
    float dcoef;

    QVector3D min;
    QVector3D max;

    QVector3D normal;
    QVector3D center;
    QVector3D position;

    SENSE sense;
    DIRECTION direction;

    std::vector< QVector3D > vertices;

    Tucano::Shader* shader_plane;

    GLuint va_plane;
    GLuint bf_vertices_plane;
    GLuint bf_faces_plane;
    GLuint bf_wireframe_plane;
    GLuint bf_colors_plane;
    GLuint bf_normals_plane;

    GLuint number_of_vertices;
    GLuint number_of_faces;
    GLuint number_of_lines;
};

#endif // FLOWCROSSSECTION_H
