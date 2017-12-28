/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "flow_visualization_canvas.h"
#include <map>

FlowVisualizationCanvas::FlowVisualizationCanvas(QWidget *parent, QString _current_dir) : QOpenGLWidget(parent)
{

    show_axis = true;

    this->current_directory = _current_dir.toStdString();
}

void FlowVisualizationCanvas::initializeGL()
{

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);


    GLint bufs;
    GLint samples;

    GLint minor;
    GLint major;

    glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
    glGetIntegerv(GL_SAMPLES, &samples);

    glGetIntegerv(GL_MINOR_VERSION, &minor);
    glGetIntegerv(GL_MAJOR_VERSION, &major);

    qDebug("Have %d buffers and %d samples", bufs, samples);
    qDebug("Have %d Minor and %d Major", minor, major);

    mesh.initializeShader(current_directory);
    crosssection.initShader(current_directory);

    /*axes.initShader(current_directory);
    axes.load();*/

    initializeShader();

}

void FlowVisualizationCanvas::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    loadBackGround();

    Eigen::Quaternionf q(Eigen::AngleAxisf(static_cast < float >(-0.5*M_PI), Eigen::Vector3f::UnitX()));
    Eigen::Affine3f M;
    M.setIdentity();
    M.rotate(q);

    Eigen::Affine3f V = camera.getViewMatrix();
    Eigen::Matrix4f P = camera.getProjectionMatrix();

    V = V * M;

    if (apply_crosssection == true)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        scale = 1.0f;
        crosssection.draw(V, P, scale);

        glDisable(GL_BLEND);

    }


    mesh.draw(V, P, scale, static_cast<float>(width()), static_cast<float>(height()));

    if (true)
    {
        //axes.draw(camera.getRotation(), camera.getViewport());
    }

}

void FlowVisualizationCanvas::resizeGL(int width, int height)
{

    glViewport(0, 0, width, height);

    camera.setViewport(Eigen::Vector2f(static_cast<float>(width), static_cast<float>(height)));
    camera.setPerspectiveMatrix(camera.getFovy(), (float)width / (float)height, 0.1f, 100.0f);

    scale = 1.5*(float)width / (float)height;
}

void FlowVisualizationCanvas::mouseMoveEvent(QMouseEvent *event)
{

    Eigen::Vector2f mouse_pos(event->x(), event->y());


    if ((event->modifiers() & Qt::ShiftModifier) && (event->buttons() & Qt::LeftButton))
    {
        camera.translateCamera(mouse_pos);
    }

    else if (event->buttons() & Qt::LeftButton)
    {
        camera.rotateCamera(mouse_pos);
    }


    else if (event->buttons() & Qt::RightButton)
    {

        float x = 2 * event->screenPos().x() / width() - 1.0f;
        float y = -2 * event->screenPos().y() / height() + 1.0f;
        float z = 1 - x*x - y*y;

        if (z >= 0) z = sqrt(z);
        else z = 0;

        double length = sqrt(x*x + y*y + z*z);
        if (length == 0.0f) return;

        x /= length;
        y /= length;
        z /= length;

        Eigen::Affine3f V = camera.getViewMatrix();
        Eigen::Matrix4f P = camera.getProjectionMatrix();

        crosssection.updatePosition(QVector3D(x, y, z));

        float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f;
        crosssection.getPlaneEquation(a, b, c, d);
        mesh.setCrossSectionClippingEquation(a, b, c, d, Eigen::Vector3f().UnitX());

    }

    update();

}

void FlowVisualizationCanvas::mousePressEvent(QMouseEvent *event)
{
    Eigen::Vector2f mouse_pos(event->x(), event->y());


    setFocus();

    if ((event->modifiers() & Qt::ShiftModifier) && (event->button() == Qt::LeftButton))
    {
        camera.translateCamera(mouse_pos);
    }

    if ((event->buttons() & Qt::RightButton))
    {

    }

}

void FlowVisualizationCanvas::mouseReleaseEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        camera.endTranslation();
        camera.endRotation();
    }

    update();
}

void FlowVisualizationCanvas::wheelEvent(QWheelEvent *event)
{
    float pos = event->delta() / float(speed_zoom);

    if (event->modifiers() & Qt::ShiftModifier)
        camera.incrementFov(pos);

    else
    {
        if (pos > 0){
            camera.increaseZoom(1.05f);
        }
        else if (pos < 0){
            camera.increaseZoom(1.0f / 1.05f);
        }


    }

    update();
}

