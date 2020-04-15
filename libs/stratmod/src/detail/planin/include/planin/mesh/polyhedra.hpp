/******************************************************************************/
/*                                                                            */
/* This file is part of the "Planar Interpolation Library" (PlanInLib)        */
/* Copyright (C) 2016, Julio Daniel Machado Silva.                            */
/*                                                                            */
/* PlanInLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 3 of the License, or (at your option) any later version.           */
/*                                                                            */
/* PlanInLib is distributed in the hope that it will be useful,               */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          */
/* Lesser General Public License for more details.                            */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public           */
/* License along with PlanInLib.  If not, see <http://www.gnu.org/licenses/>, */
/* or write to the Free Software Foundation, Inc., 51 Franklin Street,        */
/* Fifth Floor, Boston, MA  02110-1301  USA.                                  */
/*                                                                            */
/******************************************************************************/


#ifndef PLANIN_POLYHEDRA_HPP
#define PLANIN_POLYHEDRA_HPP

#include "../core.hpp"

#include <array>
#include <algorithm>
#include <numeric>


struct EdgeHeights
{
    // BUG: VS2013 does not support empty initializer lists
    std::array<double, 2> vertex_height; //= {};
    std::array<bool, 2> vertex_status; //= {};
    double tolerance = 1E-13;

    EdgeHeights() = default;
    ~EdgeHeights() = default;
    EdgeHeights( const EdgeHeights & ) = default;

    EdgeHeights& operator=( const EdgeHeights &rhs ) 
    {
        vertex_height = rhs.vertex_height;
        vertex_status = rhs.vertex_status;
        tolerance = rhs.tolerance;

        return *this;
    }

    EdgeHeights& operator-()
    {
        vertex_height[0] = -vertex_height[0];
        vertex_height[1] = -vertex_height[1];

        return *this;
    }

    bool isValid() const
    {
        return vertex_status[0] || vertex_status[1];
    }

    bool setVertex( size_t position, double vheight, bool vstatus )
    {
        if ( position > 1 )
        {
            return false;
        }

        vertex_height[position] = vheight;
        vertex_status[position] = vstatus;

        return true;
    }

    bool operator<( const EdgeHeights &rhs ) const
    {
        if ( vertex_height[0] - tolerance > rhs.vertex_height[0] )
        {
            return false;
        }

        if ( vertex_height[1] - tolerance > rhs.vertex_height[1] )
        {
            return false;
        }

        bool equal_v0 = std::fabs( vertex_height[0] - rhs.vertex_height[0] ) < tolerance;
        bool equal_v1 = std::fabs( vertex_height[1] - rhs.vertex_height[1] ) < tolerance;

        // Given equal heights a valid triangle can't be < than an invalid triangle
        // Also, enforce that x < x is false
        //
        if ( equal_v0 && equal_v1 )
        {
            if ( rhs.isValid() == false )
            {
                return false;
            }

            if ( isValid() && rhs.isValid() )
            {
                return false;
            }
        }

        return true;
    }

    bool operator>( const EdgeHeights &rhs ) const
    {
        return rhs.operator<(*this);
    }

    bool operator>=( const EdgeHeights &rhs ) const
    {
        return !( operator<(rhs) );
    }

    bool operator<=( const EdgeHeights &rhs ) const
    {
        return rhs.operator>=(*this);
    }

    bool operator==( const EdgeHeights &rhs ) const
    {
        return !( operator<(rhs) ) && !( rhs.operator<(*this) );
    }
};

struct TriangleHeights
{
    // BUG: VS2013 does not support empty initializer lists
    std::array<double, 3> vertex_height; //= {};
    std::array<bool, 3> vertex_status; //= {};
    double tolerance = 1E-13;

    TriangleHeights() = default;
    ~TriangleHeights() = default;
    TriangleHeights( const TriangleHeights & ) = default;

    TriangleHeights& operator=( const TriangleHeights &rhs ) 
    {
        vertex_height = rhs.vertex_height;
        vertex_status = rhs.vertex_status;
        tolerance = rhs.tolerance;

        return *this;
    }

