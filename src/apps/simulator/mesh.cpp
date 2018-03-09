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

#include "mesh.h"


#include "Tucano/BoundingBox3.hpp"

Mesh::Mesh()
{
    show_vertices = true;
    show_edges = true;
    show_faces = true;
    show_bbox = false;
    this->centre_ = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
    float depth_ = 0.0f;

    apply_crosssection_clipping = false;


	/// FIXME January
	shader_mesh				= nullptr;
	shader_mesh_cornerpoint = nullptr;;
	shader					= nullptr;
	quad_phong_				= nullptr;
	shader_bbox				= nullptr;
	well_shader				= nullptr;

}

void Mesh::setMeshType( Mesh::TYPE t )
{
    mesh_type = t;
}

void Mesh::setConstantColor( const float r, const float g, const float b )
{

}

void Mesh::setCrossSectionClippingEquation(const float& a, const float& b, const float& c, const float& d, Eigen::Vector3f _centre)
{

    apply_crosssection_clipping = true;

    coefACrossSectionEquation = a;
    coefBCrossSectionEquation = b;
    coefCCrossSectionEquation = c;
    coefDCrossSectionEquation = d;
}

void Mesh::disableCrossSection()
{
    apply_crosssection_clipping = false;

}

/// Dimension in Qt Space
float Mesh::getDepth() const
{
    return this->depth_;
}

std::tuple<Eigen::Vector3f, Eigen::Vector3f> Mesh::getBoudingBoxDimension() const
{
    return std::make_tuple(dim_min, dim_max);
}

void Mesh::showVertices( bool status )
{
    show_vertices = status;
}

void Mesh::showEdges( bool status )
{
    show_edges = status;
}

void Mesh::showFaces( bool status )
{
    show_faces = status;
}

bool Mesh::showVertices() const
{
    return show_vertices;
}

bool Mesh::showEdges() const
{
    return show_edges;
}

bool Mesh::showFaces() const
{
    return show_faces;
}

void Mesh::showBoundingBox( bool status )
{
    show_bbox = status;
}

bool Mesh::showBoundingBox() const
{
    return show_bbox;
}

void Mesh::reloadShader()
{
    shader_mesh->reloadShaders();
    shader_mesh_cornerpoint->reloadShaders();
    well_shader->reloadShaders();
    quad_phong_->reloadShaders();
    shader_bbox->reloadShaders();
    shader->reloadShaders();
}

void Mesh::initializeShader( std::string directory )
{


    shader_mesh = new Tucano::Shader( "shader_mesh", ( directory + "shaders/FlowDefault.vert" ),
                                                     (directory + "shaders/FlowDefault.frag"),
                                                     (directory + "shaders/FlowDefault.geom"),
                                                      "", "") ;
    shader_mesh->initialize();



    shader_mesh_cornerpoint = new Tucano::Shader( "shader_mesh_cornerpoint", ( directory + "shaders/FlowQuadrilaterals.vert" ),
                                                     (directory + "shaders/FlowQuadrilaterals.frag"),
                                                     (directory + "shaders/FlowQuadrilaterals.geom"),
                                                      "", "") ;
    shader_mesh_cornerpoint->initialize();

    shader_bbox = new Tucano::Shader(  "shader_bbox", ( directory + "shaders/BoundingBox.vert" ),
                                                      ( directory + "shaders/BoundingBox.frag" ),
                                                      (directory + "shaders/BoundingBox.geom"), "", "");
    shader_bbox->initialize();

    well_shader = new Tucano::Shader("well shader", (directory + "shaders/well_shader.vert"),
                                                    (directory + "shaders/well_shader.frag"), "", "", "");
    well_shader->initialize();


    quad_phong_ = new Tucano::Shader("Quad Phong", (directory + "shaders/QuadPhong.vert"),
                                                   (directory + "shaders/QuadPhong.frag"),
                                                   (directory + "shaders/QuadPhong.geom"),
                                                    "", "");
    quad_phong_->initialize();

    shader = new Tucano::Shader( "Surface", ( directory + "shaders/gouraud_surface.vert" ),
                                            ( directory + "shaders/gouraud_surface.frag" ),
                                            "", "", "" );

    shader->initialize();


    // Well Points
    entities_["WellPoints"] = OpenGLGeometry();
    entities_["WellLines"] = OpenGLGeometry();
    entities_["WellSegments"] = OpenGLGeometry();
    entities_["Sketchies"] = OpenGLGeometry();
    entities_["Hexahedron"] = OpenGLGeometry();
    entities_["Tetrahedron"] = OpenGLGeometry();
    entities_["BoundingBox"] = OpenGLGeometry();


    for (auto& gl : entities_)
    {
        glGenVertexArrays(1, &gl.second.vertexArray_);
        glBindVertexArray(gl.second.vertexArray_);

        glGenBuffers(1, &gl.second.vertexBuffer_vertices_);
        glBindBuffer(GL_ARRAY_BUFFER, gl.second.vertexBuffer_vertices_);
        glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &gl.second.vertexBuffer_normals_);
        glBindBuffer(GL_ARRAY_BUFFER, gl.second.vertexBuffer_normals_);
        glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &gl.second.vertexBuffer_colors_);
        glBindBuffer(GL_ARRAY_BUFFER, gl.second.vertexBuffer_colors_);
        glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &gl.second.vertexBuffer_elemets_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl.second.vertexBuffer_elemets_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

}

