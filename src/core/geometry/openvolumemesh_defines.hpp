
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


/*
 * OpenVolumeMeshDefines.hpp
 *
 *  Created on: Feb 23, 2017
 *      Author: felipe
 */

#ifndef _RRM_OPENVOLUMEMESHDEFINES_HPP_
#define _RRM_OPENVOLUMEMESHDEFINES_HPP_

// Include vector classes
#include <OpenVolumeMesh/Geometry/VectorT.hh>

// Include polyhedral mesh kernel
#include <OpenVolumeMesh/Mesh/PolyhedralMesh.hh>

// Include polyhedral mesh kernel
#include <OpenVolumeMesh/Mesh/TetrahedralMesh.hh>

//Include hexahedron mesh kernel
#include <OpenVolumeMesh/Mesh/HexahedralMesh.hh>

// Include properties per entity
#include <OpenVolumeMesh/core/PropertyDefines.hh>

// Eigen
#include <Eigen/Dense>

namespace OpenVolumeMesh
{
	using Vec3d = OpenVolumeMesh::Geometry::Vec3d;
	using BaseMesh3d = OpenVolumeMesh::GeometryKernel<Vec3d, OpenVolumeMesh::TopologyKernel>;
	using PolyhedralMeshV3d = OpenVolumeMesh::GeometricPolyhedralMeshV3d;
	using TetrahedralMeshV3d = OpenVolumeMesh::GeometricTetrahedralMeshV3d;
	using HexahedralMesh3d = OpenVolumeMesh::GeometricHexahedralMeshV3d;
	using FaceTuple = std::tuple<OpenVolumeMesh::VertexHandle, OpenVolumeMesh::VertexHandle, OpenVolumeMesh::VertexHandle>;
	using FaceMap = std::map<FaceTuple, OpenVolumeMesh::FaceHandle>;

	/// @see https://eigen.tuxfamily.org/dox/TopicCustomizing_InheritingMatrix.html
	class Vector3D : public Eigen::Vector3d
	{			
		typedef Eigen::Vector3d Base;
		using Base::base;
	public:
		Vector3D();
		Vector3D(const double&, const double&, const double&);
		Vector3D(const Vector3D&);
		Vector3D(const Base&);
		
		friend std::ostream& operator<<(std::ostream& os, const Vector3D& fp);
		friend std::istream& operator>>(std::istream& is, Vector3D& fp);
	};


	/// @see https://eigen.tuxfamily.org/dox/TopicCustomizing_InheritingMatrix.html
	class Matrix3D : public Eigen::Matrix3d
	{
		typedef Eigen::Matrix3d Base;
		using Base::base;
	public:
		Matrix3D();		
		Matrix3D(const Matrix3D&);
		Matrix3D(const Base&);

		friend std::ostream& operator<<(std::ostream& os, const Matrix3D& fp);
		friend std::istream& operator>>(std::istream& is, Matrix3D& fp);
	};

	template <class T>
	const std::string typeName();
	template <> const std::string typeName<Vector3D>();
	template <> const std::string typeName<Matrix3D>();
	
}





#endif /* OPENVOLUMEMESHDEFINES */
