
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


#include "openvolumemesh_defines.hpp"


namespace OpenVolumeMesh
{


	/// Vector3D Data Type
	std::ostream& operator<<(std::ostream& os, const Vector3D& _other_vector3d)
	{
		
		return os;
	}

	std::istream& operator>>(std::istream& is, Vector3D& _other_vector3d)
	{
		
		return is;
	}

	Vector3D::Vector3D() : Base(Identity())
	{

	}

	Vector3D::Vector3D(const double& x, const double& y, const double& z) : Base(x, y, z)
	{

	}

	Vector3D::Vector3D(const Vector3D& _other_vector3d) : Base(_other_vector3d)
	{

	}

	Vector3D::Vector3D(const Base& v) : Base(v)
	{

	}


	std::ostream& operator<<(std::ostream& os, const Matrix3D& _other_matrix3d)
	{
		return os;
	}
	std::istream& operator>>(std::istream& is, Matrix3D& _other_matrix3d)
	{
		return is;
	}

	/// Matrix 3D data type
	Matrix3D::Matrix3D() : Base(Identity())
	{

	}
	//@see Base class construction http://stackoverflow.com/questions/8773048/why-arent-copy-constructors-chained-like-default-constructors-or-destructors
	Matrix3D::Matrix3D(const Matrix3D& _other_matrix3d) : Base(_other_matrix3d)
	{

	}

	Matrix3D::Matrix3D(const Base& _other_matrix3d) : Base(_other_matrix3d)
	{

	}

	template <> const std::string typeName<Vector3D>() { return "Vector3D"; };
	template <> const std::string typeName<Matrix3D>() { return "Matrix3D"; };
}



