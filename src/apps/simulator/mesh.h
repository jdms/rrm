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


#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iterator>
#include <string>

#include "Eigen/Dense"
#include "libs/Tucano/Trackball.hpp"
#include "libs/Tucano/Shader.hpp"
#include "libs/Tucano/BoundingBox3.hpp"


#include "colormap.h"

// @DEPRECATED 
// This class itself needs a huge refactory
class Mesh
{
    public:

        enum class TYPE{ TRIANGLES, QUADRILATERAL, TETRAHEDRAL, HEXAHEDRAL };

        struct OpenGLGeometry
        {
            OpenGLGeometry()
            {
                vertexArray_ = 0;
                vertexBuffer_vertices_ = 0;
                vertexBuffer_normals_ = 0;
                vertexBuffer_colors_ = 0;
                /// Draw the mesh as element array to aid filters implementationxzs
                vertexBuffer_elemets_ =  0;
                number_of_elements_ = 0;
            }
            // Surface Mesh
            GLuint vertexArray_;
                GLuint vertexBuffer_vertices_;
                GLuint vertexBuffer_normals_;
                GLuint vertexBuffer_colors_;
            /// Draw the mesh as element array to aid filters implementationxzs
                GLuint vertexBuffer_elemets_;
            GLuint number_of_elements_;
        };


        Mesh();

        inline Mesh( TYPE t ){ mesh_type = t; }
        void setMeshType( TYPE t );//{ mesh_type = t; }

        void setConstantColor( const float r, const float g, const float b );

        void setCrossSectionClippingEquation( const float& a, const float& b, const float& c, const float& d , Eigen::Vector3f _centre);
        void disableCrossSection();



        void showVertices( bool status );
        void showEdges( bool status );
        void showFaces( bool status );
        void showBoundingBox( bool status );


        bool showVertices() const;
        bool showEdges() const;
        bool showFaces() const;
        bool showBoundingBox() const;


        void buildBoundingBox(const std::vector<float>& _vertices);
        void getBoundingBox( float& xm, float& xM, float& ym, float& yM, float& zm, float& zM);


        void load();
        void draw(const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const float& scale, const float width, const float height);
        void initializeShader( std::string directory );

        void reloadShader();

        void clear();

        void resetBuffers();
        void deleteShaders();
        /// get the correct depth from the model in QGraphicsScene coordinates
        float getDepth() const;
        std::tuple<Eigen::Vector3f, Eigen::Vector3f>  getBoudingBoxDimension() const;

        /// OpenVolumeMesh Integration
        void setTetrahedronGeometry(const std::vector < unsigned int >& _faces_array, const std::vector<float>& _vertices_array);
        void setHexahedronGeometry(const std::vector < unsigned int >& _faces_array, const std::vector<float>& _vertices_array);
        void setSkeletonGeometry(const std::vector < unsigned int >& _faces_array, const std::vector<float>& _vertices_array);
        void setTriangleSkeletonGeometry(const std::vector < unsigned int >& _faces_array, const std::vector<float>& _vertices_array);

        void setDefaultColor();
        void setMeshColor(const std::vector<float>& _colors);

        /// @TODO Later, move this fucntions to FlowvizualizationController
        void loadWellPosition(int _number_of_wells, const std::map<int, Eigen::Vector4d>& _vertices_array, const std::map<int, int>& _types, const std::map< int, Eigen::Vector2d >& _range);
        Eigen::Affine3f getModelMatrix() const;

    protected:

        TYPE mesh_type;

        float max[ 3 ];
        float min[ 3 ];

        Tucano::Shader* shader_mesh;
        Tucano::Shader* shader_mesh_cornerpoint;
        Tucano::Shader* shader;
        Tucano::Shader* quad_phong_;

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

        bool show_bbox;

        /// get the correctly depth value
        float depth_;
        Eigen::Affine3f model_matrix_;

        Eigen::Vector3f dim_max;
        Eigen::Vector3f dim_min;


        ColorMap::COLORMAP current_colormap;
        ColorMap colormap;

        /// New Mesh
        Tucano::BoundingBox3<float> boundingbox_;
        // Surface Mesh
        Tucano::Shader* well_shader;

        std::map<std::string, OpenGLGeometry> entities_;

        std::size_t number_of_faces;
};

#endif // MESH_H
