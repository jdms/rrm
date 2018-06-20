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



#include "flow_model.hpp"

namespace RRM
{

    FlowModel::FlowModel()
    {        
        model_matrix_.setIdentity();
        bounding_box_.reset();
    }
  
    /// Update Tetrahedon Mesh Colours
    void FlowModel::updateTetrahedronColors(const std::string& _property_name, const std::string& _entity_name, const std::string& _dimension, std::vector< double >& _values)
    {

        //if (this->ptr_tetrahedron_mesh_)
        //{
        //    if (_entity_name.compare("VProp") == 0)
        //    {
        //        if (_dimension.compare("double") == 0)
        //        {
        //            updateTetrahedronVertexScalarProperty(_property_name, _values);
        //        }
        //        else if (_dimension.compare("Vector3D") == 0)
        //        {
        //            std::cout << "Tetrahedron Mesh dimension not implemented " << _property_name << std::endl;
        //        }
        //        else
        //        {
        //            std::cout << "Tetrahedron Mesh dimension not supported " << _property_name << std::endl;
        //        }
        //    }
        //    else  if (_entity_name.compare("CProp") == 0)
        //    {
        //        if (_dimension.compare("double") == 0)
        //        {
        //            updateTetrahedronCellScalarProperty(_property_name, _values);
        //        }
        //        else if (_dimension.compare("Vector3D") == 0)
        //        {
        //            std::cout << "Tetrahedron Mesh dimension not implemented " << _property_name << std::endl;
        //        }
        //        else
        //        {
        //            std::cout << "Tetrahedron Mesh dimension not supported " << _property_name << std::endl;
        //        }
        //    }
        //    else
        //    {
        //        std::cout << "Tetrahedron Mesh entity not supported " << _property_name << std::endl;
        //    }

        //}
        //else
        //{
        //    std::cout << "Tetrahedron Mesh not valid " << _property_name << std::endl;
        //}
    }

    /// @TODO Later, move this fucntions to FlowvizualizationController
    Eigen::Affine3d FlowModel::getModelMatrix() const
    {
        return this->model_matrix_;
    }