void Mesh::load()
{



}

void Mesh::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const float& scale , const float width, const float height )
{

    Eigen::Affine3f M;
    M.setIdentity();

    if( mesh_type == TYPE::QUADRILATERAL )
    {
        quad_phong_->bind();

        quad_phong_->setUniform("WIN_SCALE", width, height);
        quad_phong_->setUniform("ModelMatrix", Eigen::Affine3f::Identity().data(), 4, GL_FALSE, 1);
        quad_phong_->setUniform("ViewMatrix", V.data(), 4, GL_FALSE, 1);
        quad_phong_->setUniform("ProjectionMatrix", P.data(), 4, GL_FALSE, 1);
        quad_phong_->setUniform("isClip", GL_FALSE);
        glBindVertexArray(entities_.at("Sketchies").vertexArray_);


        if( apply_crosssection_clipping == true )
        {
            glEnable(GL_CLIP_DISTANCE0);
            quad_phong_->setUniform("ClipPlane", coefACrossSectionEquation, coefBCrossSectionEquation, coefCCrossSectionEquation, coefDCrossSectionEquation);
            quad_phong_->setUniform("ClipPlaneCentre", this->centre_);
            quad_phong_->setUniform("isClip", GL_TRUE);

        }

        if( show_faces == true )
        {

            glDrawArrays(GL_LINES_ADJACENCY, 0, entities_.at("Sketchies").number_of_elements_);
        }

        glBindVertexArray( 0 );

        quad_phong_->unbind();

    }
    if( mesh_type == TYPE::TRIANGLES )
    {

        //shader->bind();

        //shader->setUniform( "ModelMatrix" , M );
        //shader->setUniform( "ViewMatrix" , V );
        //shader->setUniform( "ProjectionMatrix" , P );
        //shader->setUniform( "WIN_SCALE" , (float) width , (float) height );


        //glEnable( GL_DEPTH_TEST );



        //    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        //    glEnable( GL_POLYGON_OFFSET_FILL );
        //    glPolygonOffset( 2.0f, 1.0f );


        //    shader->setUniform( "solid" , true );
        //    shader->setUniform( "testing" , false );


		shader_mesh->bind();

		shader_mesh->setUniform("WIN_SCALE", width, height);

		shader_mesh->setUniform("ModelMatrix", Eigen::Affine3f::Identity().data(), 4, GL_FALSE, 1);
		shader_mesh->setUniform("ViewMatrix", V.data(), 4, GL_FALSE, 1);
		shader_mesh->setUniform("ProjectionMatrix", P.data(), 4, GL_FALSE, 1);
		shader_mesh->setUniform("isClip", GL_FALSE);

            glBindVertexArray( entities_.at("Sketchies").vertexArray_ );

                glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,  entities_.at("Sketchies").vertexBuffer_elemets_ );
                glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

            glBindVertexArray ( 0 );

		shader_mesh->unbind();
        //glDisable(GL_POLYGON_OFFSET_FILL);



        //glEnable( GL_DEPTH_TEST );
        //glDepthFunc( GL_LEQUAL );
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        //shader->setUniform( "solid" , false );

        //glBindVertexArray( entities_.at("Sketchies").vertexArray_ );

        //    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, entities_.at("Sketchies").vertexBuffer_elemets_ );
        //    glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

        //glBindVertexArray ( 0 );

        //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        //shader->unbind();


    }
    else if (mesh_type == TYPE::HEXAHEDRAL)
    {
        shader_mesh_cornerpoint->bind();

        shader_mesh_cornerpoint->setUniform("WIN_SCALE", width, height);
        shader_mesh_cornerpoint->setUniform("ModelMatrix", Eigen::Affine3f::Identity().data(), 4, GL_FALSE, 1);
        shader_mesh_cornerpoint->setUniform("ViewMatrix", V.data(), 4, GL_FALSE, 1);
        shader_mesh_cornerpoint->setUniform("ProjectionMatrix", P.data(), 4, GL_FALSE, 1);
        shader_mesh_cornerpoint->setUniform("isClip", GL_FALSE);
        //glBindVertexArray( va_mesh );
        glBindVertexArray(entities_.at("Hexahedron").vertexArray_);

        if (apply_crosssection_clipping == true)
        {
            glEnable(GL_CLIP_DISTANCE0);
            shader_mesh_cornerpoint->setUniform("ClipPlane", coefACrossSectionEquation, coefBCrossSectionEquation, coefCCrossSectionEquation, coefDCrossSectionEquation);
            shader_mesh_cornerpoint->setUniform("ClipPlaneCentre", this->centre_);
            shader_mesh_cornerpoint->setUniform("isClip", GL_TRUE);

        }

        if (show_faces == true)
        {

            //glDrawArrays ( GL_LINES_ADJACENCY , 0 , vector_triangles_size );
            glDrawArrays(GL_LINES_ADJACENCY, 0, entities_.at("Hexahedron").number_of_elements_);

        }

        glBindVertexArray(0);

        shader_mesh_cornerpoint->unbind();

    }
    else if (mesh_type == TYPE::TETRAHEDRAL)
    {

        shader_mesh->bind();

        shader_mesh->setUniform("WIN_SCALE", width, height);

        shader_mesh->setUniform("ModelMatrix", Eigen::Affine3f::Identity().data(), 4, GL_FALSE, 1);
        shader_mesh->setUniform("ViewMatrix", V.data(), 4, GL_FALSE, 1);
        shader_mesh->setUniform("ProjectionMatrix", P.data(), 4, GL_FALSE, 1);
        shader_mesh->setUniform("isClip", GL_FALSE);
        //glBindVertexArray( va_mesh );
        glBindVertexArray(entities_.at("Tetrahedron").vertexArray_);

        if( apply_crosssection_clipping == true )
        {
            glEnable(GL_CLIP_DISTANCE0);
            shader_mesh->setUniform("ClipPlane", coefACrossSectionEquation, coefBCrossSectionEquation, coefCCrossSectionEquation, coefDCrossSectionEquation);
            shader_mesh->setUniform( "ClipPlaneCentre", this->centre_);
            shader_mesh->setUniform("isClip", GL_TRUE);

        }

        if( show_faces == true )
        {

             //glDrawArrays( GL_TRIANGLES, 0, vector_triangles_size );
            glDrawArrays(GL_TRIANGLES, 0, entities_.at("Tetrahedron").number_of_elements_);

        }

        glBindVertexArray( 0 );
        shader_mesh->unbind();

    }
    else
    {
        //std::cout << "Nohting to be renderer" << std::endl;
    }

    if( true )
    {
        if (entities_.at("BoundingBox").number_of_elements_ > 0)
        {

            shader_bbox->bind();

            shader_bbox->setUniform("WIN_SCALE", width, height);
            shader_bbox->setUniform("ModelMatrix", Eigen::Affine3f::Identity().data(), 4, GL_FALSE, 1);
            shader_bbox->setUniform("ViewMatrix", V.data(), 4, GL_FALSE, 1);
            shader_bbox->setUniform("ProjectionMatrix", P.data(), 4, GL_FALSE, 1);

            glBindVertexArray(entities_.at("BoundingBox").vertexArray_);

            glDrawArrays(GL_LINES_ADJACENCY, 0, entities_.at("BoundingBox").number_of_elements_);

            glBindVertexArray(0);

            shader_bbox->unbind();

        }
        if ( entities_.at("WellPoints").number_of_elements_ > 0)
        {
            glPointSize(7);

            well_shader->bind();

            well_shader->setUniform("ModelMatrix", Eigen::Affine3f::Identity().data(), 4, GL_FALSE, 1);
            well_shader->setUniform("ViewMatrix", V.data(), 4, GL_FALSE, 1);
            well_shader->setUniform("ProjectionMatrix", P.data(), 4, GL_FALSE, 1);

            glBindVertexArray(entities_.at("WellPoints").vertexArray_);

            glDrawArrays(GL_POINTS, 0, entities_.at("WellPoints").number_of_elements_);

            glBindVertexArray(0);

            //std::cout << " Vertex Array " << entities_.at("WellPoints").vertexArray_ << "  " << entities_.at("WellPoints").number_of_elements_ << std::endl;

            well_shader->unbind();
        }



        if (entities_.at("WellLines").number_of_elements_ > 0)
        {
            glLineWidth(5);

            well_shader->bind();

            well_shader->setUniform("ModelMatrix", Eigen::Affine3f::Identity().data(), 4, GL_FALSE, 1);
            well_shader->setUniform("ViewMatrix", V.data(), 4, GL_FALSE, 1);
            well_shader->setUniform("ProjectionMatrix", P.data(), 4, GL_FALSE, 1);

            glBindVertexArray(entities_.at("WellLines").vertexArray_);

            glDrawArrays(GL_LINES, 0, entities_.at("WellLines").number_of_elements_);

            glBindVertexArray(0);

            //std::cout << " Vertex Array " << entities_.at("WellLines").vertexArray_ << "  " << entities_.at("WellLines").number_of_elements_ << std::endl;

            well_shader->unbind();
        }

        if (entities_.at("WellSegments").number_of_elements_ > 0)
        {
            glLineWidth(7);

            well_shader->bind();

            well_shader->setUniform("ModelMatrix", Eigen::Affine3f::Identity().data(), 4, GL_FALSE, 1);
            well_shader->setUniform("ViewMatrix", V.data(), 4, GL_FALSE, 1);
            well_shader->setUniform("ProjectionMatrix", P.data(), 4, GL_FALSE, 1);

            glBindVertexArray(entities_.at("WellSegments").vertexArray_);

            glDrawArrays(GL_LINES, 0, entities_.at("WellSegments").number_of_elements_);

            glBindVertexArray(0);

            //std::cout << " Vertex Array " << entities_.at("WellSegment").vertexArray_ << "  " << entities_.at("WellLines").number_of_elements_ << std::endl;

            well_shader->unbind();
        }

    }


}

