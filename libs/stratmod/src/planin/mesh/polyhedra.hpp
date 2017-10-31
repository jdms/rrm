#ifndef __POLYHEDRA_HPP__
#define __POLYHEDRA_HPP__

#include "../core.hpp"
#include <array>
#include <algorithm>

class Triangle
{
    public:
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
            std::vector<size_t> vindices = {{vindices_[0], vindices_[1], vindices_[2]}};

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
        std::array<bool, 3> vstatus_ = {{false, false, false}};

        std::array<bool, 3> vertex_is_set_ = {{false, false, false}};

        bool verticesAreSet() const
        {
            bool vertices_are_set_ = std::all_of( vertex_is_set_.begin(), vertex_is_set_.end(), []( bool b ) -> bool { return b; } );

            return vertices_are_set_;
        }
};

class Tetrahedron
{
    public:
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
            std::vector<size_t> vindices = {{vindices_[0], vindices_[1], vindices_[2], vindices_[3]}};

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

                vol = (a11*a22*a33 + a12*a23*a31 + a13*a21*a32) -
                    (a31*a22*a13 + a32*a23*a11 + a33*a21*a12);

                vol /= 6;
            }

            return vol;
        }

        void setAttribute( std::vector<bool> attribute )
        {
            region_attribute_ = std::move(attribute);
        }

        std::vector<bool> getAttribute() const
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
        std::array<bool, 4> vstatus_ = {{false, false, false, false}};

        std::array<bool, 4> vertex_is_set_ = {{false, false, false, false}};

        // BUG: VS2013 does not support empty initializer lists
        std::vector<bool> region_attribute_; //= {};
        
        bool verticesAreSet() const
        {
            bool vertices_are_set_ = std::all_of( vertex_is_set_.begin(), vertex_is_set_.end(), []( bool b ) -> bool { return b; } );

            return vertices_are_set_;
        }
};

class Prism 
{
    public:
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
            std::vector<Tetrahedron> tetrahedra = {};

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

        void setAttribute( std::vector<bool> attribute )
        {
            region_attribute_ = std::move(attribute);
        }

        std::vector<bool> getAttribute() const
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
        std::array<bool, 6> vstatus_ = {{false, false, false, false, false, false}};

        std::array<bool, 6> vertex_is_set_ = {{false, false, false, false, false, false}};
        // BUG: VS2013 does not support empty initializer lists
        std::vector<bool> region_attribute_; //= {};
        
        bool verticesAreSet() const
        {
            bool vertices_are_set_ = std::all_of( vertex_is_set_.begin(), vertex_is_set_.end(), []( bool b ) -> bool { return b; } );

            return vertices_are_set_;
        }
};

#endif
