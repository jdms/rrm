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


#ifndef _FLOWMODEL_HPP_
#define _FLOWMODEL_HPP_

#include <Eigen/Dense>
#include <vector>


#include "Tucano/BoundingBox3.hpp"
#include "./libs/Tucano/BoundingBox3.hpp"
#include "property_profile.hpp"



namespace RRM
{

    class FlowModel
    {
        public:
          

            FlowModel();
            ~FlowModel() = default ;

            /// OpenVolumeMesh Integration
			void updateGeometry(std::vector<float>& vertices_array_, std::vector < unsigned int >& faces_array);

            void updateTetrahedronColors(const std::string& _property_name, const std::string& _entity_name, const std::string& _dimension, std::vector< double >& _values);

            /// @TODO Later, move this fucntions to FlowvizualizationController
            Eigen::Affine3d getModelMatrix() const;

			////@FIXEME June 2018 A short solution to avoid constructing the mesh from scratch.
			void FlowModel::uploadTetrahedron(const std::vector<float>&         _vertices,       /// Lisf of all vertices of the tetrahedon mesh
										      const std::vector<unsigned int>&  _cells,			 /// List of all cells of the tetrahedron mesh. List of cells as a vertice list:
																								 /// Each four elements as indices to the list of vertices
											  std::vector<float>&               vertices_array_, /// To OpenGL buffers
											  std::vector < unsigned int >&     faces_array_	 /// To OpenGL buffers
				);


    private:
        
		std::vector<RRM::PropertyProfile> vertex_properties_;
		std::vector<RRM::PropertyProfile> cell_properties_;

        /// @TODO Later, move this fucntions to FlowvizualizationController
        /// Bring model from Model coodinates to OpenGL coordiates
        Eigen::Affine3d model_matrix_;
        /// BoundingBox in OpenGL coordiates
        Tucano::BoundingBox3<double> bounding_box_;

    };

} /* namespace RRM */

#endif /* _FLOWMODEL_HPP_ */
