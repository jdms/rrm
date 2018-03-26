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

#include "flow_crosssection.h"


# define M_PI           3.14159265358979323846


FlowCrossSection::FlowCrossSection()
{
}

FlowCrossSection::FlowCrossSection(  const QVector3D& n , const QVector3D& vmin, const QVector3D & vmax, FlowCrossSection::SENSE s )
{
    getCenterFromBoundingBox( vmin, vmax );
    setNormal( n );

}

void FlowCrossSection::getCenterFromBoundingBox( const QVector3D& vmin, const QVector3D& vmax )
{
    min.setX( vmin.x() );
    min.setY( vmin.y() );
    min.setZ( vmin.z() );

    max.setX( vmax.x() );
    max.setY( vmax.y() );
    max.setZ( vmax.z() );

    center.setX( ( vmin.x() + vmax.x() )* 0.5f );
    center.setY( ( vmin.y() + vmax.y() )* 0.5f );
    center.setZ( ( vmin.z() + vmax.z() )* 0.5f );


}

void FlowCrossSection::init()
{

    position.setX( center.x() );
    position.setY( center.y() );
    position.setZ( center.z() );

    solvePlaneEquation();
    getVerticesPlane();


}


void FlowCrossSection::solvePlaneEquation()
{
    acoef = normal.x();
    bcoef = normal.y();
    ccoef = normal.z();
    dcoef = -acoef*position.x() - bcoef*position.y() - ccoef*position.z();
}

void FlowCrossSection::getVerticesPlane()
{
    vertices.clear();

    QVector3D v0( min.x(), max.y(), max.z() );
    QVector3D v1( max.x(), max.y(), max.z() );
    QVector3D v2( max.x(), max.y(), min.z() );
    QVector3D v3( min.x(), max.y(), min.z() );


    QVector3D v4( min.x(), min.y(), max.z() );
    QVector3D v5( max.x(), min.y(), max.z() );
    QVector3D v6( max.x(), min.y(), min.z() );
    QVector3D v7( min.x(), min.y(), min.z() );


    QVector3D edge1 = v1 - v0;
    QVector3D edge2 = v2 - v1;
    QVector3D edge3 = v3 - v2;
    QVector3D edge4 = v0 - v3;

    QVector3D edge5 = v5 - v4;
    QVector3D edge6 = v6 - v5;
    QVector3D edge7 = v7 - v6;
    QVector3D edge8 = v4 - v7;

    QVector3D edge9 = v4 - v0;
    QVector3D edge10 = v5 - v1;
    QVector3D edge11 = v7 - v3;
    QVector3D edge12 = v6 - v2;


    float s1 = -( acoef*v0.x() + bcoef*v0.y() + ccoef*v0.z() + dcoef )/( QVector3D::dotProduct( normal, edge1 ) );
    if( s1 >= 0.0f && s1 <= 1.0f)
    {
        QVector3D ps1 = v0 + s1*edge1;
        vertices.push_back( ps1 );
    }

    float s2 = -( acoef*v1.x() + bcoef*v1.y() + ccoef*v1.z() + dcoef )/( QVector3D::dotProduct( normal, edge2 ) );
    if( s2 >= 0.0f && s2 <= 1.0f )
    {
        QVector3D ps2 = v1 + s2*edge2;
        vertices.push_back( ps2 );
    }

    float s3 = -( acoef*v2.x() + bcoef*v2.y() + ccoef*v2.z() + dcoef )/( QVector3D::dotProduct( normal, edge3 ) );
    if( s3 >= 0.0f && s3 <= 1.0f )
    {
        QVector3D ps3 = v2 + s3*edge3;
        vertices.push_back( ps3 );
    }

    float s4 = -( acoef*v3.x() + bcoef*v3.y() + ccoef*v3.z() + dcoef )/( QVector3D::dotProduct( normal, edge4 ) );
    if( s4 >= 0.0f && s4 <= 1.0f )
    {
        QVector3D ps4 = v3 + s4*edge4;
        vertices.push_back( ps4 );
    }

    float s5 = -( acoef*v4.x() + bcoef*v4.y() + ccoef*v4.z() + dcoef )/( QVector3D::dotProduct( normal, edge5 ) );
    if( s5 >= 0.0f && s5 <= 1.0f )
    {
        QVector3D ps5 = v4 + s5*edge5;
        vertices.push_back( ps5 );
    }

    float s6 = -( acoef*v5.x() + bcoef*v5.y() + ccoef*v5.z() + dcoef )/( QVector3D::dotProduct( normal, edge6 ) );
    if( s6 >= 0.0f && s6 <= 1.0f )
    {
        QVector3D ps6 = v5 + s6*edge6;
        vertices.push_back( ps6 );
    }


    float s7 = -( acoef*v6.x() + bcoef*v6.y() + ccoef*v6.z() + dcoef )/( QVector3D::dotProduct( normal, edge7 ) );
    if( s7 >= 0.0f && s7 <= 1.0f )
    {
        QVector3D ps7 = v6 + s7*edge7;
        vertices.push_back( ps7 );
    }

    float s8 = -( acoef*v7.x() + bcoef*v7.y() + ccoef*v7.z() + dcoef )/( QVector3D::dotProduct( normal, edge8 ) );
    if( s8 >= 0.0f && s8 <= 1.0f )
    {
        QVector3D ps8 = v7 + s8*edge8;
        vertices.push_back( ps8 );
    }

    float s9 = -( acoef*v0.x() + bcoef*v0.y() + ccoef*v0.z() + dcoef )/( QVector3D::dotProduct( normal, edge9 ) );
    if( s9 >= 0.0f && s9 <= 1.0f )
    {
        QVector3D ps9 = v0 + s9*edge9;
        vertices.push_back( ps9 );
    }

    float s10 = -( acoef*v1.x() + bcoef*v1.y() + ccoef*v1.z() + dcoef )/( QVector3D::dotProduct( normal, edge10 ) );
    if( s10 >= 0.0f && s10 <= 1.0f )
    {
        QVector3D ps10 = v1 + s10*edge10;
        vertices.push_back( ps10 );
    }

    float s11 = -( acoef*v3.x() + bcoef*v3.y() + ccoef*v3.z() + dcoef )/( QVector3D::dotProduct( normal, edge11 ) );
    if( s11 >= 0.0f && s11 <= 1.0f )
    {
        QVector3D ps11 = v3 + s11*edge11;
        vertices.push_back( ps11 );
    }


    float s12 = -( acoef*v2.x() + bcoef*v2.y() + ccoef*v2.z() + dcoef )/( QVector3D::dotProduct( normal, edge12 ) );
    if( s12 >= 0.0f && s12 <= 1.0f )
    {
        QVector3D ps12 = v2 + s12*edge12;
        vertices.push_back( ps12 );
    }


    sortVertices();

}