void FlowVisualizationCanvas::keyPressEvent(QKeyEvent *event)
{

    Eigen::Affine3f V = camera.getViewMatrix();
    Eigen::Matrix4f P = camera.getProjectionMatrix();

    switch (event->key())
    {


    case Qt::Key_A:
    {
        show_axis = !show_axis;

    } break;

    case Qt::Key_C:
    {
    } break;


    case Qt::Key_U:
    {
        this->reloadShader();
    } break;

    case Qt::Key_R:
    {
        camera.reset();
        camera.increaseZoom(2.0f*1.05f);
    } break;

    case Qt::Key_X:
    {
        if (apply_crosssection == true)
        {
            crosssection.setNormal(QVector3D(1.0f, 0.0f, 0.0f));
            crosssection.init();
            crosssection.load();

            float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f;
            crosssection.getPlaneEquation(a, b, c, d);
            mesh.setCrossSectionClippingEquation(a, b, c, d, Eigen::Vector3f().UnitX());

        }

    } break;


    case Qt::Key_Y:
    {
        if (apply_crosssection == true)
        {
            crosssection.setNormal(QVector3D(0.0f, 1.0f, 0.0f));
            crosssection.init();
            crosssection.load();


            float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f;
            crosssection.getPlaneEquation(a, b, c, d);
            mesh.setCrossSectionClippingEquation(a, b, c, d, Eigen::Vector3f().UnitX());
        }



    } break;

    case Qt::Key_Z:
    {
        if (apply_crosssection == true)
        {

            crosssection.setNormal(QVector3D(0.0f, 0.0f, 1.0f));
            crosssection.init();
            crosssection.load();

            float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f;
            crosssection.getPlaneEquation(a, b, c, d);
            mesh.setCrossSectionClippingEquation(a, b, c, d, Eigen::Vector3f().UnitX());

        }


    } break;

    case Qt::Key_Up:
    {

        if (apply_crosssection == true)
        {
            float step = 1.0f;
            crosssection.updatePosition(V, P, step);

            float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f;
            crosssection.getPlaneEquation(a, b, c, d);
            mesh.setCrossSectionClippingEquation(a, b, c, d, Eigen::Vector3f().UnitX());
        }



    }break;
    case Qt::Key_Down:
    {

        if (apply_crosssection == true)
        {
            float step = -1.0f;
            crosssection.updatePosition(V, P, step);

            float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f;
            crosssection.getPlaneEquation(a, b, c, d);
            mesh.setCrossSectionClippingEquation(a, b, c, d, Eigen::Vector3f().UnitX());

        }


    }break;
    case Qt::Key_Right:
    {

        if (apply_crosssection == true)
        {
            float step = 1.0f;
            crosssection.updatePosition(V, P, step);

            float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f;
            crosssection.getPlaneEquation(a, b, c, d);
            mesh.setCrossSectionClippingEquation(a, b, c, d, Eigen::Vector3f().UnitX());

        }


    }break;
    case Qt::Key_Left:
    {

        if (apply_crosssection == true)
        {
            float step = -1.0f;
            crosssection.updatePosition(V, P, step);

            float a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f;
            crosssection.getPlaneEquation(a, b, c, d);
            mesh.setCrossSectionClippingEquation(a, b, c, d, Eigen::Vector3f().UnitX());
        }


    }break;


    case Qt::Key_Delete:
    {
        emit controller->clearAll();
    } break;


    default:
        break;


    }

    update();
}

void FlowVisualizationCanvas::initializeShader()
{
    background = new Tucano::Shader("BackGround", (current_directory + "Shaders/DummyQuad.vert"),
        (current_directory + "Shaders/DummyQuad.frag"),
        (current_directory + "Shaders/DummyQuad.geom"), "", "");
    background->initialize();


    glGenVertexArrays(1, &va_background);
    glBindVertexArray(va_background);

    /// Requesting Vertex Buffers to the GPU
    glGenBuffers(1, &vb_background);
    glBindBuffer(GL_ARRAY_BUFFER, vb_background);
    glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);


}

void FlowVisualizationCanvas::setController( FlowVisualizationController *c )
{
    controller = c;
}
/// Debug Purpose
void FlowVisualizationCanvas::reloadShader()
{
    mesh.reloadShader();
    //axes.reloadShader();

    glFinish();
    update();
}

