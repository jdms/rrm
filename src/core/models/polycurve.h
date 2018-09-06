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



#ifndef POLYCURVE_H
#define POLYCURVE_H

#include <vector>

#include "./polygonalcurve/CurveN.hpp"

class PolyCurve
{

public:

    PolyCurve() = default;


    PolyCurve( const std::vector< Curve2D >& curve_ ){
        curve = curve_;
      }


    PolyCurve( const Curve2D& curve_ ){
        curve.clear();
        curve.push_back( curve_ );
      }


    PolyCurve( const std::vector< double >& vs_, const std::vector< std::size_t >& es_ = std::vector< std::size_t >() ){
        fromVector( vs_, es_ );
    }

    PolyCurve( const std::vector< float >& vs_, const std::vector< std::size_t >& es_ = std::vector< std::size_t >() ){
        fromVector( vs_, es_ );
    }


    Curve2D getSubcurve( std::size_t index_ ) const{
        bool valid_ = !curve.empty();
        valid_ &= ( ( index_ < curve.size() ) && ( index_ >= 0 ) );

        if( valid_ == false )
            return Curve2D();

        return curve.at( index_ );
    }


    bool isEmpty() const{
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


    void clear(){
        for( auto c_: curve )
        {
            c_.clear();
        }
        curve.clear();
    }


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


    std::size_t getNumberOfSegments() const
    {
        return curve.size();
    }


    const std::vector< Curve2D >& getCurves2D() const { return curve; }


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

    std::size_t CURVE_MIN_LENGHT = 3;
    std::vector< Curve2D > curve;


};

#endif // POLYCURVE_H