void Mesh::clear()
{
    mesh_type = TYPE::TRIANGLES;

    max[ 0 ] = 0.0f;
    max[ 1 ] = 0.0f;
    max[ 2 ] = 0.0f;

    min[ 0 ] = 0.0f;
    min[ 1 ] = 0.0f;
    min[ 2 ] = 0.0f;


    show_vertices = true;
    show_edges = false;
    show_faces = true;
    show_bbox = false;

    apply_crosssection_clipping = false;

    coefACrossSectionEquation = 0.0f;
    coefBCrossSectionEquation = 0.0f;
    coefCCrossSectionEquation = 0.0f;
    coefDCrossSectionEquation = 0.0f;
}

void Mesh::resetBuffers()
{
    deleteShaders();

    /// OpenVolume Mesh Integration ---------------------------------------------->
    // Surface Mesh

    for (auto& gl : entities_)
    {
        if (gl.second.vertexArray_ != 0)
        {

            if (gl.second.vertexBuffer_vertices_ != 0)
            {
                glDeleteBuffers(1, &gl.second.vertexBuffer_vertices_);
            }

            if (gl.second.vertexBuffer_normals_ != 0)
            {
                glDeleteBuffers(1, &gl.second.vertexBuffer_normals_);
            }

            if (gl.second.vertexBuffer_colors_ != 0)
            {
                glDeleteBuffers(1, &gl.second.vertexBuffer_colors_);
            }

            if (gl.second.vertexBuffer_elemets_ != 0)
            {
                glDeleteBuffers(1, &gl.second.vertexBuffer_elemets_);
            }

            glDeleteVertexArrays(1, &gl.second.vertexArray_);
        }

        gl.second.vertexArray_ = 0;
        gl.second.vertexBuffer_vertices_ = 0;
        gl.second.vertexBuffer_normals_ = 0;
        gl.second.vertexBuffer_colors_ = 0;
        gl.second.vertexBuffer_elemets_ = 0;
        gl.second.number_of_elements_ = 0;
    }

}

