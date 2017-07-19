#include "OpenVolumeMeshDefines.hpp"


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



