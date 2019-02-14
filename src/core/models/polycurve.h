/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file polycurve.h
 * @author Clarissa C. Marques
* @brief File containing the class PolyCurve
 */


#ifndef POLYCURVE_H
#define POLYCURVE_H

#include <vector>

#include "./polygonalcurve/CurveN.hpp"


/**
 *  A data structure to handle a curve possibly not connected, i.e., with segments that not connect each other
 */


class PolyCurve
{

    public:


        /**
        * Constructor.
        */
        PolyCurve() = default;


        /**
        * Constructor. Creates a new Polycurve given a vector of Curve2D
        */
        PolyCurve( const std::vector< Curve2D >& curve_ )
        {
            curve = curve_;
        }


        /**
        * Constructor. Creates a new Polycurve given a Curve2D
        */
        PolyCurve( const Curve2D& curve_ )
        {
            curve.clear();
            curve.push_back( curve_ );
        }


        /**
        * Constructor. Creates a new Polycurve given a vector of vertices (double) and optionally edges
        */
        PolyCurve( const std::vector< double >& vs_, const std::vector< std::size_t >& es_ = std::vector< std::size_t >() )
        {
            fromVector( vs_, es_ );
        }


        /**
        * Constructor. Creates a new Polycurve given a vector of vertices (float)
        * and optionally edges
        */
        PolyCurve( const std::vector< float >& vs_, const std::vector< std::size_t >& es_ = std::vector< std::size_t >() )
        {
            fromVector( vs_, es_ );
        }


        /**
        * Method to add a new segment of curve to the curve
        * @param curve a segment of curve
        * @see Curve2D
        * @return void.
        */
        void addSubcurve( const Curve2D& curve_ )
        {
            curve.push_back( curve_ );
        }


        /**
        * Method to return a segment of curve
        * @param index the index of the segment
        * @see Curve2D
        * @return void.
        */
        Curve2D getSubcurve( std::size_t index_ ) const
        {
            bool valid_ = !curve.empty();
            valid_ &= ( ( index_ < curve.size() ) && ( index_ >= 0 ) );

            if( valid_ == false )
                return Curve2D();

            return curve.at( index_ );
        }


        /**
        * Method to check if the curve is empty
        * @return boolean it returns true if there is no segment or if the size of the segment is less than the minimum allowed.
        */
        bool isEmpty() const
        {
            if( curve.empty() == true ) return true;

            std::size_t size_ = 0;
            for( auto c_: curve )
            {
                size_ += c_.size();
            }

            if( size_ <= CURVE_MIN_LENGHT )
                return true;

            return false;
        }


        /**
        * Method to remove all the segments from the curve
        * @return void.
        */
        void clear()
        {
            for( auto c_: curve )
            {
                c_.clear();
            }
            curve.clear();
        }


        /**
        * Method to create a curve given a vector of vertices, which coordinates are represented by doubles, and edges
        * @param vs the vector of vertices coordinates in double
        * @param es the vector of edges
        * @return void.
        */
        void fromVector( const std::vector< double >& vs_, const std::vector< std::size_t >& es_ = std::vector< std::size_t >() ){

            std::size_t number_of_edges_ = es_.size()/2;
            std::size_t last_id_ = UINT_MAX;


            bool status_ = ( vs_.empty() || ( vs_.size() < 2 ) );


            if( status_ == true ) return;

            if( es_.empty() == true )
            {
                curve.push_back( Curve2D() );
                std::size_t number_of_points_ = vs_.size()/2;
                for( std::size_t i = 0; i < number_of_points_; ++i )
                {
                    double x0_ = vs_[ 2*i ];
                    double y0_ = vs_[ 2*i + 1 ];

                    curve[ 0 ].add( Point2D( x0_, y0_ ) );
                }

                return;
            }


            int id_ = -1;
    //        curve.push_back( Curve2D() );

            for( std::size_t i = 0; i < number_of_edges_; ++i )
            {

                std::size_t id0_ = es_[ 2*i ];
                std::size_t id1_ = es_[ 2*i + 1 ];

                double x0 = vs_[ 2*id0_ ];
                double y0 = vs_[ 2*id0_ + 1 ];
                double x1 = vs_[ 2*id1_ ];
                double y1 = vs_[ 2*id1_ + 1 ];

                if( last_id_ != id0_ )
                {
                    curve.push_back( Curve2D() );
                    id_++;
                }

                curve[ id_ ].add( Point2D( x0, y0 ) );
                curve[ id_ ].add( Point2D( x1, y1 ) );

                last_id_ = id1_;

            }


        }


