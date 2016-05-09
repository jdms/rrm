/*
 * ExtrusionController.cpp
 *
 *  Created on: Apr 29, 2016
 *      Author: felipe
 */

#include "ExtrusionController.hpp"

namespace RRM
{

	ExtrusionController::ExtrusionController ( QObject* parent  ) : QObject(parent)
	{

	}

	ExtrusionController::~ExtrusionController ( )
	{

	}

	/// Use the extensions of the scene bounding box
	bool  ExtrusionController:: initialize (  float _x_min,
						  float _y_min,
						  float _z_min,
						  float _x_max,
						  float _y_max,
						  float _z_max )
	{
		scale_x_ = ( _x_max - _x_min);
		scale_y_ = ( _y_max - _y_min);
		scale_z_ = ( _z_max - _z_min);

		if ( scale_x_ > scale_y_ )
		{
			if ( scale_x_ > scale_z_)
			{
				scale_ = scale_x_;
			}else
			{
				scale_ = scale_z_;
			}

		}else
		{
			if ( scale_y_ > scale_z_)
			{
				scale_ = scale_y_;
			}else
			{
				scale_ = scale_z_;
			}
		}

		scale_x_ /= scale_;
		scale_y_ /= scale_;
		scale_z_ /= scale_;

		min_ = Eigen::Vector4f(_x_min,_y_min,_z_min,1.0f);
		max_ = Eigen::Vector4f(_x_max,_y_max,_z_max,1.0f);

		center_ = Eigen::Vector4f( (max_.x()+min_.x())*0.5,
					   (max_.y()+min_.y())*0.5,
					   (max_.z()+min_.z())*0.5,0.0);
		// scaling the centre
		center_ = center_ / scale_;

		trasnform_matrix_ = Eigen::Affine3f::Identity();

		trasnform_matrix_.translation() = -center_.head<3>();
		trasnform_matrix_.scale (Eigen::Vector3f(1.0f/scale_,1.0f/scale_,1.0f/scale_));


		min_ = trasnform_matrix_.matrix() * min_;
		max_ = trasnform_matrix_.matrix() * max_;

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
			//float z = ((400/100)*(100-position_iterator))/scale_;
			float z = (scale_z_/100)*(100-position_iterator);
			z = z - center_.z();
			// Front Face
			cube.push_back(Eigen::Vector4f ( max_.x(), max_.y(), z, 1.0f ));
                        cube.push_back(Eigen::Vector4f ( min_.x(), max_.y(), z, 1.0f ));
                        cube.push_back(Eigen::Vector4f ( max_.x(), min_.y(), z, 1.0f ));
			cube.push_back(Eigen::Vector4f ( min_.x(), min_.y(), z, 1.0f ));
		}

		return cube;
	}

	std::vector<Eigen::Vector4f> ExtrusionController::getSketchLines ( )
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

	std::vector<Eigen::Vector4f> ExtrusionController::updateSeismicSlices ( const std::map<unsigned int, SeismicSlice >& _seismic_slices)
	{
		this->seismic_slices_ = _seismic_slices;

		std::vector<Eigen::Vector4f> lines;

		for ( auto slice_iterator: this->seismic_slices_ )
		{
			std:: cout << "Extrusion Controller " << slice_iterator.second.edges_.size() << std::endl;

			for ( auto& edges_iterator: slice_iterator.second.edges_ )
			{
				std:: cout << "Curve Size" << edges_iterator.second.segment.curve.size() << std::endl;

				for ( std::size_t it = 0; it < edges_iterator.second.segment.curve.size() -1 ; it++)
				{
					float z = (scale_z_/100)*(100-10*slice_iterator.first);
					z = z - center_.z();
					Eigen::Vector4f point1 = Eigen::Vector4f(edges_iterator.second.segment.curve[it].x(),
										edges_iterator.second.segment.curve[it].y(),
										0.0,1.0);

					Eigen::Vector4f point2 = Eigen::Vector4f(edges_iterator.second.segment.curve[it+1].x(),
										edges_iterator.second.segment.curve[it+1].y(),
										0.0,1.0);

					point1 = trasnform_matrix_ * point1;
					point2 = trasnform_matrix_ * point2;

					point1[2] = z;
					point2[2] = z;

					lines.push_back(point1);
					lines.push_back(point2);
				}
			}
		}

		return lines;
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