float FlowCrossSection::angleBetween( const QVector3D& v0,  const QVector3D& v1 )
{

    float dot = v0.x()*v1.x() + v0.y()*v1.y() + v0.z()*v1.z();
    QVector3D cross = QVector3D::crossProduct( v0, v1 );
    float det = QVector3D::dotProduct( normal, cross );

    float angle = (float) atan2( det, dot );
    if( angle < 0.0f )
        angle += (float) 2*M_PI;

    return angle;


}



void FlowCrossSection::sortVertices()
{


    unsigned int vertices_number = vertices.size();
    int save = 1;

    if( vertices.empty() == true )
        return;


    float minx = vertices[ 0 ].x() ;
    float miny = vertices[ 0 ].y() ;
    float minz = vertices[ 0 ].z() ;

    float maxx = vertices[ 0 ].x() ;
    float maxy = vertices[ 0 ].y() ;
    float maxz = vertices[ 0 ].z() ;


    for( unsigned int i = 1; i < vertices_number; ++i )
    {
        if( vertices[ i ].x() <= minx ) minx = vertices[ i ].x();
        if( vertices[ i ].y() <= miny ) miny = vertices[ i ].y();
        if( vertices[ i ].z() <= minz ) minz = vertices[ i ].z();

        if( vertices[ i ].x() >= maxx ) maxx = vertices[ i ].x();
        if( vertices[ i ].y() >= maxy ) maxy = vertices[ i ].y();
        if( vertices[ i ].z() >= maxz ) maxz = vertices[ i ].z();

    }


    center1 = QVector3D( ( maxx + minx )*0.5f, ( maxy + miny )*0.5f, ( maxz + minz )*0.5f );


     QVector3D v0 = vertices[ 0 ] - center1;

    for( unsigned int i = 1; i < vertices_number; ++i )
    {


        QVector3D vi = vertices[ i ] - center1;

        float min_angle = angleBetween( v0, vi );

        bool find_minimum = false;
        for( unsigned int j = i + 1; j < vertices_number; ++j  )
        {

            QVector3D vj = vertices[ j ] - center1;

            float anglej = angleBetween( v0 , vj );


            if( anglej <= min_angle )
            {
                min_angle = anglej;
                save = (int) j;
                find_minimum = true;
            }
        }

        if( find_minimum == true )
        {
            QVector3D temp = vertices[ i ];
            vertices[ i ] = vertices[ save ];
            vertices[ save ] = temp;
        }
    }


}


void FlowCrossSection::updatePosition( const QVector3D& updated )
{


    QVector3D direction_vector = updated - QVector3D( position.x(), position.y(), position.z() );
    float step = direction_vector.length()*0.003;

    float dot = QVector3D::dotProduct( direction_vector, QVector3D( 1.0f, 1.0f, 0.0f ) );
    bool same_direction = ( dot > 0.0f )? true:false;

    if( same_direction == false )
        step *= -1.0f;

    position = normal  * step + position;

    solvePlaneEquation();
    getVerticesPlane();

    load();

}

void FlowCrossSection::updatePosition( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, float dir )
{


    position = normal * dir * 0.003f + position;

    solvePlaneEquation();
    getVerticesPlane();



    load();

}