        /**
        * Method to create a curve given a vector of vertices, which coordinates are represented by float, and edges
        * @param vs the vector of vertices coordinates in float
        * @param es the vector of edges
        * @return void.
        */
        void fromVector( const std::vector< float >& vs_, const std::vector< std::size_t >& es_ = std::vector< std::size_t >() ){

            std::size_t number_of_edges_ = es_.size()/2;
            std::size_t last_id_ = UINT_MAX;


            bool status_ = ( vs_.empty() || ( vs_.size() < 2 ) );


            if( status_ == true ) return;

            if( es_.empty() == true )
            {
                curve.push_back( Curve2D() );
                std::size_t number_of_points_ = vs_.size()/2;
                for( std::size_t i = 0; i < number_of_points_; ++i )
                {
                    double x0_ = static_cast< double >( vs_[ 2*i ] );
                    double y0_ = static_cast< double >( vs_[ 2*i + 1 ] );

                    curve[ 0 ].add( Point2D( x0_, y0_ ) );
                }

                return;
            }


            int id_ = -1;
    //        curve.push_back( Curve2D() );

            for( std::size_t i = 0; i < number_of_edges_; ++i )
            {

                std::size_t id0_ = es_[ 2*i ];
                std::size_t id1_ = es_[ 2*i + 1 ];

                double x0 = static_cast< double >( vs_[ 2*id0_ ] );
                double y0 = static_cast< double >( vs_[ 2*id0_ + 1 ] );
                double x1 = static_cast< double >( vs_[ 2*id1_ ] );
                double y1 = static_cast< double >( vs_[ 2*id1_ + 1 ] );

                if( last_id_ != id0_ )
                {
                    curve.push_back( Curve2D() );
                    id_++;
                }

                curve[ id_ ].add( Point2D( x0, y0 ) );
                curve[ id_ ].add( Point2D( x1, y1 ) );

                last_id_ = id1_;

            }


        }


        /**
        * Method to return a vector of coordinates (double) and edges from a Polycurve
        * @param vs a reference to the vector of vertices coordinates in double
        * @param es a reference to the vector of edges
        * @return void.
        */
        void toVector( std::vector< double >& vs_, std::vector< std::size_t >& es_ ) const{
            int index_ = 0;
            for( auto c_: curve )
            {
                if( c_.size() < CURVE_MIN_LENGHT ) continue;

                for( unsigned int i = 0; i < c_.size(); ++i )
                {
                    Point2D p_ = c_.at( i );

                    vs_.push_back( p_.x() );
                    vs_.push_back( p_.y() );

                    if( ( i + 1 ) == c_.size() )
                    {
                        es_.push_back( index_ );
                        es_.push_back( ++index_ );
                    }

                }

            }
        }


        /**
        * Method to return the number of segments existent in the curve
        * @return std::size_t the number of segments existent in the curve
        */
        std::size_t getNumberOfSegments() const
        {
            return curve.size();
        }


        /**
        * Method to return the vector of segments in Curve2D
        * @see Curve2D
        * @return std::vector< Curve2D > the vector of segments in Curve2D
        */
        const std::vector< Curve2D >& getCurves2D() const { return curve; }