void Mesh::deleteShaders()
{

	if (shader_mesh != nullptr)
    {
        delete (shader_mesh);
        shader_mesh = nullptr;
    }
	if (shader_mesh_cornerpoint != nullptr)
    {
        delete (shader_mesh_cornerpoint);
        shader_mesh = nullptr;
    }

	if (shader_bbox != nullptr)
    {
        delete(shader_bbox);
        shader_bbox = nullptr;
    }

	if (well_shader != nullptr)
    {
        delete(well_shader);
        well_shader = nullptr;
    }

}
/// OpenVolume Mesh Integration ---------------------------------------------->

void Mesh::setDefaultColor()
{
    std::vector<float> _colors(entities_.at("Tetrahedron").number_of_elements_, 0.5F);
    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Tetrahedron").vertexBuffer_colors_);
    glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(_colors[0]), _colors.data(), GL_STATIC_DRAW);
}

void Mesh::setMeshColor(const std::vector<float>& _colors)
{
    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Tetrahedron").vertexBuffer_colors_);
    glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(_colors[0]), _colors.data(), GL_STATIC_DRAW);
}

void Mesh::setTetrahedronGeometry(const std::vector < unsigned int >& _faces_array, const std::vector<float>& _vertices_array)
{
    std::vector<float> colors_array_ = std::vector<float>(_vertices_array.size(), 0.5f);

    entities_.at("Tetrahedron").number_of_elements_ = _vertices_array.size();

    /// Load by Arrays
    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Tetrahedron").vertexBuffer_vertices_);
    glBufferData(GL_ARRAY_BUFFER, _vertices_array.size() * sizeof(_vertices_array[0]), _vertices_array.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Tetrahedron").vertexBuffer_normals_);
    glBufferData(GL_ARRAY_BUFFER, _vertices_array.size() * sizeof(_vertices_array[0]), _vertices_array.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Tetrahedron").vertexBuffer_colors_);
    glBufferData(GL_ARRAY_BUFFER, colors_array_.size() * sizeof(colors_array_[0]), colors_array_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entities_.at("Tetrahedron").vertexBuffer_elemets_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _faces_array.size() * sizeof(_faces_array[0]), _faces_array.data(), GL_STATIC_DRAW);

}

void Mesh::setHexahedronGeometry(const std::vector < unsigned int >& _faces_array, const std::vector<float>& _vertices_array)
{

    std::vector<float> colors_array_ = std::vector<float>(_vertices_array.size(), 0.5f);

    entities_.at("Hexahedron").number_of_elements_ = _vertices_array.size();

    /// Load by Arrays
    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Hexahedron").vertexBuffer_vertices_);
    glBufferData(GL_ARRAY_BUFFER, _vertices_array.size() * sizeof(_vertices_array[0]), _vertices_array.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Hexahedron").vertexBuffer_normals_);
    glBufferData(GL_ARRAY_BUFFER, _vertices_array.size() * sizeof(_vertices_array[0]), _vertices_array.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Hexahedron").vertexBuffer_colors_);
    glBufferData(GL_ARRAY_BUFFER, colors_array_.size() * sizeof(colors_array_[0]), colors_array_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entities_.at("Hexahedron").vertexBuffer_elemets_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _faces_array.size() * sizeof(_faces_array[0]), _faces_array.data(), GL_STATIC_DRAW);
}

void Mesh::buildBoundingBox(const std::vector<float>& _vertices)
{

    std::vector< Eigen::Vector3f > normalized_vertices;

    for (unsigned int it = 0; it < _vertices.size(); it+=3)
    {
        normalized_vertices.push_back(Eigen::Vector3f(_vertices[it + 0], _vertices[it + 1], _vertices[it + 2]));
    }

    boundingbox_.fromPointCloud(normalized_vertices.begin(), normalized_vertices.end());

    std::cout << (boundingbox_.Min().z()) + (boundingbox_.Max().z() - boundingbox_.Min().z())*0.5f << std::endl;

    this->depth_ = (boundingbox_.Min().y()) + ((boundingbox_.Max().y() - boundingbox_.Min().y())*0.5f);


    for (unsigned int it = 0; it < normalized_vertices.size(); ++it)
    {
        normalized_vertices[it] = (normalized_vertices[it] - boundingbox_.center()) / (boundingbox_.diagonal()*0.25f);
    }

    boundingbox_.fromPointCloud(normalized_vertices.begin(), normalized_vertices.end());

    // Cube /////////////////////////////////////////////////////////////////////////////////////
    //    v2----- v1
    //   /|      /|
    //  v3------v0|
    //  | |     | |
    //  | |v6---|-|v7
    //  |/      |/
    //  v5------v4

    // Vertex coordinate array for glDrawArrays() =========================
    // Using Quad Shader Layout
    // Draw GL_LINES_ADJACENCY -> Triangle strip
    //   2*-->-*3
    //   | *    |
    //   |   *  |
    //   0*-->-*1

    std::vector<Eigen::Vector3f> v =
    {
        /// Top Face
        Eigen::Vector3f(boundingbox_.Max().x(), boundingbox_.Max().y(), boundingbox_.Max().z()), // v0
        Eigen::Vector3f(boundingbox_.Max().x(), boundingbox_.Max().y(), boundingbox_.Min().z()), // v1
        Eigen::Vector3f(boundingbox_.Min().x(), boundingbox_.Max().y(), boundingbox_.Max().z()), // v3
        Eigen::Vector3f(boundingbox_.Min().x(), boundingbox_.Max().y(), boundingbox_.Min().z()), // v2


        /// Bottom Face
        Eigen::Vector3f(boundingbox_.Max().x(), boundingbox_.Min().y(), boundingbox_.Max().z()), // v4
        Eigen::Vector3f(boundingbox_.Min().x(), boundingbox_.Min().y(), boundingbox_.Max().z()), // v5
        Eigen::Vector3f(boundingbox_.Max().x(), boundingbox_.Min().y(), boundingbox_.Min().z()), // v7
        Eigen::Vector3f(boundingbox_.Min().x(), boundingbox_.Min().y(), boundingbox_.Min().z()), // v6


        /// Front Face
        Eigen::Vector3f(boundingbox_.Max().x(), boundingbox_.Min().y(), boundingbox_.Max().z()), // v4
        Eigen::Vector3f(boundingbox_.Max().x(), boundingbox_.Max().y(), boundingbox_.Max().z()), // v0
        Eigen::Vector3f(boundingbox_.Min().x(), boundingbox_.Min().y(), boundingbox_.Max().z()), // v5
        Eigen::Vector3f(boundingbox_.Min().x(), boundingbox_.Max().y(), boundingbox_.Max().z()), // v3

        /// Back Face
        Eigen::Vector3f(boundingbox_.Max().x(), boundingbox_.Min().y(), boundingbox_.Min().z()), // v7
        Eigen::Vector3f(boundingbox_.Max().x(), boundingbox_.Max().y(), boundingbox_.Min().z()), // v1
        Eigen::Vector3f(boundingbox_.Min().x(), boundingbox_.Min().y(), boundingbox_.Min().z()), // v6
        Eigen::Vector3f(boundingbox_.Min().x(), boundingbox_.Max().y(), boundingbox_.Min().z()), // v2

        /// Right Face
        Eigen::Vector3f(boundingbox_.Max().x(), boundingbox_.Min().y(), boundingbox_.Max().z()), // v4
        Eigen::Vector3f(boundingbox_.Max().x(), boundingbox_.Min().y(), boundingbox_.Min().z()), // v7
        Eigen::Vector3f(boundingbox_.Max().x(), boundingbox_.Max().y(), boundingbox_.Max().z()), // v0
        Eigen::Vector3f(boundingbox_.Max().x(), boundingbox_.Max().y(), boundingbox_.Min().z()), // v1

        /// Left Face
        Eigen::Vector3f(boundingbox_.Min().x(), boundingbox_.Min().y(), boundingbox_.Max().z()), // v5
        Eigen::Vector3f(boundingbox_.Min().x(), boundingbox_.Max().y(), boundingbox_.Max().z()), // v3
        Eigen::Vector3f(boundingbox_.Min().x(), boundingbox_.Min().y(), boundingbox_.Min().z()), // v6
        Eigen::Vector3f(boundingbox_.Min().x(), boundingbox_.Max().y(), boundingbox_.Min().z()), // v2

    };


    entities_.at("BoundingBox").number_of_elements_ = v.size();
    /// Load by Arrays
    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("BoundingBox").vertexBuffer_vertices_);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(Eigen::Vector3f),v.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("BoundingBox").vertexBuffer_normals_);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(Eigen::Vector3f), v.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("BoundingBox").vertexBuffer_colors_);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(Eigen::Vector3f), v.data(), GL_STATIC_DRAW);

}