void FlowCrossSection::initShader( std::string directory )
{

    shader_plane = new Tucano::Shader(  "shader_plane", ( directory + "shaders/vertex_plane_shader.vert" ) , ( directory + "shaders/fragment_plane_shader.frag" ), "", "", "" );
    shader_plane->initialize();

    glGenVertexArrays( 1, &va_plane );
    glBindVertexArray( va_plane );

        glGenBuffers( 1, &bf_vertices_plane );
        glBindBuffer( GL_ARRAY_BUFFER, bf_vertices_plane );
        glBufferData( GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 0 );

        glGenBuffers( 1, &bf_wireframe_plane );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_wireframe_plane );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );


    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    glBindVertexArray( 0 );

}


void FlowCrossSection::load()
{


    number_of_vertices = vertices.size();

    std::vector< float > vertices_coordinates;
    for( unsigned int i = 0; i < number_of_vertices; ++i )
    {
        //vertices_coordinates.push_back( vertices[ i ].x() );
        //vertices_coordinates.push_back( vertices[ i ].y() );
        //vertices_coordinates.push_back( vertices[ i ].z() );
		vertices_coordinates.push_back( vertices[ i ].x() );
		vertices_coordinates.push_back( vertices[ i ].z() );
		vertices_coordinates.push_back( vertices[ i ].y() );

    }

    glBindBuffer( GL_ARRAY_BUFFER, bf_vertices_plane );
    glBufferData( GL_ARRAY_BUFFER, vertices_coordinates.size()*sizeof( GLfloat ), vertices_coordinates.data(), GL_STATIC_DRAW );


    vector< unsigned int > lines;
    for( unsigned int i = 0; i < number_of_vertices; ++i )
    {
        lines.push_back( i );

        int next = i + 1;
        if( next >= (int)number_of_vertices )
            next = 0;

        lines.push_back( next );
    }

    number_of_lines = lines.size();
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_wireframe_plane );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, number_of_lines*sizeof( GLuint ), lines.data(), GL_STATIC_DRAW );

  glBufferData( GL_ELEMENT_ARRAY_BUFFER, number_of_lines*sizeof( GLuint ), lines.data(), GL_STATIC_DRAW );

}

void FlowCrossSection::draw( const Eigen::Affine3f &V, const Eigen::Matrix4f &P, const float& scale )
{

    Eigen::Affine3f M;
    M.setIdentity();


    shader_plane->bind();
    shader_plane->setUniform( "mmatrix", M );
    shader_plane->setUniform( "vmatrix", V );
    shader_plane->setUniform( "pmatrix", P );
    shader_plane->setUniform( "scale", scale);

    glBindVertexArray( va_plane );


        glLineWidth( 4.0f );

        shader_plane->setUniform( "in_color", Eigen::Vector3f( 1.0f, 1.0f, 0.0f ) );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_wireframe_plane );
        glDrawElements( GL_LINES, number_of_lines, GL_UNSIGNED_INT, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );


        shader_plane->setUniform( "in_color", Eigen::Vector3f( 1.0f, 0.0f, 1.0f ) );
        glPointSize( 4.0f );
        glDrawArrays( GL_POINTS, 0, number_of_vertices );



    glBindVertexArray( 0 );

    shader_plane->unbind();


}

void FlowCrossSection::clear()
{
    acoef = 0.0f;
    bcoef = 0.0f;
    ccoef = 0.0f;
    dcoef = 0.0f;

    min.setX( 0.0f );
    min.setY( 0.0f );
    min.setZ( 0.0f );
    max.setX( 0.0f );
    max.setY( 0.0f );
    max.setZ( 0.0f );

    normal.setX( 0.0f );
    normal.setY( 0.0f );
    normal.setZ( 0.0f );
    center.setX( 0.0f );
    center.setY( 0.0f );
    center.setZ( 0.0f );
    position.setX( 0.0f );
    position.setY( 0.0f );
    position.setZ( 0.0f );


    vertices.clear();

    number_of_vertices = 0;
    number_of_faces = 0;
    number_of_lines = 0;
}


void FlowCrossSection::resetBuffers()
{
    deleteShaders();

    if( va_plane )
    {
        glDeleteVertexArrays( 1, &va_plane );
        if( bf_vertices_plane )
            glDeleteBuffers(1, &bf_vertices_plane);
        if( bf_faces_plane )
            glDeleteBuffers(1, &bf_faces_plane);
        if( bf_wireframe_plane )
            glDeleteBuffers(1, &bf_wireframe_plane);
        if( bf_colors_plane )
            glDeleteBuffers(1, &bf_colors_plane);
        if( bf_normals_plane )
            glDeleteBuffers(1, &bf_normals_plane);

    }


    va_plane = 0;
    bf_vertices_plane = 0;
    bf_faces_plane = 0;
    bf_wireframe_plane = 0;
    bf_colors_plane = 0;
    bf_normals_plane = 0;

    number_of_vertices = 0;
    number_of_faces = 0;
    number_of_lines = 0;


}

void FlowCrossSection::deleteShaders()
{

    if (shader_plane)
    {
        delete (shader_plane);
        shader_plane = nullptr;
    }


}