    TriangleHeights& operator-()
    {
        vertex_height[0] = -vertex_height[0];
        vertex_height[1] = -vertex_height[1];
        vertex_height[2] = -vertex_height[2];

        return *this;
    }

    bool isValid() const
    {
        return vertex_status[0] || vertex_status[1] || vertex_status[2];
    }

    bool setVertex( size_t position, double vheight, bool vstatus )
    {
        if ( position > 2 )
        {
            return false;
        }

        vertex_height[position] = vheight;
        vertex_status[position] = vstatus;

        return true;
    }

    bool operator<( const TriangleHeights &rhs ) const
    {
        if ( vertex_height[0] - tolerance > rhs.vertex_height[0] )
        {
            return false;
        }

        if ( vertex_height[1] - tolerance > rhs.vertex_height[1] )
        {
            return false;
        }

        if ( vertex_height[2] - tolerance > rhs.vertex_height[2] )
        {
            return false;
        }

        bool equal_v0 = std::fabs( vertex_height[0] - rhs.vertex_height[0] ) < tolerance;
        bool equal_v1 = std::fabs( vertex_height[1] - rhs.vertex_height[1] ) < tolerance;
        bool equal_v2 = std::fabs( vertex_height[2] - rhs.vertex_height[2] ) < tolerance;

        // Given equal heights a valid triangle can't be < than an invalid triangle
        // Also, enforce that x < x is false
        //
        if ( equal_v0 && equal_v1 && equal_v2 )
        {
            if ( rhs.isValid() == false )
            {
                return false;
            }

            if ( isValid() && rhs.isValid() )
            {
                return false;
            }
        }

        return true;
    }

    bool operator>( const TriangleHeights &rhs ) const
    {
        return rhs.operator<(*this);
    }

    bool operator>=( const TriangleHeights &rhs ) const
    {
        return !( operator<(rhs) );
    }

    bool operator<=( const TriangleHeights &rhs ) const
    {
        return rhs.operator>=(*this);
    }

    bool operator==( const TriangleHeights &rhs ) const
    {
        /* return !( operator<(rhs) ) && !( rhs.operator<(*this) ); */
        return operator<=(rhs) && operator>=(rhs);
    }

    bool operator!=( const TriangleHeights &rhs ) const
    {
        return !operator==(rhs);
    }
};

template<typename __AttributeType__ = int>
class Triangle
{
    public:
        using AttributeType = __AttributeType__;

        Triangle() = default;
        ~Triangle() = default;

        Triangle( const Triangle& ) = default;
        Triangle& operator=( const Triangle& ) = default;

        /* Triangle( Triangle&& ) = default; */
        /* Triangle& operator=( Triangle&& ) = default; */

        Triangle(Point3 v0, size_t i0, Point3 v1, size_t i1, Point3 v2, size_t i2 )
        {
            vertices_[0] = v0;
            vindices_[0] = i0;
            vertex_is_set_[0] = true;

            vertices_[1] = v1;
            vindices_[1] = i1;
            vertex_is_set_[1] = true;

            vertices_[2] = v2;
            vindices_[2] = i2;
            vertex_is_set_[2] = true;
        }

        template<typename _Point3_>
        void setVertex( size_t position, _Point3_ &&vertex_coordinates, size_t vindex, bool vstatus = true )
        {
            vertices_[position] = static_cast<Point3>(vertex_coordinates);
            vindices_[position] = vindex;
            vstatus_[position] = vstatus;
            vertex_is_set_[position] = true;
        }

        std::vector<size_t> getConnectivity() const
        {
            std::vector<size_t> vindices; //= {{vindices_[0], vindices_[1], vindices_[2]}};
            vindices.push_back(vindices_[0]);
            vindices.push_back(vindices_[1]);
            vindices.push_back(vindices_[2]);

            return vindices;
        }

        bool isValid() const
        {
            bool is_valid_ = verticesAreSet() && std::any_of( vstatus_.begin(), vstatus_.end(), []( bool b ) -> bool { return b; } ) ;

            return is_valid_ ; 
        }

        Point3 getCentroid() const
        {
            Point3 c{};
            const double numv = static_cast<double>(num_vertices_);

            if ( isValid() )
            {
                for ( size_t i = 0; i < num_vertices_; ++i )
                {
                    c.x += vertices_[i].x/numv;
                    c.y += vertices_[i].y/numv;
                    c.z += vertices_[i].z/numv;
                }

            }

            return c;
        }

