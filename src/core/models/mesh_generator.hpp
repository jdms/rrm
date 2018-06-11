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


#ifndef _MESHGENERATOR_HPP_
#define _MESHGENERATOR_HPP_

// Include vector classes
#include "openvolumemesh_defines.hpp"

// std headers
#include <tuple>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>


	class MeshGenerator
	{
	public:

		MeshGenerator(OpenVolumeMesh::TetrahedralMeshV3d& _mesh) : mesh_(_mesh)
		{
			this->face_map_.clear();
			this->vertices_.clear();
			this->mesh_.clear();
		}

		~MeshGenerator() = default;


		void addVertex(const OpenVolumeMesh::Vec3d& vertex);

		void addCell(std::vector<unsigned int>& _idx);

	private:

		void rotate2Small(OpenVolumeMesh::FaceTuple& _face_tuple);
		OpenVolumeMesh::Vec3d centroid(const std::vector<unsigned int>& _idx);

		OpenVolumeMesh::FaceMap face_map_;
		std::vector<OpenVolumeMesh::VertexHandle> vertices_;

		OpenVolumeMesh::TetrahedralMeshV3d& mesh_;
	};


#endif /* _MESHGENERATOR_HPP_ */
