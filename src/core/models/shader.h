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



#ifndef SHADER_H
#define SHADER_H

#include <QVector>

#include <iostream>
#include <string>
#include <vector>

#include "./core/definitions/constants.hpp"
//#include "./apps/simulator/colormap.h"
#include <Tucano/Shader.hpp>


class Shader
{
    public:

        Shader() = default;

        void setDirectory( const std::string& path_ )
        {
            shader_directory.clear();
            shader_directory = path_;
        }
        std::string getDirectory() const
        {
            return shader_directory;
        }

        void init()
        {
            initShaders();
            initBuffers();
        }
        void reset()
        {
            resetShaders();
            resetBuffers();
        }
        void reload()
        {
            shader->reloadShaders();
        }

        static std::vector< float > normalize( const std::vector< float >& points, const Eigen::Vector3f& M, const Eigen::Vector3f& m, int n )
        {

            if( points.empty() == true ) return points;


            std::size_t number_of_points = points.size()/n;

            std::vector< float > points_in_box;
            for( std::size_t i = 0; i < number_of_points; ++i )
            {
                Eigen::Vector3f p ( points[ n*i ], points[ n*i + 1 ], points[ n*i + 2 ] );
                Eigen::Vector3f pn = normalize( p, M, m );

                points_in_box.push_back( pn.x() );
                points_in_box.push_back( pn.y() );
                points_in_box.push_back( pn.z() );

                if( n == 4 )
                    points_in_box.push_back( 1.0f );
            }

            return points_in_box;
        }


        static Eigen::Vector3f normalize( const Eigen::Vector3f& p, const Eigen::Vector3f& M, const Eigen::Vector3f& m )
        {

            float dimx = M.x() - m.x();
            float dimy = M.y() - m.y();
            float dimz = M.z() - m.z();

            float scale = std::max( std::max( dimx, dimy ), dimz );


            Eigen::Vector3f center = Eigen::Vector3f( ( M.x() + m.x() )*0.5f,( M.y() + m.y() )*0.5f,( M.z() + m.z() )*0.5f );
            Eigen::Vector3f cpy( p.x(), p.y(), p.z() );

            cpy -= center;
            cpy /= scale;


            return cpy;
        }


        static std::vector< float > convertToFloat( const std::vector< double >& points_ )
        {
            std::vector< float > points;
            for( auto v: points_ )
                points.push_back( static_cast< float >( v ) );
            return points;
        }


        static std::vector< unsigned int > convertToUnsignedInt( const std::vector< std::size_t >& edges_ )
        {
            std::vector< unsigned int > edges;
            for( auto e: edges_ )
                edges.push_back( static_cast< unsigned int >( e ) );
            return edges;
        }


        static std::vector< float > getDefaultNormals( std::size_t number_of_vertices_ )
        {
            std::size_t number_of_points = number_of_vertices_/3;

            std::vector< float > normals_;
            for( std::size_t i = 0; i < number_of_points; ++i )
            {
                normals_.push_back( 0.0 );
                normals_.push_back( 1.0 );
                normals_.push_back( 0.0 );
            }

            return normals_;
        }


        static std::vector< float > getHeightMapColor(  double zmin_, double zmax_, std::vector< float > values )
        {
//            ColorMap colormap;
            std::vector< float > colors_;
//            for( double v: values )
//            {
//                QVector3D color_ = colormap.getColor( ColorMap::COLORMAP::COOL_TO_WARM, v, zmin_, zmax_ );
//                colors_.push_back( color_.x() );
//                colors_.push_back( color_.y() );
//                colors_.push_back( color_.z() );
//            }

            return colors_;
        }


    protected:

        virtual void initShaders() = 0;
        virtual void initBuffers() = 0;

        virtual void resetShaders() = 0;
        virtual void resetBuffers() = 0;

        std::string shader_directory;
        Tucano::Shader* shader;



};

#endif // SHADER_H