        double getVolume() const
        {
            double vol = 0;

            if ( isValid() )
            {
                double a11, a12, a21, a22;

                a11 = vertices_[0].x - vertices_[2].x;
                a12 = vertices_[1].x - vertices_[2].x;

                a21 = vertices_[0].y - vertices_[2].y;
                a22 = vertices_[1].y - vertices_[2].y;

                vol = (a11*a22 - a12*a21)/2; 
            }

            return vol;
        }

    private:
        // BUG: VS2013 does not support constexpr
        // static constexpr size_t num_vertices_ = 3;
        
        const size_t num_vertices_ = 3;

        /* std::array<Point3, num_vertices_>  vertices_ = {}; */ 
        /* std::array<size_t, num_vertices_>  vindices_ = {}; */
        /* std::array<bool, num_vertices_> vstatus_ = {{false, false, false}}; */

        /* std::array<bool, num_vertices_> vertex_is_set_ = {{false, false, false}}; */

        // BUG: VS2013 does not support empty initializer lists
        std::array<Point3, 3>  vertices_; //= {}; 
        std::array<size_t, 3>  vindices_; //= {};
        std::array<bool, 3> vstatus_; //= {{false, false, false}};

        std::array<bool, 3> vertex_is_set_; //= {{false, false, false}};

        AttributeType attribute_;

        bool verticesAreSet() const
        {
            bool vertices_are_set_ = std::all_of( vertex_is_set_.begin(), vertex_is_set_.end(), []( bool b ) -> bool { return b; } );

            return vertices_are_set_;
        }
};

template<typename __AttributeType__ = int>
class Tetrahedron
{
    public:
        using AttributeType = __AttributeType__;

        Tetrahedron() = default;
        ~Tetrahedron() = default;

        Tetrahedron( const Tetrahedron& ) = default;
        Tetrahedron& operator=( const Tetrahedron& ) = default;

        /* Tetrahedron( Tetrahedron&& ) = default; */
        /* Tetrahedron& operator=( Tetrahedron&& ) = default; */

        Tetrahedron(Point3 v0, size_t i0, Point3 v1, size_t i1, Point3 v2, size_t i2, Point3 v3, size_t i3 )
        {
            vertices_[0] = v0;
            vindices_[0] = i0;
            vertex_is_set_[0] = true;

            vertices_[1] = v1;
            vindices_[1] = i1;
            vertex_is_set_[1] = true;

            vertices_[2] = v2;
            vindices_[2] = i2;
            vertex_is_set_[2] = true;

            vertices_[3] = v3;
            vindices_[3] = i3;
            vertex_is_set_[3] = true;
        }

        template<typename _Point3_>
        void setVertex( size_t position, size_t vindex, _Point3_ &&vertex_coordinates, bool vstatus = true )
        {
            vindices_[position] = vindex;
            vertices_[position] = static_cast<Point3>(vertex_coordinates);
            vstatus_[position] = vstatus;
            vertex_is_set_[position] = true;
        }

        std::vector<size_t> getConnectivity() const
        {
            std::vector<size_t> vindices; //= {{vindices_[0], vindices_[1], vindices_[2], vindices_[3]}};
            vindices.push_back(vindices_[0]);
            vindices.push_back(vindices_[1]);
            vindices.push_back(vindices_[2]);
            vindices.push_back(vindices_[3]);


            return vindices;
        }

        bool isValid() const
        {
            bool is_valid = verticesAreSet() && std::any_of( vstatus_.begin(), vstatus_.end(), []( bool b ) -> bool { return b; } ) ;

            return is_valid;
        }