        /**
        * Method to return the vertices vector of the curve
        * @return std::vector< double > the vertices vector of the curve
        */
        std::vector< double > getPoints() const
        {
            std::vector< double > points_;

            for( Curve2D c_: curve )
            {
                int number_of_points_ = c_.size();
                for( int i = 0; i < number_of_points_; ++i )
                {
                    points_.push_back( c_[i].x() );
                    points_.push_back( c_[i].y() );
                }

            }

            return points_;
        }


        /**
        * Method to return the vertices vector of the curve, which 2d coordinates were swapped
        * @return std::vector< double > the vertices vector of the curve, which 2d coordinates were swapped
        */
        std::vector< double > getPointsSwapped() const
        {
            std::vector< double > points_ = getPoints();

            std::size_t number_ = points_.size()/2;
            for( std::size_t i = 0; i < number_; ++i )
            {
                std::swap( points_[ 2*i ], points_[ 2*i + 1 ]);
            }
            return points_;

        }


        /**
        * Method to get a curve 3d adding a fixed X coordinate to the vertices
        * @param depth the fixed X coordinate
        * @param swap if true the coordinates should be swapped before adding the X coordinate
        * @return std::vector< double > the vector of 3d coordinates adding a X coordinate to the vector of vertices existent
        */
        std::vector< double > addXCoordinate( double depth_, bool swap_ = false ) const
        {
            std::vector< double > points_;
            if( swap_ == true )
                points_ = getPointsSwapped();
            else
                points_ = getPoints();

            std::vector< double > points3d_;

            std::size_t number_ = points_.size()/2;
            for( auto i = 0; i < number_; ++i )
            {
                points3d_.push_back( depth_ );
                points3d_.push_back( points_[ 2*i ] );
                points3d_.push_back( points_[ 2*i + 1 ] );
            }
            return points3d_;
        }


        /**
        * Method to get a curve 3d adding a fixed Y coordinate to the vertices
        * @param depth the fixed Y coordinate
        * @param swap if true the coordinates should be swapped before adding the Y coordinate
        * @return std::vector< double > the vector of 3d coordinates adding a Y coordinate to the vector of vertices existent
        */
        std::vector< double > addYCoordinate( double depth_, bool swap_ = false ) const
        {
            std::vector< double > points3d_;
            std::vector< double > points_;
            if( swap_ == true )
                points_ = getPointsSwapped();
            else
                points_ = getPoints();

            std::size_t number_ = points_.size()/2;
            for( auto i = 0; i < number_; ++i )
            {
                points3d_.push_back( points_[ 2*i ] );
                points3d_.push_back( depth_ );
                points3d_.push_back( points_[ 2*i + 1 ] );
            }

            return points3d_;

        }


        /**
        * Method to get a curve 3d adding a fixed Z coordinate to the vertices
        * @param depth the fixed Z coordinate
        * @param swap if true the coordinates should be swapped before adding the Z coordinate
        * @return std::vector< double > the vector of 3d coordinates adding a Z coordinate to the vector of vertices existent
        */
        std::vector< double > addZCoordinate( double depth_, bool swap_ = false ) const
        {
            std::vector< double > points3d_;
            std::vector< double > points_;

            if( swap_ == true )
                points_ = getPointsSwapped();
            else
                points_ = getPoints();

            std::size_t number_ = points_.size()/2;
            for( auto i = 0; i < number_; ++i )
            {
                points3d_.push_back( points_[ 2*i ] );
                points3d_.push_back( points_[ 2*i + 1 ] );
                points3d_.push_back( depth_ );

            }

            return points3d_;
        }


    protected:

        std::size_t CURVE_MIN_LENGHT = 3;                                   /**< The minimum number of points that a curve should have to be considered valid */

        std::vector< Curve2D > curve;                                       /**< A vector of subcurves, i.e. Curve2D, to represent a curve */


};

#endif // POLYCURVE_H
