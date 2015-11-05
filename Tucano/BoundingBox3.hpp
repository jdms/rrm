#ifndef CELER_BOUNDINGBOX3_HPP_
#define CELER_BOUNDINGBOX3_HPP_

// from Standard Library
#include <vector>
#include <limits>
// Celer Library
#include <Eigen/Dense>

namespace Celer
{

	/*!
	 *@class BoundingBox3.
	 *@brief Class that represent a Box in 3D.
	 *@details coming soon , but ... coming soon  ... wait ¬¬.
	 *@author Felipe Moura.
	 *@version 1.0.
	 *@date 25-Feb-2008.
	 *@todo Give a model, polygon, ellipse ... and so one, with its coords point create a Box.
	 */

	// Box ///////////////////////////////////////////////////////////////////////
	//    * ------*
	//   /|      /|
	//  *-----max
	//  | |     | |
	//  | min ----*
	//  |/      |/
	//  * ------*

	template < class Real >
	class BoundingBox3
	{
		private:

			//@see http://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html

			typedef Eigen::Matrix<Real, 3, 1> Vector3;
			typedef Eigen::Matrix<Real, 4, 1> Vector4;

			Vector3 min_;
			Vector3 max_;

			/// For the oriented bounding Box;
			Vector3 basis_[3];

		public:

			BoundingBox3 ( )
			{
				this->reset();
			}

			BoundingBox3 ( const BoundingBox3<Real>& box )
			{
				this->min_ = Vector3 ( box.min ( ) );
				this->max_ = Vector3 ( box.max ( ) );
			}

			BoundingBox3 ( const Vector3& point_min , const Vector3& point_max )
			{
				this->min_ = Vector3 ( point_min );
				this->max_ = Vector3 ( point_max );
			}

			BoundingBox3 ( const Real& xMin , const Real& yMin , const Real& zMin , const Real& xMax , const Real& yMax , const Real& zMax )
			{
				this->min_ = Vector3 ( xMin , yMin , zMin );
				this->max_ = Vector3 ( xMax , yMax , zMax );
			}

			void reset ( )
			{
				// FIXME How to get the real limits of the bounding box using std::numeric_limits
				this->min_ = Vector3 (  std::numeric_limits<Real>::max(), std::numeric_limits<Real>::max(), std::numeric_limits<Real>::max() );
				this->max_ = Vector3 ( -std::numeric_limits<Real>::max(),-std::numeric_limits<Real>::max(),-std::numeric_limits<Real>::max() );

				basis_[0] = Vector3(1.0,0.0,0.0);
				basis_[0] = Vector3(0.0,1.0,0.0);
				basis_[0] = Vector3(0.0,0.0,1.0);

			}

			void fromPointCloud( typename std::vector<Vector3 >::const_iterator new_point_begin, typename std::vector<Vector3 >::const_iterator new_point_end)
			{
				this->reset();

				for ( typename std::vector<Vector3 >::const_iterator new_point = new_point_begin; new_point != new_point_end; new_point++)
				{

					 BoundingBox3<Real> box( std::min ( min_.x() , new_point->x() ) ,
								 std::min ( min_.y() , new_point->y() ) ,
								 std::min ( min_.z() , new_point->z() ) ,
								 std::max ( max_.x() , new_point->x() ) ,
								 std::max ( max_.y() , new_point->y() ) ,
								 std::max ( max_.z() , new_point->z() ) );

//						std::cout << min( );
//						std::cout << max( );
//						std::cout << " new_point " << *new_point;

					 *this = *this + box;
				}

			}

			void fromPointCloud( typename std::vector<Vector4>::const_iterator new_point_begin, typename std::vector<Vector4>::const_iterator new_point_end)
			{
				this->reset();

				for ( typename std::vector<Vector4>::const_iterator new_point = new_point_begin; new_point != new_point_end; new_point++)
				{

					 BoundingBox3<Real> box( std::min ( min_.x() , new_point->x() ) ,
								 std::min ( min_.y() , new_point->y() ) ,
								 std::min ( min_.z() , new_point->z() ) ,
								 std::max ( max_.x() , new_point->x() ) ,
								 std::max ( max_.y() , new_point->y() ) ,
								 std::max ( max_.z() , new_point->z() ) );

//						std::cout << min( );
//						std::cout << max( );
//						std::cout << " new_point " << *new_point;

					 *this = *this + box;
				}

			}

			void fromPointCloud( const typename std::vector<Vector4>& points,
			                     const Vector3& first_basis,
			                     const Vector3& second_basis,
			                     const Vector3& third_basis )
			{
				this->reset();

				// Reference :
				// Geometric Tools, LLC
				// Copyright (c) 1998-2012
				// Distributed under the Boost Software License, Version 1.0.
				// Wm5ContBox3.cpp

				basis_[0] = first_basis;
				basis_[1] = second_basis;
				basis_[2] = third_basis;


				for ( typename std::vector<Vector4>::const_iterator new_point = points.begin(); new_point != points.end(); new_point++)
				{

					 BoundingBox3<Real> box( std::min ( min_.x() , new_point->x() ) ,
								 std::min ( min_.y() , new_point->y() ) ,
								 std::min ( min_.z() , new_point->z() ) ,
								 std::max ( max_.x() , new_point->x() ) ,
								 std::max ( max_.y() , new_point->y() ) ,
								 std::max ( max_.z() , new_point->z() ) );

//						std::cout << min( );
//						std::cout << max( );
//						std::cout << " new_point " << *new_point;

					 *this = *this + box;
				}

				Vector3 diff = points[0].toVector3() - this->center();
				Vector3 pmin ( ( diff * basis_[0] ) , ( diff * basis_[1] ) , (diff * basis_[2]) );
				Vector3 pmax = pmin;


				for ( typename std::vector<Vector4>::const_iterator new_point = points.begin(); new_point != points.end(); new_point++)
				{
//					diff = points[i] - box.Center;
//					for ( int j = 0; j < 3; ++j )
//					{
//						Real dot = diff.Dot ( box.Axis[j] );
//						if ( dot < pmin[j] )
//						{
//							pmin[j] = dot;
//						}
//						else if ( dot > pmax[j] )
//						{
//							pmax[j] = dot;
//						}
//					}

				}


//				for ( int i = 1; i < numPoints; ++i )
//				{
//				}

//				min_ += ( ( (Real) 0.5 ) * ( pmin[0] + pmax[0] ) ) * box.Axis[0] + ( ( (Real) 0.5 ) * ( pmin[1] + pmax[1] ) ) * box.Axis[1] + ( ( (Real) 0.5 ) * ( pmin[2] + pmax[2] ) ) * box.Axis[2];

			}


