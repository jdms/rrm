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
		colors_.push_back(Eigen::Vector3f(0.0f,0.0f,0.0f));
		colors_.push_back(Eigen::Vector3f(141.0f/255.0f,211.0f/255.0f,199.0f/255.0f));
		colors_.push_back(Eigen::Vector3f(255.0f/255.0f,255.0f/255.0f,179.0f/255.0f));
		colors_.push_back(Eigen::Vector3f(190.0f/255.0f,186.0f/255.0f,218.0f/255.0f));
		colors_.push_back(Eigen::Vector3f(251.0f/255.0f,128.0f/255.0f,114.0f/255.0f));
		colors_.push_back(Eigen::Vector3f(128.0f/255.0f,177.0f/255.0f,211.0f/255.0f));

		this->resolution_ = 16;
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


		float zmax = ( max_.z() - (scale_z_*(2*(1))/scale_) );
		float zmin = ( max_.z() - (scale_z_*(2*(290))/scale_) );

		 Point2 o, l;
		 o.x = min_.x();
		 l.x = (max_.x()-min_.x());
		 o.y = zmin;
		 l.y = (zmax - zmin);

		 for ( std::size_t surfaces_iterator = 1; surfaces_iterator < 6; surfaces_iterator++)
		 {
			 this->surfaces[surfaces_iterator] = std::make_shared<PlanarSurface>();
			 this->surfaces[surfaces_iterator]->requestChangeDiscretization( resolution_,resolution_ );
			 this->surfaces[surfaces_iterator]->setOrigin(o);
			 this->surfaces[surfaces_iterator]->setLenght(l);
			 this->number_of_curves_[surfaces_iterator] = 0;
		 }

		return true;
	}
	// Black Screen Module ---------->
	void ExtrusionController::createBlackScreenExtrusionMesh ( const std::vector<std::vector<Eigen::Vector3f> >& _patchies, float stepx, float stepz, float volume_width, Eigen::Vector3f center, float diagonal, std::vector<Eigen::Vector3f> &_patch)
	{

		_patch.clear();

		std::size_t last;

		stepz = volume_width / stepz;

		for ( std::size_t it_patch = 0; it_patch < _patchies.size ( ); it_patch++ )
		{
			for ( float j = 0.0f; j <= volume_width-stepz; j += stepz )
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
					last = _patchies[it_patch].size ( );
				}

				//last -= stepx;
				if ( j > volume_width )
				{
					j = volume_width;
				}

				_patch.push_back ( Eigen::Vector3f ( _patchies[it_patch][last].x ( ) , _patchies[it_patch][last].y ( ), j   ) );

				_patch.push_back ( Eigen::Vector3f ( _patchies[it_patch].back ( ).x ( ) , _patchies[it_patch].back ( ).y ( ) , j   ) );
				// In the Extrude
				_patch.push_back ( Eigen::Vector3f ( _patchies[it_patch][last].x ( ) , _patchies[it_patch][last].y ( ), j + stepz  ) );

				_patch.push_back ( Eigen::Vector3f ( _patchies[it_patch].back ( ).x ( ) , _patchies[it_patch].back ( ).y ( ), j + stepz  ) );
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

		Eigen::Vector3f min_ = _box.Min();
		Eigen::Vector3f max_ = _box.Max();

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
				if ( edge.second.is_visible_ && edge.second.is_enable_ )
				{
					for ( std::size_t p_it = 0; p_it < edge.second.segment.curve.size(); p_it++ )
					{
						patch[p_it] = Eigen::Vector3f ( edge.second.segment.curve[p_it].x(),edge.second.segment.curve[p_it].y(),1.0f);
					}
					patchies.push_back(patch);
				}

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

	void ExtrusionController::exportBlankScreen(const std::string& _file_name, int _extrusion_size)
	{

		std::ofstream output;
		output.open(_file_name);

		ExtrusionController::CrossSection cross_section = this->cross_section_;
					
		int number_points_ = 0; /// Number of point per sketch 
		std::vector<unsigned int> number_edges_; // Number of Sketch Segments
		// Number of Internal Surface
		int number_lines_ = cross_section.curves_history_.size(); //number of surfaces

		output << number_lines_ << std::endl;
		
		float diagonal = std::pow((cross_section.viewPort_.first.x() - cross_section.viewPort_.second.x()), 2) + std::pow((cross_section.viewPort_.first.y() - cross_section.viewPort_.second.y()), 2);

		diagonal = std::sqrt(diagonal);

		std::cout << "Diagonal " << diagonal << std::endl;

		for (auto& history_iterator : cross_section.curves_history_)
		{

			number_points_ = 0;
			number_edges_.clear(); //number of surfaces

			for (auto& edge_iterator : cross_section.edges_)
			{//now is number of surfaces
				if ((edge_iterator.second.is_boundary_ == false) && (edge_iterator.second.is_visible_ == true) && (edge_iterator.second.segment.curve_index == history_iterator.second.curve_index))
				{ //only internal sketched
					number_edges_.push_back(edge_iterator.second.id_);
					edge_iterator.second.segment.curve.chaikinFilter(5);
					number_points_ += edge_iterator.second.segment.curve.size();
				}
			}

			number_points_ -= (number_edges_.size() - 1);

			output << number_points_ << " " << _extrusion_size << std::endl; //nx and ny of the surface

			float extrusion_step = 0;

			for (int j = 0; j < _extrusion_size; j++) //output is firstly x direction then y direction for each surface
			{
				for (unsigned int id = 0; id < number_edges_.size() - 1; id++)
				{ //now is number of surfaces
					int edge_id = number_edges_[id];
					for (std::size_t it = 0; it < cross_section.edges_[edge_id].segment.curve.size() - 1; it++)
					{
						output << cross_section.edges_[edge_id].segment.curve[it].x() << " " << j + extrusion_step << " " << cross_section.edges_[edge_id].segment.curve[it].y() << std::endl;
					}
				}

				for (std::size_t it = 0; it < cross_section.edges_[number_edges_.back()].segment.curve.size(); it++)
				{
					output << cross_section.edges_[number_edges_.back()].segment.curve[it].x() << " " << j + extrusion_step << " " << cross_section.edges_[number_edges_.back()].segment.curve[it].y() << std::endl;
				}

				extrusion_step += ( diagonal * 0.01);
			}

		}
		output.close();


	}

	// Seismic Module --------------->
	void ExtrusionController::setResolution(int _resolution,std::vector<float>& vl,std::vector<float>& nl,std::vector<std::size_t>& fl)
	{
		this->resolution_ = _resolution;

		std::vector<float> vt;
		std::vector<float> nt;
		std::vector<std::size_t> ft;
		std::size_t stride = 0;

		vl.clear();
		nl.clear();
		fl.clear();

		 for ( std::size_t surfaces_iterator = 1; surfaces_iterator < 6; surfaces_iterator++)
		 {
			 this->surfaces[surfaces_iterator]->requestChangeDiscretization( resolution_,resolution_ );

			 if ( surfaces[surfaces_iterator]->surfaceIsSet()  )
			 {

				 	vt.clear();
					nt.clear();
					ft.clear();

					surfaces[surfaces_iterator]->getVertexList ( vt );
					surfaces[surfaces_iterator]->getNormalList ( nt );
					surfaces[surfaces_iterator]->getFaceList ( ft );

					for ( auto& index: ft)
					{
						index+=stride;
					}

					for ( std::size_t it = 0; it < nt.size()-3; it+=3 )
					{
						nt[it+0] = colors_[surfaces_iterator].x();
						nt[it+1] = colors_[surfaces_iterator].y();
						nt[it+2] = colors_[surfaces_iterator].z();
					}

					vl.insert(vl.end(),vt.begin(),vt.end());
					nl.insert(nl.end(),nt.begin(),nt.end());

					stride += static_cast<std::size_t>(vt.size()/3);

					fl.insert(fl.end(),ft.begin(),ft.end());
			 }
		 }


	}
	std::vector<float> ExtrusionController::getCubeMesh ( )
	{


		float zmax = ( max_.z() - (scale_z_*(2*(1))/scale_) );
		float zmin = ( max_.z() - (scale_z_*(2*(290))/scale_) );


		std::vector<float> cube =
		{
			//  Top Face
			max_.x(), max_.y(), zmax,1.0,
			min_.x(), max_.y(), zmax,1.0,
			max_.x(), max_.y(), zmin,1.0,
			min_.x(), max_.y(), zmin,1.0,
			// Bottom Face
			max_.x(), min_.y(), zmax,1.0,
			min_.x(), min_.y(), zmax,1.0,
			max_.x(), min_.y(), zmin,1.0,
			min_.x(), min_.y(), zmin,1.0,
			// Front Face
			max_.x(), max_.y(), zmax,1.0,
			min_.x(), max_.y(), zmax,1.0,
			max_.x(), min_.y(), zmax,1.0,
			min_.x(), min_.y(), zmax,1.0,
			// Back Face
			max_.x(), max_.y(), zmin,1.0,
			min_.x(), max_.y(), zmin,1.0,
			max_.x(), min_.y(), zmin,1.0,
			min_.x(), min_.y(), zmin,1.0,
			// Left Face
			max_.x(), max_.y(), zmin,1.0,
			max_.x(), max_.y(), zmax,1.0,
			max_.x(), min_.y(), zmin,1.0,
			max_.x(), min_.y(), zmax,1.0,
			// Right Face
			min_.x(), max_.y(), zmax,1.0,
			min_.x(), max_.y(), zmin,1.0,
			min_.x(), min_.y(), zmax,1.0,
			min_.x(), min_.y(), zmin,1.0
		};

		return cube;
	}

	std::vector<float> ExtrusionController::getPlaneMesh ( float _index )
	{
		std::vector<float> cube =
		{
			// Front Face
			max_.x(), max_.y(), max_.z(),1.0,
			min_.x(), max_.y(), max_.z(),1.0,
			max_.x(), min_.y(), max_.z(),1.0,
			min_.x(), min_.y(), max_.z(),1.0
		};

		return cube;
	}

	float ExtrusionController::slicePositon( int _index )
	{
		return ( max_.z() - (scale_z_*(2*(_index))/scale_) );
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
	// Interpolation
	std::vector<Eigen::Vector4f> ExtrusionController::updateSeismicSlices (std::vector<float>& vl,std::vector<float>& nl,std::vector<std::size_t>& fl)
	{

		vl.clear();
		nl.clear();
		fl.clear();


		std::cout << " back " << (scale_z_)*(294/scale_) << std::endl;
		std::cout << " front " << (scale_z_)*(1/scale_) << std::endl;
		std::cout << " min " << min_ << std::endl;
		std::cout << " max " << max_ << std::endl;

		float zmax = ( max_.z() - (scale_z_*(2*(1))/scale_) );
		float zmin = ( max_.z() - (scale_z_*(2*(290))/scale_) );

		 Point2 o, l;
		 o.x = min_.x();
		 l.x = (max_.x()-min_.x());
		 o.y = zmin;
		 l.y = (zmax - zmin);


		 this->number_of_curves_.clear();

		 for ( std::size_t surfaces_iterator = 1; surfaces_iterator < 6; surfaces_iterator++)
		 {
			 this->surfaces[surfaces_iterator] = std::make_shared<PlanarSurface>();
			 this->surfaces[surfaces_iterator]->requestChangeDiscretization( resolution_,resolution_ );
			 this->surfaces[surfaces_iterator]->setOrigin(o);
			 this->surfaces[surfaces_iterator]->setLenght(l);
			 this->number_of_curves_[surfaces_iterator] = 0;
		 }


		std::vector<Eigen::Vector4f> lines;

		if ( this->seismic_slices_.size() < 1 )
			return lines;

		for ( auto slice_iterator: this->seismic_slices_ )
		{
			// FIXME testing
			unsigned int curve_index = 0;

			for ( auto& edges_iterator: slice_iterator.second.edges_ )
			{

				// FIXME testing
				if ( (edges_iterator.second.is_visible_) && (!edges_iterator.second.is_boundary_) && (edges_iterator.second.segment.curve_index < 6) )
				{
					 	if ( curve_index != edges_iterator.second.segment.curve_index )
						{
					 		this->number_of_curves_[edges_iterator.second.segment.curve_index] += 1;
					 		curve_index = edges_iterator.second.segment.curve_index;
						}
				}

				float z = ( max_.z() - (scale_z_*(2*(slice_iterator.first))/scale_) );

				for ( std::size_t it = 0; it < edges_iterator.second.segment.curve.size() -1 ; it++)
				{

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

					if ( edges_iterator.second.is_visible_ && !edges_iterator.second.is_boundary_)
					{
						lines.push_back(point1);
						lines.push_back(point2);

						Eigen::Vector3d v(point1.x(),point1.z(),point1.y());

						if ( edges_iterator.second.is_boundary_  )
						{
						}
						else if ( edges_iterator.second.is_visible_)
						{
							if ( edges_iterator.second.segment.curve_index < 6 )
							{
								surfaces[edges_iterator.second.segment.curve_index]->addPoint ( v );
							}
						}
					}

				}


				if ( edges_iterator.second.is_visible_ && !edges_iterator.second.is_boundary_)
				{

					Eigen::Vector4f point = Eigen::Vector4f(edges_iterator.second.segment.curve.back().x(),
										edges_iterator.second.segment.curve.back().y(),
															 0.0,1.0);
					point = trasnform_matrix_ * point;
					point[2] = z;

					Eigen::Vector3d v(point.x(),point.z(),point.y());

					if ( edges_iterator.second.is_boundary_  )
					{
					}
					else if ( edges_iterator.second.is_visible_)
					{
						if ( edges_iterator.second.segment.curve_index < 6 )
						{
							surfaces[edges_iterator.second.segment.curve_index]->addPoint ( v );
						}
					}
				}

			}
		}

		std::vector<float> vt;
		std::vector<float> nt;
		std::vector<std::size_t> ft;
		std::size_t stride = 0;


		 for ( std::size_t surfaces_iterator = 1; surfaces_iterator < 6; surfaces_iterator++)
		 {

			 if ( this->number_of_curves_[surfaces_iterator] >= 2)
			 {
				 surfaces[surfaces_iterator]->generateSurface();
			 }

			 if ( surfaces[surfaces_iterator]->surfaceIsSet()  )
			 {

					vt.clear();
					nt.clear();
					ft.clear();

					surfaces[surfaces_iterator]->getVertexList ( vt );
					surfaces[surfaces_iterator]->getNormalList ( nt );
					surfaces[surfaces_iterator]->getFaceList ( ft );

					for ( auto& index: ft)
					{
						index+=stride;
					}

					for ( std::size_t it = 0; it < nt.size()-3; it+=3 )
					{
						nt[it+0] = colors_[surfaces_iterator].x();
						nt[it+1] = colors_[surfaces_iterator].y();
						nt[it+2] = colors_[surfaces_iterator].z();
					}

					vl.insert(vl.end(),vt.begin(),vt.end());
					nl.insert(nl.end(),nt.begin(),nt.end());

					stride += static_cast<std::size_t>(vt.size()/3);

					fl.insert(fl.end(),ft.begin(),ft.end());

//					std::cout << "stride" << stride << std::endl;
//
//					for ( auto i : fl )
//						std::cout << "index " << i << std::endl;
//
//					std::cout << "face Count " << fl.size() << std::endl;
//					std::cout << "Normal Count " << nl.size() << std::endl;
//					std::cout << "Vertex Count " << vl.size() << std::endl;
			 }

	 }


//		std::cout << "face Count " << fl.size() << std::endl;
//		std::cout << "Normal Count " << nl.size() << std::endl;
//		std::cout << "Vertex Count " << vl.size() << std::endl;

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