void FlowVisualizationCanvas::loadBackGround()
{
    glDisable( GL_DEPTH_TEST );
    glDepthMask( GL_FALSE );

    background->bind();

    background->setUniform( "viewportSize", width(), height() );

    glBindVertexArray ( va_background );
        glDrawArrays ( GL_POINTS , 0 , 1 );
    glBindVertexArray ( 0 );

    background->unbind();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void FlowVisualizationCanvas::showBoundingBox( bool status )
{
    mesh.showBoundingBox( status );
    update();
}

void FlowVisualizationCanvas::setCrossSectionNormalCoordinates( float X, float Y, float Z )
{
    apply_crosssection = true;

    float xm, xM, ym, yM, zm, zM;
    mesh.getBoundingBox( xm, xM, ym, yM, zm, zM );

    crosssection.setNormal( QVector3D( X, Y, Z ) );
    crosssection.getCenterFromBoundingBox( QVector3D( xm, ym, zm ), QVector3D( xM, yM, zM ) );
    crosssection.init();
    crosssection.load();


    float a = 0.0f, b = 0.0f , c = 0.0f , d = 0.0f;
    crosssection.getPlaneEquation( a, b, c, d );
    mesh.setCrossSectionClippingEquation( a, b, c, d , Eigen::Vector3f().UnitX());

    update();
}

void FlowVisualizationCanvas::showRegions()
{

    std::vector< QColor > colors;
    std::vector< double > values;
    std::vector<int> ids;
    //std::map< double, QVector3D> unique_colors = controller->getRegionsColor( colors , values, current_colormap, ids);

    std::vector< QVector3D > colors_;
    values.clear();
    //for( std::map< double, QVector3D>::iterator it = unique_colors.begin(); it != unique_colors.end(); ++it )
    //{
    //    values.push_back( it->first);
    //     colors_.push_back( it->second);
    //}


    update();

}

void FlowVisualizationCanvas::disableCrossSection( bool status )
{

    apply_crosssection = status;

    if( status == false ){
        mesh.disableCrossSection();
        glDisable(GL_CLIP_DISTANCE0);
    }
    update();

}

void FlowVisualizationCanvas::updateMesh()
{


    std::vector< double > vertices;
    std::vector< unsigned int > faces;

    controller->buildSurfaceSkeleton( vertices, faces );


    mesh.setMeshType( Mesh::TYPE::QUADRILATERAL );

    std::vector<float> vertices_float;

    for (auto v : vertices)
    {
        vertices_float.push_back(static_cast<float>(v));
    }


    mesh.buildBoundingBox(vertices_float);

    mesh.setSkeletonGeometry(faces, vertices_float);


    camera.reset();
    camera.increaseZoom(2.0f*1.05f);

    update();

}


void FlowVisualizationCanvas::updateTriangleMesh( const std::vector< double >& vertices,
                                          const std::vector< std::size_t >& faces )
{



    std::vector<float> vertices_float;
    for (auto v : vertices)
    {
        vertices_float.push_back(static_cast<float>(v));
    }

    std::vector<unsigned int> faces_;
    for (auto f : faces)
    {
        faces_.push_back(static_cast<unsigned int>(f));
    }

    mesh.buildBoundingBox(vertices_float);

    mesh.setMeshType( Mesh::TYPE::TRIANGLES );
    mesh.setTriangleSkeletonGeometry( faces_, vertices_float );
//    mesh.setSkeletonGeometry(faces, vertices_float);
//    mesh.setMeshType( Mesh::TYPE::TETRAHEDRAL );
//    mesh.setTetrahedronGeometry(faces_, vertices_float);



    camera.reset();
    camera.increaseZoom(2.0f*1.05f);

    update();

}


void FlowVisualizationCanvas::updateCornerPoint( )
{


    std::vector< float > vertices_double;
    std::vector< unsigned int > edges;
    std::vector< unsigned int > faces ;

    controller->updateCornerPoint( vertices_double, edges, faces );


    std::vector< float > vertices;
    vertices.assign( vertices_double.begin(), vertices_double.end() );

    mesh.setMeshType( Mesh::TYPE::HEXAHEDRAL );
    mesh.setHexahedronGeometry(faces, vertices);


    update();

}

void FlowVisualizationCanvas::updateVolumetricMesh()
{

    std::vector< float > raw_vertices, normalized_vertices;
    std::vector< unsigned int > edges;
    std::vector< unsigned int > faces ;


    controller->updateVolumetricMesh( raw_vertices, normalized_vertices, edges, faces);

    mesh.setMeshType( Mesh::TYPE::TETRAHEDRAL );

    /// OpenVolumeMesh
    mesh.setTetrahedronGeometry(faces, normalized_vertices);

    update();

}

void FlowVisualizationCanvas::clear()
{
    mesh.clear();

    crosssection.clear();

    apply_crosssection = false;

    camera.reset();

    update();
}

FlowVisualizationCanvas::~FlowVisualizationCanvas()
{
    mesh.resetBuffers();
    crosssection.resetBuffers();
    //axes.resetBuffers();

}
/// Get the current boundingobx depth in the extrusion framework
float FlowVisualizationCanvas::getDepth() const
{
    return mesh.getDepth();
}


void FlowVisualizationCanvas::setDefaultColor()
{
    mesh.setDefaultColor();
    update();

}

void FlowVisualizationCanvas::setColors(const std::vector< float >& colors)
{
    mesh.setMeshColor(colors);
    update();

}

void FlowVisualizationCanvas::updateWellsPosition(int _number_of_wells_, const std::map<int, Eigen::Vector4d>& _positions, const std::map<int, int>& _types, const std::map<int, Eigen::Vector2d>& _range)
{
    mesh.loadWellPosition(_number_of_wells_, _positions, _types, _range);
    update();
}