        Point3 getCentroid() const
        {
            Point3 c{};
            const double numv = static_cast<double>(num_vertices_);

            /* std::cout << "Vertex0: x = " << vertices_[0].x << ", " << " y = " << vertices_[0].y << ", z = " << vertices_[0].z << "\n"; */
            /* std::cout << "Vertex1: x = " << vertices_[1].x << ", " << " y = " << vertices_[1].y << ", z = " << vertices_[1].z << "\n"; */
            /* std::cout << "Vertex2: x = " << vertices_[2].x << ", " << " y = " << vertices_[2].y << ", z = " << vertices_[2].z << "\n"; */
            /* std::cout << "Vertex3: x = " << vertices_[3].x << ", " << " y = " << vertices_[3].y << ", z = " << vertices_[3].z << "\n"; */
            if ( verticesAreSet() )
            {
                for ( size_t i = 0; i < num_vertices_; ++i )
                {
                    c.x += vertices_[i].x/numv;
                    c.y += vertices_[i].y/numv;
                    c.z += vertices_[i].z/numv;
                }

            }

            /* std::cout << "Centroid: x = " << c.x << ", " << " y = " << c.y << ", z = " << c.z << "\n"; */

            return c;
        }

        double getVolume() const
        {
            double vol = 0;

            if ( isValid() )
            {
                double a11, a12, a13, a21, a22, a23, a31, a32, a33;

                a11 = vertices_[0].x - vertices_[3].x;
                a12 = vertices_[1].x - vertices_[3].x;
                a13 = vertices_[2].x - vertices_[3].x;

                a21 = vertices_[0].y - vertices_[3].y;
                a22 = vertices_[1].y - vertices_[3].y;
                a23 = vertices_[2].y - vertices_[3].y;

                a31 = vertices_[0].z - vertices_[3].z;
                a32 = vertices_[1].z - vertices_[3].z;
                a33 = vertices_[2].z - vertices_[3].z;

                // TODO: Had to multiply vol by -1, check tet orientation
                vol = -(a11*a22*a33 + a12*a23*a31 + a13*a21*a32) +
                    (a31*a22*a13 + a32*a23*a11 + a33*a21*a12);

                vol /= 6;
            }

            return vol;
        }

        void setAttribute( AttributeType attribute )
        {
            region_attribute_ = std::move(attribute);
        }

        AttributeType getAttribute() const
        {
            return region_attribute_;
        }

    private:
        // BUG: VS2013 does not support constexpr
        // static constexpr size_t num_vertices_ = 4;
        const size_t num_vertices_ = 4;

        /* std::array<Point3, num_vertices_>  vertices_ = {}; */ 
        /* std::array<size_t, num_vertices_>  vindices_ = {}; */
        /* std::array<bool, num_vertices_> vstatus_ = {{false, false, false, false}}; */

        /* std::array<bool, num_vertices_> vertex_is_set_ = {{false, false, false, false}}; */

        // BUG: VS2013 does not support empty initializer lists
        std::array<Point3, 4>  vertices_; //= {}; 
        std::array<size_t, 4>  vindices_; //= {};
        std::array<bool, 4> vstatus_; //= {{false, false, false, false}};

        std::array<bool, 4> vertex_is_set_; //= {{false, false, false, false}};

        // BUG: VS2013 does not support empty initializer lists
        AttributeType region_attribute_; //= {};
        
        bool verticesAreSet() const
        {
            bool vertices_are_set_ = std::all_of( vertex_is_set_.begin(), vertex_is_set_.end(), []( bool b ) -> bool { return b; } );

            return vertices_are_set_;
        }
};

template<typename __AttributeType__ = int>
class Prism 
{
    using Tetrahedron = ::Tetrahedron<__AttributeType__>;

    public:
        using AttributeType = __AttributeType__;

        template<typename _Point3_>
        void setVertex( size_t position, _Point3_ &&vertex_coordinates, size_t vindex, bool vstatus )
        {
            vertices_[position] = static_cast<Point3>(vertex_coordinates);
            vindices_[position] = vindex;
            vstatus_[position] = vstatus;
            vertex_is_set_[position] = true;
        }