	void FlowModel::uploadTetrahedron(const std::vector<float>&         _vertices,       /// Lisf of all vertices of the tetrahedon mesh
		                              const std::vector<unsigned int>&  _cells,			 /// List of all cells of the tetrahedron mesh. List of cells as a vertice list:
																						 /// Each four elements as indices to the list of vertices
								      std::vector<float>&               vertices_array_, /// To OpenGL buffers
									  std::vector < unsigned int >&     faces_array_	 /// To OpenGL buffers
									  )  
	{

		/// The 3D Tetrahedron Mesh come from Tetget.

		// @see  TETGEN DOCUMENTATION at http://wias-berlin.de/software/tetgen/1.5/doc/manual/manual006.html
		// 5.2.4  .ele files


		// An.ele file contains a list of tetrahedra.

		//	First line : <# of tetrahedra> <nodes per tet. (4 or 10)>
		//	<region attribute(0 or 1)>
		//	Remaining lines list # of tetrahedra :
		//  <tetrahedron #> <node> <node> ... <node>[attribute]
		//	...
		//	Each tetrahedron has four corners(or ten corners if the - o2 switch is used).
		//  Nodes are indices into the corresponding.node file.
		//  The first four nodes are the corner vertices.If - o2 switch is used, the remaining six nodes are generated on the midpoints of the edges of the tetrahedron.
		//  Figure 20 shows how these corners and the second - order nodes are locally numbered.Second order nodes are output only.They are omitted by the mesh reconstruction(the - r switch).
		//
		//                       3
		//                       /\
		//                      /  \
		//                     /    \
		//                    /      \
		//                   /        \
		//                0 /__________\ 1
		//                 / \        / \
		//                /   \      /   \
		//               /     \    /     \
		//              /       \  /       \
		//           3 /_________\/_________\ 3
		//            *          2           *

		/// From Flow Diagnostic Interface

		/// Each Cell has four vertices, {v0, v1, v2, v3 } and 4 Faces.
		/// Each Face has 3 vertice as listed bellow:
		/// [v0,v1,v2], 
		//  [v0,v2,v3], 
		/// [v2,v1,v3], 
		/// [v0,v3,v1]
		/// The vertices geometry are:
		/// [v0.x,v0.y,v0.z; v1.x,v1.y,v1.z; v2.x,v2.y,v2.z]  9 floats
		/// [v0.x,v0.y,v0.z; v2.x,v2.y,v2.z; v3.x,v3.y,v3.z]  9 floats
		/// [v2.x,v2.y,v2.z; v1.x,v1.y,v1.z; v3.x,v3.y,v3.z]  9 floats
		/// [v0.x,v0.y,v0.z; v3.x,v3.y,v3.z; v1.x,v1.y,v1.z]  9 floats


		/// Construct a bounding box from the geomtry and use it to
		/// bring the model origin to the (0,0,0) and scale to 0-1.
		/// @TODO  transform this procedure into a function
		std::vector< Eigen::Vector4d > normalized_vertices;

		bounding_box_.reset();

		for (unsigned int it = 0; it < _vertices.size(); it += 3)
		{
			normalized_vertices.push_back(Eigen::Vector4d(_vertices[it + 0], _vertices[it + 1], _vertices[it + 2], 1.0));
		}

		bounding_box_.fromPointCloud(normalized_vertices.begin(), normalized_vertices.end());

		double scale = 1.0 / (bounding_box_.diagonal()*0.25);

		Eigen::Affine3d t;
		Eigen::Affine3d s;

		t.setIdentity();
		s.setIdentity();

		t.translation() = -bounding_box_.center();
		s.scale(Eigen::Vector3d(scale, scale, scale));

		model_matrix_ = s * t;

		for (unsigned int it = 0; it < normalized_vertices.size(); ++it)
		{
			normalized_vertices[it] = model_matrix_ * normalized_vertices[it];
		}

		//bounding_box_.fromPointCloud(normalized_vertices.begin(), normalized_vertices.end());

		/// Swap back the transformed vertices
		std::vector<float> normalized;
		normalized.resize(_vertices.size());
		for (std::size_t it = 0; it < normalized_vertices.size(); it++)
		{
			normalized[3 * it + 0] = normalized_vertices[it].x();
			normalized[3 * it + 1] = normalized_vertices[it].y();
			normalized[3 * it + 2] = normalized_vertices[it].z();
		}


		std::size_t  number_of_cells = static_cast<int>( _cells.size() / 4.0f);

		faces_array_.clear();
		/// The number of faces  = 4 * cells
		//faces_array_.resize(number_of_cells * 12);
		/// The number of veritces  =  12 * number of cells 
		vertices_array_.clear();
		//vertices_array_.resize(number_of_cells * 36);

		std::size_t face_index = 0;

		/// Looping over the cells. Each cell 4 faces: Eeach Face 3 vertices
		for (std::size_t index = 0; index < _cells.size(); index += 4)
		{
			/// [v0,v1,v2] = 9 floats			
			/// Face 1			
			// v0
			vertices_array_.push_back(normalized_vertices[_cells[index+0]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index+0]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index+0]].z());
			faces_array_.push_back(index + 0);
			
			// v1
			vertices_array_.push_back(normalized_vertices[_cells[index+1]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index+1]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index+1]].z());
			faces_array_.push_back(index + 1);
			
			// v2
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].z());
			faces_array_.push_back(index + 2);

			/// [v0,v2,v3] 
			/// Face 2			
			// v0
			vertices_array_.push_back(normalized_vertices[_cells[index + 0]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 0]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 0]].z());
			faces_array_.push_back(index + 0);

			// v2
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].z());
			faces_array_.push_back(index + 2);

			// v3
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].z());
			faces_array_.push_back(index + 3);

			/// [v2,v1,v3] 
			/// Face 3
			// v2
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 2]].z());
			faces_array_.push_back(index + 2);

			// v1
			vertices_array_.push_back(normalized_vertices[_cells[index + 1]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 1]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 1]].z());
			faces_array_.push_back(index + 1);

			// v2
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].z());
			faces_array_.push_back(index + 3);

			/// [v0,v3,v1]
			/// Face 4			
			// v0
			vertices_array_.push_back(normalized_vertices[_cells[index + 0]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 0]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 0]].z());
			faces_array_.push_back(index + 0);

			// v1
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 3]].z());
			faces_array_.push_back(index + 3);

			// v2
			vertices_array_.push_back(normalized_vertices[_cells[index + 1]].x());
			vertices_array_.push_back(normalized_vertices[_cells[index + 1]].y());
			vertices_array_.push_back(normalized_vertices[_cells[index + 1]].z());
			faces_array_.push_back(index + 1);
		}

	}


} /* namespace RRM */