void Mesh::getBoundingBox(float& xm, float& xM, float& ym, float& yM, float& zm, float& zM)
{
    xm = boundingbox_.Min().x();
    ym = boundingbox_.Min().y();
    zm = boundingbox_.Min().z();

    xM = boundingbox_.Max().x();
    yM = boundingbox_.Max().y();
    zM = boundingbox_.Max().z();
}

void Mesh::setSkeletonGeometry(const std::vector < unsigned int >& _faces_array, const std::vector<float>& _vertices_array)
{
    std::vector< Eigen::Vector3f > normalized_vertices;

    for (unsigned int it = 0; it < _vertices_array.size(); it += 3)
    {
        normalized_vertices.push_back(Eigen::Vector3f(_vertices_array[it + 0], _vertices_array[it + 1], _vertices_array[it + 2]));
    }

    Tucano::BoundingBox3<float > bb;

    bb.fromPointCloud(normalized_vertices.begin(), normalized_vertices.end());


    double scale = 1.0 / (bb.diagonal()*0.25);

    Eigen::Affine3f t;
    Eigen::Affine3f s;

    t.setIdentity();
    s.setIdentity();

    t.translation() = -bb.center();
    s.scale(Eigen::Vector3f(scale, scale, scale));

    this->model_matrix_.setIdentity();
    this->model_matrix_ = s * t;

    std::cout << "(bbox_mesh) on MESH" << std::endl;
    std::cout << (bb.Max().y()) - (bb.Min().y()) << std::endl;
    std::cout << (bb.Max().x()) - (bb.Min().x()) << std::endl;
    std::cout << (bb.Max().z()) - (bb.Min().z()) << std::endl;

    dim_max.x() = (bb.Max().x());
    dim_max.y() = (bb.Max().y());
    dim_max.z() = (bb.Max().z());


    dim_min.x() = (bb.Min().x());
    dim_min.y() = (bb.Min().y());
    dim_min.z() = (bb.Min().z());

    std::cout << "(bbox_mesh) on Max/Min" << std::endl;
    std::cout << bb.Max() << std::endl;
    std::cout << bb.Min() << std::endl;
    std::cout << bb.center() << std::endl;



    for (unsigned int it = 0; it < normalized_vertices.size(); ++it)
    {
        normalized_vertices[it] = this->model_matrix_ * normalized_vertices[it];
    }


    std::vector< Eigen::Vector3f > vertices_coord;

    std::vector< unsigned int > triangles_id;;
    std::size_t faces_number = _faces_array.size() / 4;

    for (std::size_t i = 0; i < faces_number; ++i)
    {

        triangles_id.clear();
        triangles_id.push_back(_faces_array[4 * i]);
        triangles_id.push_back(_faces_array[4 * i + 1]);
        triangles_id.push_back(_faces_array[4 * i + 3]);
        triangles_id.push_back(_faces_array[4 * i + 2]);

        for (std::size_t index = 0; index < triangles_id.size(); index++)
        {
            vertices_coord.push_back(normalized_vertices[triangles_id[index]]);

        }
    }

    std::vector< Eigen::Vector3f > color(vertices_coord.size(), Eigen::Vector3f(0.5, 0.5, 0.5));


    entities_.at("Sketchies").number_of_elements_ = vertices_coord.size();

    /// Load by Arrays
    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Sketchies").vertexBuffer_vertices_);
    glBufferData(GL_ARRAY_BUFFER, vertices_coord.size() * sizeof(Eigen::Vector3f), vertices_coord.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Sketchies").vertexBuffer_normals_);
    glBufferData(GL_ARRAY_BUFFER, vertices_coord.size() * sizeof(Eigen::Vector3f), vertices_coord.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Sketchies").vertexBuffer_colors_);
    glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(Eigen::Vector3f), color.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entities_.at("Sketchies").vertexBuffer_elemets_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _faces_array.size() * sizeof(_faces_array[0]), _faces_array.data(), GL_STATIC_DRAW);

}



void Mesh::setTriangleSkeletonGeometry(const std::vector < unsigned int >& _faces_array, const std::vector<float>& _vertices_array)
{
    std::vector< Eigen::Vector3f > normalized_vertices;

    for (unsigned int it = 0; it < _vertices_array.size(); it += 3)
    {
        normalized_vertices.push_back(Eigen::Vector3f(_vertices_array[it + 0], _vertices_array[it + 1], _vertices_array[it + 2]));
    }

    Tucano::BoundingBox3<float > bb;

    bb.fromPointCloud(normalized_vertices.begin(), normalized_vertices.end());


    double scale = 1.0 / (bb.diagonal()*0.25);

    Eigen::Affine3f t;
    Eigen::Affine3f s;

    t.setIdentity();
    s.setIdentity();

    t.translation() = -bb.center();
    s.scale(Eigen::Vector3f(scale, scale, scale));

    this->model_matrix_.setIdentity();
    this->model_matrix_ = s * t;

    std::cout << "(bbox_mesh) on MESH" << std::endl;
    std::cout << (bb.Max().y()) - (bb.Min().y()) << std::endl;
    std::cout << (bb.Max().x()) - (bb.Min().x()) << std::endl;
    std::cout << (bb.Max().z()) - (bb.Min().z()) << std::endl;

    dim_max.x() = (bb.Max().x());
    dim_max.y() = (bb.Max().y());
    dim_max.z() = (bb.Max().z());


    dim_min.x() = (bb.Min().x());
    dim_min.y() = (bb.Min().y());
    dim_min.z() = (bb.Min().z());

    std::cout << "(bbox_mesh) on Max/Min" << std::endl;
    std::cout << bb.Max() << std::endl;
    std::cout << bb.Min() << std::endl;
    std::cout << bb.center() << std::endl;



    for (unsigned int it = 0; it < normalized_vertices.size(); ++it)
    {
        normalized_vertices[it] = this->model_matrix_ * normalized_vertices[it];
    }


    std::vector< Eigen::Vector3f > vertices_coord;

    std::vector< unsigned int > triangles_id;;
    std::size_t faces_number = _faces_array.size() / 3;

    for (std::size_t i = 0; i < faces_number; ++i)
    {

        triangles_id.clear();
        triangles_id.push_back(_faces_array[3 * i]);
        triangles_id.push_back(_faces_array[3 * i + 1]);
        triangles_id.push_back(_faces_array[3 * i + 2]);

        for (std::size_t index = 0; index < triangles_id.size(); index++)
        {
            vertices_coord.push_back(normalized_vertices[triangles_id[index]]);
        }
    }

    std::vector< Eigen::Vector3f > color(vertices_coord.size(), Eigen::Vector3f(0.5, 0.5, 0.5));


    entities_.at("Sketchies").number_of_elements_ = vertices_coord.size();
    number_of_faces = _faces_array.size();

    /// Load by Arrays
    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Sketchies").vertexBuffer_vertices_);
    glBufferData(GL_ARRAY_BUFFER, normalized_vertices.size() * sizeof(Eigen::Vector3f), normalized_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Sketchies").vertexBuffer_normals_);
    glBufferData(GL_ARRAY_BUFFER, normalized_vertices.size() * sizeof(Eigen::Vector3f), normalized_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("Sketchies").vertexBuffer_colors_);
    glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(Eigen::Vector3f), color.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entities_.at("Sketchies").vertexBuffer_elemets_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _faces_array.size() * sizeof(_faces_array[0]), _faces_array.data(), GL_STATIC_DRAW);

}



/// @TODO Later, move this fucntions to FlowvizualizationController
void Mesh::loadWellPosition(int _number_of_wells, const std::map<int, Eigen::Vector4d>& _vertices_array, const std::map<int, int>& _types, const std::map< int, Eigen::Vector2d >& _range)
{

    /// Load by Arrays

    std::cout << "Range X " << _range.at(0).x() << std::endl;
    std::cout << "Range Y " << _range.at(0).y() << std::endl;

    std::vector<Eigen::Vector3f> wells_tip;
    std::vector<Eigen::Vector3f> wells_tip_colors;

    std::vector<Eigen::Vector3f> wells_base;
    std::vector<Eigen::Vector3f> wells_base_colors;

    std::vector<Eigen::Vector3f> wells_segments;
    std::vector<Eigen::Vector3f> wells_segments_color;


    Eigen::Vector3f v;
    Eigen::Vector3f well_direction(0.0f,0.0f,-1.0f);
    float well_size = ((model_matrix_ * Eigen::Vector4f(0.0f, 0.0, dim_max.z(), 1.0f)).head<3>()).z() - ((model_matrix_ * Eigen::Vector4f(0.0f, 0.0f, dim_min.z(), 1.0f)).head<3>()).z();

    const Eigen::Vector3f injector(0.0f,0.0f,1.0f);
    const Eigen::Vector3f producer(1.0f, 0.0f, 0.0f);
    const Eigen::Vector3f undefined(0.5f, 0.5f, 0.5f);

    for (auto p : _vertices_array )
    {

        if (p.first >= _number_of_wells)
        {
            break;
        }

        //float x = dim_min.x() + (dim_max.x() - dim_min.x()) * (p.second.x() / (dim_max.x() - dim_min.x()));
        //float y = dim_min.y() + (dim_max.y() - dim_min.y()) * (p.second.y() / (dim_max.y() - dim_min.y()) );
        //float z = dim_max.z() + (dim_max.z() - dim_min.z()) * (p.second.z() / 100.0f);

        v = (model_matrix_ * Eigen::Vector4f(p.second.x(), p.second.y(), p.second.z(), 1.0f)).head<3>();

        wells_tip.push_back(v);

        if (_types.at(p.first) == -1 ) // Injector
        {
            wells_tip_colors.push_back(producer);

            wells_base_colors.push_back(undefined);
            wells_base_colors.push_back(undefined);
            wells_base_colors.push_back(undefined);
            wells_base_colors.push_back(undefined);

            wells_segments_color.push_back(producer);
            wells_segments_color.push_back(producer);
        }
        else if (_types.at(p.first) == 1) // producer
        {
            wells_tip_colors.push_back(injector);

            wells_base_colors.push_back(undefined);
            wells_base_colors.push_back(undefined);
            wells_base_colors.push_back(undefined);
            wells_base_colors.push_back(undefined);

            wells_segments_color.push_back(injector);
            wells_segments_color.push_back(injector);

        }else /// error
        {
            wells_tip_colors.push_back(undefined);

            wells_base_colors.push_back(undefined);
            wells_base_colors.push_back(undefined);
            wells_base_colors.push_back(undefined);
            wells_base_colors.push_back(undefined);

            wells_segments_color.push_back(undefined);
            wells_segments_color.push_back(undefined);
        }

        //std::cout << well_direction * well_size * (p.second.w() / 100.0f) << std::endl;

        wells_base.push_back(v);
        wells_base.push_back(v + well_direction * well_size * (_range.at(p.first).y() / 100.0f));

        wells_base.push_back(v + well_direction * well_size * (_range.at(p.first).x() / 100.0f));
        wells_base.push_back(v + well_direction * well_size * (100.0f / 100.0f));

        wells_segments.push_back(v + well_direction * well_size * (_range.at(p.first).x() / 100.0f));
        wells_segments.push_back(v + well_direction * well_size * (_range.at(p.first).y() / 100.0f));

        std::cout << "_range.at(p.first).x() " << _range.at(p.first).x() << std::endl;
        std::cout << "_range.at(p.first).y() " << _range.at(p.first).y() << std::endl;

    }

    entities_.at("WellPoints").number_of_elements_ = wells_tip.size();
    entities_.at("WellLines").number_of_elements_ = wells_base.size();
    entities_.at("WellSegments").number_of_elements_ = wells_segments.size();

    //v[0] = (model_matrix_ * Eigen::Vector4f(dim_max.x(), dim_max.y(), dim_max.z(), 1.0f)).head<3>();
    //v[1] = (model_matrix_ * Eigen::Vector4f(dim_min.x(), dim_min.y(), dim_min.z(), 1.0f)).head<3>();

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("WellPoints").vertexBuffer_vertices_);
    glBufferData(GL_ARRAY_BUFFER, wells_tip.size() * sizeof(Eigen::Vector3f), wells_tip.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("WellPoints").vertexBuffer_colors_);
    glBufferData(GL_ARRAY_BUFFER, wells_tip_colors.size() * sizeof(Eigen::Vector3f), wells_tip_colors.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("WellLines").vertexBuffer_vertices_);
    glBufferData(GL_ARRAY_BUFFER, wells_base.size() * sizeof(Eigen::Vector3f), wells_base.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("WellLines").vertexBuffer_colors_);
    glBufferData(GL_ARRAY_BUFFER, wells_base_colors.size() * sizeof(Eigen::Vector3f), wells_base_colors.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("WellSegments").vertexBuffer_vertices_);
    glBufferData(GL_ARRAY_BUFFER, wells_segments.size() * sizeof(Eigen::Vector3f), wells_segments.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, entities_.at("WellSegments").vertexBuffer_colors_);
    glBufferData(GL_ARRAY_BUFFER, wells_segments_color.size() * sizeof(Eigen::Vector3f), wells_segments_color.data(), GL_STATIC_DRAW);

}


Eigen::Affine3f Mesh::getModelMatrix() const
{
    return this->model_matrix_;
}