			Real diagonal ( ) const
			{

				return std::sqrt ( ( this->min_.x() - this->max_.x() ) * ( this->min_.x() - this->max_.x() ) +
						   ( this->min_.y() - this->max_.y() ) * ( this->min_.y() - this->max_.y() ) +
						   ( this->min_.z() - this->max_.z() ) * ( this->min_.z() - this->max_.z() ));

			}

			Vector3 center ( ) const
			{
				return Vector3 ( (max_ + min_) * static_cast<Real>( 0.5 ) );
			}

			inline const Vector3& min ( ) const
			{
				return ( this->min_ );
			}

			inline const Vector3& max ( ) const
			{
				return ( this->max_ );
			}

			inline bool operator== ( const BoundingBox3<Real>& box ) const
			{
				return ( min ( ) == box.min ( ) and max ( ) == box.max ( ) );
			}

			inline bool operator!= ( const BoundingBox3<Real>& box ) const
			{
				return ! ( box == *this );
			}

			inline BoundingBox3<Real>& operator= ( const BoundingBox3<Real>& box )
			{
				this->min_ = box.min ( );
				this->max_ = box.max ( );

				return ( *this );
			}

			inline BoundingBox3<Real> operator+ ( const BoundingBox3<Real>& box ) const
			{
				return BoundingBox3<Real> (  std::min ( min_.x() , box.min ( ).x() ) ,
							     std::min ( min_.y() , box.min ( ).y() ) ,
							     std::min ( min_.z() , box.min ( ).z() ) ,
							     std::max ( max_.x() , box.max ( ).x() ) ,
							     std::max ( max_.y() , box.max ( ).y() ) ,
							     std::max ( max_.z() , box.max ( ).z() ) );
			}

			inline BoundingBox3<Real> operator+ ( const Vector3& new_point ) const
			{
				return BoundingBox3<Real> (  std::min ( min_.x() , new_point.x() ) ,
							     std::min ( min_.y() , new_point.y() ) ,
							     std::min ( min_.z() , new_point.z() ) ,
							     std::max ( max_.x() , new_point.x() ) ,
							     std::max ( max_.y() , new_point.y() ) ,
							     std::max ( max_.z() , new_point.z() ) );
			}

			bool intersect ( const Vector3& p ) const
			{
				return ( ( p.x() ( ) >= this->min_.x() ) and ( p.x() ( ) < this->max_.x() ) and
		                         ( p.y() ( ) >= this->min_.y() ) and ( p.y() ( ) < this->max_.y() ) and
		                         ( p.z() ( ) >= this->min_.z() ) and ( p.z() ( ) < this->max_.z() ) );
			}

			bool intersect ( const Celer::BoundingBox3<Real>& box ) const
			{
				return ( ( box.max( ).x() > this->min_.x() ) and ( box.min( ).x() < this->max_.x() ) and
		                         ( box.max( ).y() > this->min_.y() ) and ( box.min( ).y() < this->max_.y() ) and
		                         ( box.max( ).z() > this->min_.z() ) and ( box.min( ).z() < this->max_.z() ) );
			}


			virtual ~BoundingBox3 ( )
			{

			}
	};

}/* Celer :: NAMESPACE */

#endif /*BOUNDINGBOX3_HPP_*/


//Celer::BoundingBox3<float> box;
//
//Celer::Vector3< float > v[9];
//
//v[0] = Celer::Vector3<float> ( 1.f , 0.f , -1.f );
//v[1] = Celer::Vector3<float> ( 1.f , 0.f , 1.f );
//v[2] = Celer::Vector3<float> ( -1.f , 0.f , 1.f );
//v[3] = Celer::Vector3<float> ( -1.f , 0.f , -1.f );
//
//v[4] = Celer::Vector3<float> ( 1.f , 1.f , -1.f );
//v[5] = Celer::Vector3<float> ( 1.f , 1.f , 1.f );
//v[6] = Celer::Vector3<float> ( -1.f , 1.f , 1.f );
//v[7] = Celer::Vector3<float> ( -1.f , 1.f , -1.f );
//
//v[8] = Celer::Vector3<float> ( 0.f , 2.f , 0.f );
//
//for (int var = 0; var < 9 ; ++var)
//{
//	box = box + v[var];
//}
//
//std::cout << "box center : " << box.center( ) << std::endl;
//
//std::cout << "box min : " << box.min( ) << std::endl;
//std::cout << "box max : " << box.max( ) << std::endl;
