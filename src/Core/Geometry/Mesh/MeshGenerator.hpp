/*
 * MeshGenerator.hpp
 *
 *  Created on: Feb 21, 2017
 *      Author: felipe
 */

#ifndef _MESHGENERATOR_HPP_
#define _MESHGENERATOR_HPP_

// Include vector classes
#include "OpenVolumeMeshDefines.hpp"

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
