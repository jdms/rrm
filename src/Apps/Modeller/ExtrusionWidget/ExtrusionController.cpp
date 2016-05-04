/*
 * ExtrusionController.cpp
 *
 *  Created on: Apr 29, 2016
 *      Author: felipe
 */

#include "ExtrusionController.hpp"

namespace RRM
{

	ExtrusionController::ExtrusionController ( )
	{
		// TODO Auto-generated constructor stub

	}

	ExtrusionController::~ExtrusionController ( )
	{
		// TODO Auto-generated destructor stub
	}

	bool  ExtrusionController:: initialize (  float _x_min,
						  float _y_min,
						  float _z_min,
						  float _x_max,
						  float _y_max,
						  float _z_max )
	{
		normalize_x_ = ( _x_max - _x_min);
		normalize_y_ = ( _y_max - _y_min);
		normalize_z_ = ( _z_max - _z_min);

		if ( normalize_x_ > normalize_y_ )
		{
			if ( normalize_x_ > normalize_z_)
			{
				normalize_ = normalize_x_;
			}else
			{
				normalize_ = normalize_z_;
			}

		}else
		{
			if ( normalize_y_ > normalize_z_)
			{
				normalize_ = normalize_y_;
			}else
			{
				normalize_ = normalize_z_;
			}
		}

		normalize_x_ /= normalize_;
		normalize_y_ /= normalize_;
		normalize_z_ /= normalize_;

		min_ = Eigen::Vector4f( _x_min/normalize_,
				        _y_min/normalize_,
					_z_min/normalize_,1.0f);
		max_ = Eigen::Vector4f( _x_max/normalize_,
					_y_max/normalize_,
					_z_max/normalize_,1.0f);

		center_ = Eigen::Vector4f( (max_.x()+min_.x())*0.5,
					   (max_.y()+min_.y())*0.5,
					   (max_.z()+min_.z())*0.5,1.0f);

		min_ = min_ - center_;
		max_ = max_ - center_;

		return true;
	}

	std::vector<Eigen::Vector4f> ExtrusionController::getcubeMesh ( )
	{

		std::vector<Eigen::Vector4f> cube;

		cube =
		{
			//  Top Face
			Eigen::Vector4f ( max_.x(), max_.y(), max_.z(), 1.0f ),
			Eigen::Vector4f ( min_.x(), max_.y(), max_.z(), 1.0f ),
			Eigen::Vector4f ( max_.x(), max_.y(), min_.z(), 1.0f ),
			Eigen::Vector4f ( min_.x(), max_.y(), min_.z(), 1.0f ),
			// Bottom Face
			Eigen::Vector4f ( max_.x(), min_.y(), max_.z(), 1.0f ),
			Eigen::Vector4f ( min_.x(), min_.y(), max_.z(), 1.0f ),
			Eigen::Vector4f ( max_.x(), min_.y(), min_.z(), 1.0f ),
			Eigen::Vector4f ( min_.x(), min_.y(), min_.z(), 1.0f ),
			// Front Face
			Eigen::Vector4f ( max_.x(), max_.y(), max_.z(), 1.0f ),
			Eigen::Vector4f ( min_.x(), max_.y(), max_.z(), 1.0f ),
			Eigen::Vector4f ( max_.x(), min_.y(), max_.z(), 1.0f ),
			Eigen::Vector4f ( min_.x(), min_.y(), max_.z(), 1.0f ),
			// Back Face
			Eigen::Vector4f ( max_.x(), max_.y(), min_.z(), 1.0f ),
			Eigen::Vector4f ( min_.x(), max_.y(), min_.z(), 1.0f ),
			Eigen::Vector4f ( max_.x(), min_.y(), min_.z(), 1.0f ),
			Eigen::Vector4f ( min_.x(), min_.y(), min_.z(), 1.0f ),
			// Left Face
			Eigen::Vector4f ( max_.x(), max_.y(), min_.z(), 1.0f ),
			Eigen::Vector4f ( max_.x(), max_.y(), max_.z(), 1.0f ),
			Eigen::Vector4f ( max_.x(), min_.y(), min_.z(), 1.0f ),
			Eigen::Vector4f ( max_.x(), min_.y(), max_.z(), 1.0f ),
			// Right Face
			Eigen::Vector4f ( min_.x(), max_.y(), max_.z(), 1.0f ),
			Eigen::Vector4f ( min_.x(), max_.y(), min_.z(), 1.0f ),
			Eigen::Vector4f ( min_.x(), min_.y(), max_.z(), 1.0f ),
			Eigen::Vector4f ( min_.x(), min_.y(), min_.z(), 1.0f ),
		};

		return cube;
	}

	std::vector<Eigen::Vector4f> ExtrusionController::getPlanes ( const std::vector<unsigned int>& slice_position )
	{

		std::vector<Eigen::Vector4f> cube;
		for (auto position_iterator: slice_position)
		{
			//float z = ((400/100)*(100-position_iterator))/normalize_;
			float z = (normalize_z_/100)*(100-position_iterator);
			z = z - center_.z();
			// Front Face
			cube.push_back(Eigen::Vector4f ( max_.x(), max_.y(), z, 1.0f ));
                        cube.push_back(Eigen::Vector4f ( min_.x(), max_.y(), z, 1.0f ));
                        cube.push_back(Eigen::Vector4f ( max_.x(), min_.y(), z, 1.0f ));
			cube.push_back(Eigen::Vector4f ( min_.x(), min_.y(), z, 1.0f ));
		}


		return cube;
	}

	std::vector<Eigen::Vector4f> ExtrusionController::sketchLinearInterpolation ( )
	{
		return std::vector<Eigen::Vector4f>();
	}

	std::vector<Eigen::Vector4f> ExtrusionController::sketchRBFInterpolation ( )
	{
		return std::vector<Eigen::Vector4f>();
	}

} /* namespace RRM */