        std::vector<Tetrahedron> tetrahedralize() const
        {
            std::vector<Tetrahedron> tetrahedra; //= {};

            if ( isValid() )
            {
                if ( std::min(vindices_[2-1], vindices_[6-1]) < std::min(vindices_[3-1], vindices_[6-1] ) )
                {
                    if ( vstatus_[2] || vstatus_[5] )
                    {
                        tetrahedra.emplace_back( Tetrahedron( vertices_[0], vindices_[1-1], vertices_[1], vindices_[2-1], vertices_[2], vindices_[3-1], vertices_[5], vindices_[6-1] ) );
                        tetrahedra.back().setAttribute( getAttribute() );
                    }

                    if ( vstatus_[1] || vstatus_[4] )
                    {
                        tetrahedra.emplace_back( Tetrahedron( vertices_[0], vindices_[1-1], vertices_[1], vindices_[2-1], vertices_[5], vindices_[6-1], vertices_[4], vindices_[5-1] ) );
                        tetrahedra.back().setAttribute( getAttribute() );
                    }

                    if ( vstatus_[0] || vstatus_[3] )
                    {
                        tetrahedra.emplace_back( Tetrahedron( vertices_[0], vindices_[1-1], vertices_[4], vindices_[5-1], vertices_[5], vindices_[6-1], vertices_[3], vindices_[4-1] ) );
                        tetrahedra.back().setAttribute( getAttribute() );
                    }
                }

                else
                {
                    if ( vstatus_[1] || vstatus_[4] )
                    {
                        tetrahedra.emplace_back( Tetrahedron( vertices_[0], vindices_[1-1], vertices_[1], vindices_[2-1], vertices_[2], vindices_[3-1], vertices_[4], vindices_[5-1] ) );
                        tetrahedra.back().setAttribute( getAttribute() );
                    }

                    if ( vstatus_[2] || vstatus_[5] )
                    {
                        tetrahedra.emplace_back( Tetrahedron( vertices_[0], vindices_[1-1], vertices_[4], vindices_[5-1], vertices_[2], vindices_[3-1], vertices_[5], vindices_[6-1] ) );
                        tetrahedra.back().setAttribute( getAttribute() );
                    }

                    if ( vstatus_[0] || vstatus_[3] )
                    {
                        tetrahedra.emplace_back( Tetrahedron( vertices_[0], vindices_[1-1], vertices_[4], vindices_[5-1], vertices_[5], vindices_[6-1], vertices_[3], vindices_[4-1] ) );
                        tetrahedra.back().setAttribute( getAttribute() );
                    }
                }
            }

            return tetrahedra;
        }
        
        bool verticesAreSet() const
        {
            bool vertices_are_set_ = std::all_of( vertex_is_set_.begin(), vertex_is_set_.end(), []( bool b ) -> bool { return b; } );

            return vertices_are_set_;
        }

        bool isValid() const
        {
            return verticesAreSet() && std::any_of( vstatus_.begin(), vstatus_.end(), []( bool b ) -> bool { return b; } );
        }

        Point3 getCentroid() const
        {
            Point3 c{};
            const double numv = static_cast<double>(num_vertices_);

            if ( isValid() )
            {
                for ( size_t i = 0; i < num_vertices_; ++i )
                {
                    c.x += vertices_[i].x/numv;
                    c.y += vertices_[i].y/numv;
                    c.z += vertices_[i].z/numv;
                }

            }

            return c;
        }

        void setAttribute( AttributeType attribute )
        {
            region_attribute_ = std::move(attribute);
        }

        AttributeType getAttribute() const
        {
            return region_attribute_;
        }

    private:
        // BUG: VS2013 does not support constexpr
        // static constexpr size_t num_vertices_ = 6;

        const size_t num_vertices_ = 6;

        /* std::array<Point3, num_vertices_>  vertices_ = {}; */ 
        /* std::array<size_t, num_vertices_>  vindices_ = {}; */
        /* std::array<bool, num_vertices_> vstatus_ = {{false, false, false, false, false, false}}; */

        /* std::array<bool, num_vertices_> vertex_is_set_ = {{false, false, false, false, false, false}}; */

        // BUG: VS2013 does not support empty initializer lists
        std::array<Point3, 6>  vertices_; //= {}; 
        std::array<size_t, 6>  vindices_; //= {};
        std::array<bool, 6> vstatus_; //= {{false, false, false, false, false, false}};

        std::array<bool, 6> vertex_is_set_; //= {{false, false, false, false, false, false}};
        // BUG: VS2013 does not support empty initializer lists
        AttributeType region_attribute_; //= {};
};

#endif
