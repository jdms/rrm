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

		_z_max *= 2.0f;
		_z_min *= 2.0f;

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

//		 this->surfaces[4] = std::make_shared<PlanarSurface>();
//		 Point2 o, l;
//		 o.x = min_.x();
//		 l.x = max_.x()-min_.x();
//		 o.y = max_.z();
//		 l.y = (max_.z()-min_.z());
//		 this->surfaces[4]->requestChangeDiscretization( 16,16 );
//
//		 this->surfaces[4]->setOrigin(o);
//		 this->surfaces[4]->setLenght(l);

		return true;
	}

	// Black Screen Module ---------->
	void ExtrusionController::createBlackScreenExtrusionMesh ( const std::vector<std::vector<Eigen::Vector3f> >& _patchies, float stepx, float stepz, float volume_width, Eigen::Vector3f center, float diagonal, std::vector<Eigen::Vector3f> &_patch)
	{

		_patch.clear();

		std::size_t last;
		std::size_t last_j;

		stepz = volume_width / stepz;

		for ( std::size_t it_patch = 0; it_patch < _patchies.size ( ); it_patch++ )
		{
			for ( float j = 0.0f; j < volume_width; j += stepz )
			{
				for ( std::size_t i = 0; i < ( _patchies[it_patch].size ( ) - stepx ); i += stepx )
				{
					// In the Curve
					_patch.push_back ( Eigen::Vector3f ( _patchies[it_patch][i].x ( )         , _patchies[it_patch][i].y ( )          , j   ) );

					_patch.push_back ( Eigen::Vector3f ( _patchies[it_patch][i + stepx].x ( ) ,  _patchies[it_patch][i + stepx].y ( ) , j  ) );
					// In the Extrude
					_patch.push_back ( Eigen::Vector3f ( _patchies[it_patch][i].x ( )          , _patchies[it_patch][i].y ( )         , j + stepz   ) );

					_patch.push_back ( Eigen::Vector3f ( _patchies[it_patch][i + stepx].x ( )  , _patchies[it_patch][i + stepx].y ( ) , j + stepz  ) );

					last = i;

				}
	//
				if ( last > _patchies[it_patch].size ( ) )
				{
					last -= stepx;
				}

				last -= stepx;

				last_j = j;

				_patch.push_back ( Eigen::Vector3f ( _patchies[it_patch][last].x ( ) , _patchies[it_patch][last].y ( ), last_j   ) );

				_patch.push_back ( Eigen::Vector3f ( _patchies[it_patch].back ( ).x ( ) , _patchies[it_patch].back ( ).y ( ) , last_j   ) );
				// In the Extrude
				_patch.push_back ( Eigen::Vector3f ( _patchies[it_patch][last].x ( ) , _patchies[it_patch][last].y ( ), last_j + stepz  ) );

				_patch.push_back ( Eigen::Vector3f ( _patchies[it_patch].back ( ).x ( ) , _patchies[it_patch].back ( ).y ( ), last_j + stepz  ) );
			}
		}


		std::cout << "Box Center " << center << std::endl;

		for ( std::size_t it = 0; it < _patch.size ( ); it++ )
		{
			_patch[it] = (_patch[it] - center)/diagonal;
		}

	}

	void ExtrusionController::createBlackScreenCube ( const Celer::BoundingBox3<float>& _box, std::vector<Eigen::Vector3f>& _cube)
	{
		_cube.clear();

		Eigen::Vector3f min_ = _box.min();
		Eigen::Vector3f max_ = _box.max();

		Eigen::Vector3f vertex_data[] =
		{
			//  Top Face
			Eigen::Vector3f ( max_.x(), max_.y(), max_.z() ),
			Eigen::Vector3f ( min_.x(), max_.y(), max_.z() ),
			Eigen::Vector3f ( max_.x(), max_.y(), min_.z() ),
			Eigen::Vector3f ( min_.x(), max_.y(), min_.z() ),
			// Bottom Face
			Eigen::Vector3f ( max_.x(), min_.y(), max_.z() ),
			Eigen::Vector3f ( min_.x(), min_.y(), max_.z() ),
			Eigen::Vector3f ( max_.x(), min_.y(), min_.z() ),
			Eigen::Vector3f ( min_.x(), min_.y(), min_.z() ),
			// Front Face
			Eigen::Vector3f ( max_.x(), max_.y(), max_.z() ),
			Eigen::Vector3f ( min_.x(), max_.y(), max_.z() ),
			Eigen::Vector3f ( max_.x(), min_.y(), max_.z() ),
			Eigen::Vector3f ( min_.x(), min_.y(), max_.z() ),
			// Back Face
			Eigen::Vector3f ( max_.x(), max_.y(), min_.z() ),
			Eigen::Vector3f ( min_.x(), max_.y(), min_.z() ),
			Eigen::Vector3f ( max_.x(), min_.y(), min_.z() ),
			Eigen::Vector3f ( min_.x(), min_.y(), min_.z() ),
			// Left Face
			Eigen::Vector3f ( max_.x(), max_.y(), min_.z() ),
			Eigen::Vector3f ( max_.x(), max_.y(), max_.z() ),
			Eigen::Vector3f ( max_.x(), min_.y(), min_.z() ),
			Eigen::Vector3f ( max_.x(), min_.y(), max_.z() ),
			// Right Face
			Eigen::Vector3f ( min_.x(), max_.y(), max_.z() ),
			Eigen::Vector3f ( min_.x(), max_.y(), min_.z() ),
			Eigen::Vector3f ( min_.x(), min_.y(), max_.z() ),
			Eigen::Vector3f ( min_.x(), min_.y(), min_.z() ),
		};

		std::copy( vertex_data	 , vertex_data + 24	, std::back_inserter ( _cube ) );

		for ( std::size_t it = 0; it < _cube.size ( ); it++ )
		{
			_cube[it] = ( _cube[it] - _box.center ( ) ) / _box.diagonal ( );
		}

	}

	void ExtrusionController::setBlackScreenCrossSection (const CrossSection& _cross_section)
	{
		this->cross_section_ = _cross_section;
	}

	void ExtrusionController::updateBlackScreenMesh ( float stepx, float stepz, float volume_width, std::vector<Eigen::Vector3f>& _cube,std::vector<Eigen::Vector3f> &_patch)
	{

		std::vector<std::vector<Eigen::Vector3f> > 	patchies;
		std::vector<Eigen::Vector3f> 			patch;

		Celer::BoundingBox3<float> 			box;

		for ( auto edge: this->cross_section_.edges_ )
		{
			patch.clear ( );
			patch.resize ( edge.second.segment.curve.size() );

			if ( !edge.second.is_boundary_ )
			{
				for ( std::size_t p_it = 0; p_it < edge.second.segment.curve.size(); p_it++ )
				{
					patch[p_it] = Eigen::Vector3f ( edge.second.segment.curve[p_it].x(),edge.second.segment.curve[p_it].y(),1.0f);
				}
				patchies.push_back(patch);
			}
		}

		std::vector<Eigen::Vector3f> points = { Eigen::Vector3f (cross_section_.viewPort_.first.x(), cross_section_.viewPort_.first.y(), 0.0f),
							Eigen::Vector3f (cross_section_.viewPort_.second.x(), cross_section_.viewPort_.second.y(), volume_width) };
		box.reset();

		box.fromPointCloud(points.begin(),points.end());

		_patch.clear();

		if ( patchies.size() > 0)
		{
			createBlackScreenExtrusionMesh(patchies,stepx, stepz,volume_width,box.center(),box.diagonal(),_patch);
		}
		createBlackScreenCube(box,_cube);
	}

	// Seismic Module --------------->
	std::vector<Eigen::Vector3f> ExtrusionController::getcubeMesh ( )
	{
		std::vector<Eigen::Vector3f> cube;

		cube =
		{
			//  Top Face
			Eigen::Vector3f ( max_.x(), max_.y(), max_.z()),
			Eigen::Vector3f ( min_.x(), max_.y(), max_.z()),
			Eigen::Vector3f ( max_.x(), max_.y(), min_.z()),
			Eigen::Vector3f ( min_.x(), max_.y(), min_.z()),
			// Bottom Face
			Eigen::Vector3f ( max_.x(), min_.y(), max_.z()),
			Eigen::Vector3f ( min_.x(), min_.y(), max_.z()),
			Eigen::Vector3f ( max_.x(), min_.y(), min_.z()),
			Eigen::Vector3f ( min_.x(), min_.y(), min_.z()),
			// Front Face
//			Eigen::Vector3f ( max_.x(), max_.y(), max_.z()),
//			Eigen::Vector3f ( min_.x(), max_.y(), max_.z()),
//			Eigen::Vector3f ( max_.x(), min_.y(), max_.z()),
//			Eigen::Vector3f ( min_.x(), min_.y(), max_.z()),
			// Back Face
			Eigen::Vector3f ( max_.x(), max_.y(), min_.z()),
			Eigen::Vector3f ( min_.x(), max_.y(), min_.z()),
			Eigen::Vector3f ( max_.x(), min_.y(), min_.z()),
			Eigen::Vector3f ( min_.x(), min_.y(), min_.z()),
			// Left Face
			Eigen::Vector3f ( max_.x(), max_.y(), min_.z()),
			Eigen::Vector3f ( max_.x(), max_.y(), max_.z()),
			Eigen::Vector3f ( max_.x(), min_.y(), min_.z()),
			Eigen::Vector3f ( max_.x(), min_.y(), max_.z()),
			// Right Face
			Eigen::Vector3f ( min_.x(), max_.y(), max_.z()),
			Eigen::Vector3f ( min_.x(), max_.y(), min_.z()),
			Eigen::Vector3f ( min_.x(), min_.y(), max_.z()),
			Eigen::Vector3f ( min_.x(), min_.y(), min_.z())
		};

		return cube;
	}

	std::vector<Eigen::Vector4f> ExtrusionController::getPlanes ( const std::vector<unsigned int>& slice_position )
	{
		std::vector<Eigen::Vector4f> cube;
		for (auto position_iterator: slice_position)
		{
			//float z = ((400/100)*(100-position_iterator))/scale_;
			float z = ( max_.z() - ((position_iterator)/scale_) );
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

	std::vector<Eigen::Vector4f> ExtrusionController::updateSeismicSlices (std::vector<float>& vl,std::vector<float>& nl,std::vector<std::size_t>& fl)
	{

		vl.clear();
		nl.clear();
		fl.clear();


		std::cout << " back " << (scale_z_)*(294/scale_) << std::endl;
		std::cout << " front " << (scale_z_)*(1/scale_) << std::endl;
		std::cout << " min " << min_ << std::endl;
		std::cout << " max " << max_ << std::endl;


		 this->surfaces[2] = std::make_shared<PlanarSurface>();
		 Point2 o, l;
		 o.x = min_.x();
		 l.x = (max_.x()-min_.x());
		 o.y = min_.z();
		 l.y = (max_.z()-min_.z());
		 this->surfaces[2]->requestChangeDiscretization( 16,16 );

		 this->surfaces[2]->setOrigin(o);
		 this->surfaces[2]->setLenght(l);

		 //
		 this->surfaces[1] = std::make_shared<PlanarSurface>();

		 this->surfaces[1]->requestChangeDiscretization( 16,16 );

		 this->surfaces[1]->setOrigin(o);
		 this->surfaces[1]->setLenght(l);

		std::vector<Eigen::Vector4f> lines;

		if ( this->seismic_slices_.size() < 1 )
			return lines;

		for ( auto slice_iterator: this->seismic_slices_ )
		{
			std:: cout << "Extrusion Controller " << slice_iterator.first << std::endl;

			for ( auto& edges_iterator: slice_iterator.second.edges_ )
			{
				std:: cout << "Curve Size" << edges_iterator.second.segment.curve_index << std::endl;

				for ( std::size_t it = 0; it < edges_iterator.second.segment.curve.size() -1 ; it++)
				{
					float z = ( max_.z() - (scale_z_*(2*(slice_iterator.first-1))/scale_) );
					//z = center_.z() - z;
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

					Eigen::Vector3d v(point1.x(),point1.z(),point1.y());

					if ( edges_iterator.second.is_boundary_  )
					{
					}
					else if ( edges_iterator.second.is_visible_)
					{
						surfaces[edges_iterator.second.segment.curve_index]->addPoint ( v );
					}
				}
			}
		}

		/* Do useful stuff. */
		using VListType = std::vector<float>;
		VListType s1_points, s2_points, s3_points, sn_points;
		VListType s1_vlist, s2_vlist, s3_vlist, sn_vlist;

		using FListType = std::vector<unsigned int>;
		FListType s1_flist, s2_flist, s3_flist, sn_flist;

		if ( surfaces[2]->surfaceIsSet() == false ) {
			surfaces[2]->generateSurface();
		}

		surfaces[2]->getVertexList ( vl );
		surfaces[2]->getNormalList ( nl );
		surfaces[2]->getFaceList ( fl );


		std::cout << "face Count " << fl.size() << std::endl;
		std::cout << "Normal Count " << nl.size() << std::endl;
		std::cout << "Vertex Count " << vl.size() << std::endl;

		return lines;
	}

	void  ExtrusionController::setSeismicSlices ( const SeismicSlices& _seismic_slices)
	{
		this->seismic_slices_ = _seismic_slices;
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

