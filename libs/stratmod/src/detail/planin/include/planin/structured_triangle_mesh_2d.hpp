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


#ifndef PLANIN_STRUCTURED_TRIANGLE_SURFACE_2D_HPP
#define PLANIN_STRUCTURED_TRIANGLE_SURFACE_2D_HPP


#include <array>
#include <cmath>
#include <tuple>
#include <vector> 
#include <iostream>

/* #include "predicates.h" */
#include "planin/predicates.hpp"

#include <Eigen/Dense>

template<typename Point2D = std::array<double, 2>>
class StructuredTriangleMesh2D {
    public:
        using Natural = unsigned long int; 
        using NaturalPair = std::array<Natural, 2>; 
        using Triangle = std::array<Natural, 3>;
        using BaricentricCoordinates = std::array<double, 3>;

        /* StructuredTriangleMesh2D(); */
        /* ~StructuredTriangleMesh2D() = default; */
        /* StructuredTriangleMesh2D(StructuredTriangleMesh2D&) = default; */
        /* StructuredTriangleMesh2D& operator=(StructuredTriangleMesh2D&) = default; */
        /* StructuredTriangleMesh2D(StructuredTriangleMesh2D&&) = default; */
        /* StructuredTriangleMesh2D& operator=(StructuredTriangleMesh2D&&) = default; */

        void setDomainOrigin(const Point2D& origin);
        bool setDomainLength(const Point2D& length);
        virtual bool setDiscretization(const NaturalPair& discretization);
        virtual bool setDiscretization(Natural discretization_x, Natural discretization_y);

        Point2D origin() const;
        Point2D length() const;
        NaturalPair discretization() const;

        Natural numBlocks() const;
        Natural numVertices() const;
        Natural numTriangles() const;

        bool liesInDomain(const Point2D& point) const;
        Point2D projectOntoDomain(const Point2D& point) const;

        bool blockIndex(const Point2D& point, Natural &block_index) const;
        bool triangleIndex(const Point2D& point, Natural &triangle_index) const;

        bool vertex(const Natural vertex_index, Point2D& vertex) const;
        bool triangle(const Natural triangle_index, Triangle& triangle) const;

        Natural blockIndex(const Point2D& point) const;
        Natural triangleIndex(const Point2D& point) const;

        std::tuple<Natural, BaricentricCoordinates> baricentricCoordinates(const Point2D& p) const;

        template<typename FList>
        unsigned int triangleList( FList &flist ); 

    protected:
        bool getVertexIndices( Natural v, NaturalPair &indices ) const; 

        Natural numTrianglesPerBlock() const;

        Natural getVertexIndex( Natural i, Natural j ) const;

        Natural getBlockIndex( Natural i, Natural j ) const;

        NaturalPair getBlockIndices( Natural b ) const;

        Natural getVertexIndexFromPositionInBlock( Natural vpos,  Natural bindex ) const;

        Natural getTriangleIndexFromPositionInBlock( Natural tpos, Natural bindex ) const;

        Triangle getTriangleConnectivityFromIndex(Natural tindex ) const;

        Natural getTriangleIndexFromConnectivity( const Triangle& t_connectivity ) const;

    private:
        Point2D origin_ = {0.0, 0.0}; 
        Point2D length_ = {1.0, 1.0}; 

        Natural discretization_X_ = 1;
        Natural discretization_Y_ = 1;

        Natural nX_ = 3; // = 2*discretization_X_ + 1; 
        Natural nY_ = 3; // = 2*discretization_Y_ + 1; 

        // Cereal provides an easy way to serialize objects
        /* friend class cereal::access; */

        /* template<typename Archive> */
        /* void save( Archive &ar, const std::uint32_t version ) const; */

        /* template<typename Archive> */
        /* void load( Archive &ar, const std::uint32_t version ); */
};

/* template<typename Point2D> */
/* StructuredTriangleMesh2D<Point2D>::StructuredTriangleMesh2D() */
/* { */
/*     ::exactinit(); */
/* } */

template<typename Point2D>
void StructuredTriangleMesh2D<Point2D>::setDomainOrigin(const Point2D& origin)
{
    origin_ = origin;
}

template<typename Point2D>
bool StructuredTriangleMesh2D<Point2D>::setDomainLength(const Point2D& length)
{
    if ((length[0] <= 0) || (length[1] <= 0))
    {
        return false;
    }
    length_ = length;

    return true;
}

template<typename Point2D>
bool StructuredTriangleMesh2D<Point2D>::setDiscretization(const NaturalPair& discretization)
{
    return setDiscretization(discretization[0], discretization[1]);
}

template<typename Point2D>
bool StructuredTriangleMesh2D<Point2D>::setDiscretization(Natural discretization_x, Natural discretization_y)
{
    if ((discretization_x <= 0) || (discretization_y <= 0))
    {
        return false;
    }

    discretization_X_ = discretization_x;
    discretization_Y_ = discretization_y;

    nX_ = 2*discretization_X_ + 1;
    nY_ = 2*discretization_Y_ + 1;

    return true;
}

template<typename Point2D>
Point2D StructuredTriangleMesh2D<Point2D>::origin() const
{
    return origin_;
}

template<typename Point2D>
Point2D StructuredTriangleMesh2D<Point2D>::length() const
{
    return length_;
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::NaturalPair StructuredTriangleMesh2D<Point2D>::discretization() const
{
    return {discretization_X_, discretization_Y_};
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::Natural StructuredTriangleMesh2D<Point2D>::numBlocks() const
{
    auto num_blocks = discretization_X_ * discretization_Y_;

    return num_blocks;
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::Natural StructuredTriangleMesh2D<Point2D>::numVertices() const
{
    auto nX = 2*discretization_X_ + 1; 
    auto nY = 2*discretization_Y_ + 1; 

    auto num_vertices = nX * nY; 

    return num_vertices;
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::Natural StructuredTriangleMesh2D<Point2D>::numTriangles() const
{
    auto num_triangles_per_block = numTrianglesPerBlock();
    auto num_triangles = num_triangles_per_block * numBlocks();

    return num_triangles;
}

template<typename Point2D>
bool StructuredTriangleMesh2D<Point2D>::liesInDomain(const Point2D& p) const
{
    bool inside_x_direction = (p[0] >= origin_[0]) && (p[0] <= origin_[0] + length_[0]);
    bool inside_y_direction = (p[1] >= origin_[1]) && (p[1] <= origin_[1] + length_[1]);

    if (!inside_x_direction || !inside_y_direction)
    {
        return false;
    }

    /* const Point2D& p0 = origin_; */
    /* Point2D p1 = {origin_[0] + length_[0], origin_[1]}; */
    /* Point2D p2 = {origin_[0] + length_[0], origin_[1] + length_[1]}; */
    /* Point2D p3 = {origin_[0], origin_[1] + length_[1]}; */

    /* auto line_p0p1 = [&](const Point2D& p) -> double */
    /* { */
    /*     return ::orient2d(const_cast<double*>(p0.data()), p1.data(), const_cast<double*>(p.data())); */
    /* }; */

    /* auto line_p1p2 = [&](const Point2D& p) -> double */
    /* { */
    /*     return ::orient2d(p1.data(), p2.data(), const_cast<double*>(p.data())); */
    /* }; */

    /* auto line_p2p3 = [&](const Point2D& p) -> double */
    /* { */
    /*     return ::orient2d(p2.data(), p3.data(), const_cast<double*>(p.data())); */
    /* }; */

    /* auto line_p3p0 = [&](const Point2D& p) -> double */
    /* { */
    /*     return ::orient2d(p3.data(), const_cast<double*>(p0.data()), const_cast<double*>(p.data())); */
    /* }; */

    /* bool inside_lp0p1 = line_p0p1(p) >= 0.0; */
    /* bool inside_lp1p2 = line_p1p2(p) >= 0.0; */
    /* bool inside_lp2p3 = line_p2p3(p) >= 0.0; */
    /* bool inside_lp3p0 = line_p3p0(p) >= 0.0; */

    /* if (!inside_lp0p1 || !inside_lp1p2 || !inside_lp2p3 || !inside_lp3p0) */
    /* { */
    /*     return false; */
    /* } */

    return true;
}

template<typename Point2D>
Point2D StructuredTriangleMesh2D<Point2D>::projectOntoDomain(const Point2D& point) const
{
    Point2D p = {point[0], point[1]};
    if(liesInDomain(p))
    {
        return p;
    }

    Eigen::Map<Eigen::Matrix<double, 2, 1>> q(p.data());
    Eigen::Vector2d p0 = {origin_[0], origin_[1]};
    Eigen::Vector2d p1 = {origin_[0] + length_[0], origin_[1]};
    Eigen::Vector2d p2 = {origin_[0] + length_[0], origin_[1] + length_[1]};
    Eigen::Vector2d p3 = {origin_[0], origin_[1] + length_[1]};
    Eigen::Vector2d centroid = (p0 + p1 + p2 + p3)/4; 

    auto lies_in_quadrant = [&](const Eigen::Vector2d& q0, const Eigen::Vector2d& q1, const Eigen::Vector2d& p) -> bool {
        double theta_min = std::atan2(q0[1], q0[0]);
        double theta_max = std::atan2(q1[1], q1[0]);
        double theta = std::atan2(p[1], p[0]);

        if ((theta >= theta_min) && (theta <= theta_max))
        {
            return true;
        }

        return false;
    };

    Eigen::MatrixXd A(2,2);
    Eigen::Vector2d coefs;

    // Q01 lies between p0 & p1
    if (lies_in_quadrant(p0 - centroid, p1 - centroid, q - centroid))
    {
        A << centroid[1], 1,
             q[1], 1;
        coefs = A.lu().solve(Eigen::Vector2d(centroid[0], q[0]));
        
        p[1] = p0[1];
        p[0] = coefs[0]*p0[1] + coefs[1];

        if (p[0] < p0[0])
        {
            p[0] = p0[0];
        }
        else if (p[0] > p1[0])
        {
            p[0] = p1[0];
        }

        return p;
    }

    // Q12 lies between p1 & p2
    if (lies_in_quadrant(p1 - centroid, p2 - centroid, q - centroid))
    {
        A << centroid[0], 1,
             q[0], 1;
        coefs = A.lu().solve(Eigen::Vector2d(centroid[1], q[1]));
        
        p[0] = p1[0];
        p[1] = coefs[0]*p1[0] + coefs[1];

        if (p[1] < p1[1])
        {
            p[1] = p1[1];
        }
        else if (p[1] > p2[1])
        {
            p[1] = p2[1];
        }

        return p;
    }

    
    // Q23 lies between p2 & p3
    if (lies_in_quadrant(p2 - centroid, p3 - centroid, q - centroid))
    {
        A << centroid[1], 1,
             q[1], 1;
        coefs = A.lu().solve(Eigen::Vector2d(centroid[0], q[0]));
        
        p[1] = p2[1];
        p[0] = coefs[0]*p2[1] + coefs[1];

        if (p[0] < p3[0])
        {
            p[0] = p3[0];
        }
        else if (p[0] > p2[0])
        {
            p[0] = p2[0];
        }

        return p;
    }

    
    // Q30 lies between p3 & p0
    // Reflects all points with relation to the y axis when inspecting whether point
    // lies in this quadrant because atan2 is not monotone in this quadrant (in fact,
    // it is discontinuous at (-1., 0.).
    Eigen::MatrixXd R(2, 2);
    R << -1, 0, 
          0, 1;

    if (lies_in_quadrant(R*(p0 - centroid), R*(p3 - centroid), R*(q - centroid)))
    {
        A << centroid[0], 1,
             q[0], 1;
        coefs = A.lu().solve(Eigen::Vector2d(centroid[1], q[1]));
        
        p[0] = p3[0];
        p[1] = coefs[0]*p3[0] + coefs[1];

        if (p[1] < p0[1])
        {
            p[1] = p0[1];
        }
        else if (p[1] > p3[1])
        {
            p[1] = p3[1];
        }

        return p;
    }

    // Code should never reach this line
    std::cout << "\n\n------> Code reached that place!\n\n";
    return p;
}

template<typename Point2D>
bool StructuredTriangleMesh2D<Point2D>::blockIndex(const Point2D& point, Natural &block_index) const
{
    double dx = length_[0]/static_cast<double>(discretization_X_);
    double dy = length_[1]/static_cast<double>(discretization_Y_);

    long int ix, jy;
    if (liesInDomain(point))
    {
        ix = static_cast<long int>(std::floor((point[0] - origin_[0])/dx));
        jy = static_cast<long int>(std::floor((point[1] - origin_[1])/dy));
    }
    else
    {
        return false;
    }
    /* else */
    /* { */
    /*     auto p = projectOntoDomain(point); */
    /*     ix = static_cast<long int>(std::floor(p[0]/dx)); */
    /*     jy = static_cast<long int>(std::floor(p[1]/dy)); */
    /* } */

    auto last_block_indices = getBlockIndices(numBlocks() - 1);
    bool lies_in_interior = (ix >= 0) && (ix <= static_cast<long int>(last_block_indices[0]))
        && (jy >= 0) && (jy <= static_cast<long int>(last_block_indices[1]));

    if (!lies_in_interior)
    {
        if (ix < 0) 
        {
            ix = 0;
        }
        else if (ix > static_cast<long int>(last_block_indices[0]))
        {
            ix = static_cast<long int>(last_block_indices[0]);
        }

        if (jy < 0)
        {
            jy = 0;
        }
        else if (jy > static_cast<long int>(last_block_indices[1]))
        {
            jy = static_cast<long int>(last_block_indices[1]);
        }
    }

    block_index = getBlockIndex(static_cast<Natural>(ix), static_cast<Natural>(jy));

    return true;
}


template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::Natural StructuredTriangleMesh2D<Point2D>::blockIndex(const Point2D& point) const
{
    Natural block_index;

    double dx = length_[0]/static_cast<double>(discretization_X_);
    double dy = length_[1]/static_cast<double>(discretization_Y_);

    long int ix, jy;
    if (liesInDomain(point))
    {
        ix = static_cast<long int>(std::floor((point[0] - origin_[0])/dx));
        jy = static_cast<long int>(std::floor((point[1] - origin_[1])/dy));
    }
    else
    {
        auto p = projectOntoDomain(point);
        ix = static_cast<long int>(std::floor((p[0] - origin_[0])/dx));
        jy = static_cast<long int>(std::floor((p[1] - origin_[1])/dy));
    }

    auto last_block_indices = getBlockIndices(numBlocks() - 1);
    bool lies_in_interior = (ix >= 0) && (ix <= static_cast<long int>(last_block_indices[0]))
        && (jy >= 0) && (jy <= static_cast<long int>(last_block_indices[1]));

    if (!lies_in_interior)
    {
        if (ix < 0) 
        {
            ix = 0;
        }
        else if (ix > static_cast<long int>(last_block_indices[0]))
        {
            ix = static_cast<long int>(last_block_indices[0]);
        }

        if (jy < 0)
        {
            jy = 0;
        }
        else if (jy > static_cast<long int>(last_block_indices[1]))
        {
            jy = static_cast<long int>(last_block_indices[1]);
        }
    }

    block_index = getBlockIndex(static_cast<Natural>(ix), static_cast<Natural>(jy));

    return block_index;
}

template<typename Point2D>
bool StructuredTriangleMesh2D<Point2D>::triangleIndex(const Point2D& point, Natural &tindex) const
{
    /* Natural tindex; */

    /* auto p = projectOntoDomain(point); */
    Natural bindex;
    if (!blockIndex(point, bindex))
    {
        return false;
    }

    /* std::cout << "---> triangle(point = {" << point[0] << ", " << point[1] << "})"; */
    /* std::cout << " -- point lies in block " << bindex; */

    auto line_v0v8 = [&](const Point2D& p) -> double
    {
        Point2D v0, v8;
        vertex(getVertexIndexFromPositionInBlock(0, bindex), v0);
        vertex(getVertexIndexFromPositionInBlock(8, bindex), v8);

        /* return ::orient2d(v0.data(), v8.data(), const_cast<double*>(p.data())); */
        return RobustPredicates::Get().orient2d(v0, v8, p);
    };


    auto line_v3v5 = [&](const Point2D& p) -> double
    {
        Point2D v3, v5;
        vertex(getVertexIndexFromPositionInBlock(3, bindex), v3);
        vertex(getVertexIndexFromPositionInBlock(5, bindex), v5);

        /* return ::orient2d(v3.data(), v5.data(), const_cast<double*>(p.data())); */
        return RobustPredicates::Get().orient2d(v3, v5, p);
    };

    auto line_v6v2 = [&](const Point2D& p) -> double
    {
        Point2D v6, v2;
        vertex(getVertexIndexFromPositionInBlock(6, bindex), v6);
        vertex(getVertexIndexFromPositionInBlock(2, bindex), v2);

        /* return ::orient2d(v6.data(), v2.data(), const_cast<double*>(p.data())); */
        return RobustPredicates::Get().orient2d(v6, v2, p);
    };

    auto line_v7v1 = [&](const Point2D& p) -> double
    {
        Point2D v7, v1;
        vertex(getVertexIndexFromPositionInBlock(7, bindex), v7);
        vertex(getVertexIndexFromPositionInBlock(1, bindex), v1);

        /* return ::orient2d(v7.data(), v1.data(), const_cast<double*>(p.data())); */
        return RobustPredicates::Get().orient2d(v7, v1, p);
    };

    bool above_lv0v8 = line_v0v8(point) > 0.0;
    bool above_lv3v5 = line_v3v5(point) > 0.0;
    bool above_lv6v2 = line_v6v2(point) > 0.0;
    bool above_lv7v1 = line_v7v1(point) > 0.0;

    if (!above_lv0v8 && !above_lv3v5 && !above_lv6v2 && !above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(0, bindex);
        /* std::cout << ", tpos = " << 0 << "\n"; */
    }
    else if (above_lv0v8 && !above_lv3v5 && !above_lv6v2 && !above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(1, bindex);
        /* std::cout << ", tpos = " << 1 << "\n"; */
    }
    else if (above_lv0v8 && above_lv3v5 && !above_lv6v2 && !above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(4, bindex);
        /* std::cout << ", tpos = " << 4 << "\n"; */
    }
    else if (above_lv0v8 && above_lv3v5 && above_lv6v2 && !above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(5, bindex);
        /* std::cout << ", tpos = " << 5 << "\n"; */
    }
    else if (above_lv0v8 && above_lv3v5 && above_lv6v2 && above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(7, bindex);
        /* std::cout << ", tpos = " << 7 << "\n"; */
    }
    else if (!above_lv0v8 && above_lv3v5 && above_lv6v2 && above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(6, bindex);
        /* std::cout << ", tpos = " << 6 << "\n"; */
    }
    else if (!above_lv0v8 && !above_lv3v5 && above_lv6v2 && above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(3, bindex);
        /* std::cout << ", tpos = " << 3 << "\n"; */
    }
    else if (!above_lv0v8 && !above_lv3v5 && !above_lv6v2 && above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(2, bindex);
        /* std::cout << ", tpos = " << 2 << "\n"; */
    }

    return true;
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::Natural StructuredTriangleMesh2D<Point2D>::triangleIndex(const Point2D& point) const
{
    Natural tindex;

    auto p = projectOntoDomain(point);
    Natural bindex = blockIndex(p);
    /* std::cout << "---> triangle(p = {" << p[0] << ", " << p[1] << "})"; */
    /* std::cout << " -- p lies in block " << bindex; */

    auto line_v0v8 = [&](const Point2D& p) -> double
    {
        Point2D v0, v8;
        vertex(getVertexIndexFromPositionInBlock(0, bindex), v0);
        vertex(getVertexIndexFromPositionInBlock(8, bindex), v8);

        /* return ::orient2d(v0.data(), v8.data(), const_cast<double*>(p.data())); */
        return RobustPredicates::Get().orient2d(v0, v8, p);
    };


    auto line_v3v5 = [&](const Point2D& p) -> double
    {
        Point2D v3, v5;
        vertex(getVertexIndexFromPositionInBlock(3, bindex), v3);
        vertex(getVertexIndexFromPositionInBlock(5, bindex), v5);

        /* return ::orient2d(v3.data(), v5.data(), const_cast<double*>(p.data())); */
        return RobustPredicates::Get().orient2d(v3, v5, p);
    };

    auto line_v6v2 = [&](const Point2D& p) -> double
    {
        Point2D v6, v2;
        vertex(getVertexIndexFromPositionInBlock(6, bindex), v6);
        vertex(getVertexIndexFromPositionInBlock(2, bindex), v2);

        /* return ::orient2d(v6.data(), v2.data(), const_cast<double*>(p.data())); */
        return RobustPredicates::Get().orient2d(v6, v2, p);
    };

    auto line_v7v1 = [&](const Point2D& p) -> double
    {
        Point2D v7, v1;
        vertex(getVertexIndexFromPositionInBlock(7, bindex), v7);
        vertex(getVertexIndexFromPositionInBlock(1, bindex), v1);

        /* return ::orient2d(v7.data(), v1.data(), const_cast<double*>(p.data())); */
        return RobustPredicates::Get().orient2d(v7, v1, p);
    };

    bool above_lv0v8 = line_v0v8(p) > 0.0;
    bool above_lv3v5 = line_v3v5(p) > 0.0;
    bool above_lv6v2 = line_v6v2(p) > 0.0;
    bool above_lv7v1 = line_v7v1(p) > 0.0;

    if (!above_lv0v8 && !above_lv3v5 && !above_lv6v2 && !above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(0, bindex);
        /* std::cout << ", tpos = " << 0 << "\n"; */
    }

    if (above_lv0v8 && !above_lv3v5 && !above_lv6v2 && !above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(1, bindex);
        /* std::cout << ", tpos = " << 1 << "\n"; */
    }

    if (above_lv0v8 && above_lv3v5 && !above_lv6v2 && !above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(4, bindex);
        /* std::cout << ", tpos = " << 4 << "\n"; */
    }

    if (above_lv0v8 && above_lv3v5 && above_lv6v2 && !above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(5, bindex);
        /* std::cout << ", tpos = " << 5 << "\n"; */
    }

    if (above_lv0v8 && above_lv3v5 && above_lv6v2 && above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(7, bindex);
        /* std::cout << ", tpos = " << 7 << "\n"; */
    }

    if (!above_lv0v8 && above_lv3v5 && above_lv6v2 && above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(6, bindex);
        /* std::cout << ", tpos = " << 6 << "\n"; */
    }

    if (!above_lv0v8 && !above_lv3v5 && above_lv6v2 && above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(3, bindex);
        /* std::cout << ", tpos = " << 3 << "\n"; */
    }

    if (!above_lv0v8 && !above_lv3v5 && !above_lv6v2 && above_lv7v1)
    {
        tindex = getTriangleIndexFromPositionInBlock(2, bindex);
        /* std::cout << ", tpos = " << 2 << "\n"; */
    }

    return tindex;
}

template<typename Point2D>
bool StructuredTriangleMesh2D<Point2D>::vertex(const Natural vindex, Point2D& v) const
{
    if (/*(vindex < 0) ||*/ (vindex > numVertices())) { 
        return false; 
    }

    NaturalPair indices; 
    getVertexIndices(vindex, indices); 

    auto cast = []( Natural n ) -> double { return static_cast<double>(n); }; 
    v[0] = origin_[0] + cast(indices[0]) * length_[0]/cast(nX_ - 1); 
    v[1] = origin_[1] + cast(indices[1]) * length_[1]/cast(nY_ - 1); 

    return true; 
}

template<typename Point2D>
bool StructuredTriangleMesh2D<Point2D>::triangle(const Natural tindex, Triangle& triangle) const
{
    Natural num_triangles_per_block = numTrianglesPerBlock();
    auto num_triangles = numTriangles();

    if ( tindex >= num_triangles )
    {
        return false;
    }

    auto tpos = tindex % num_triangles_per_block;
    auto bindex = (tindex - tpos) / num_triangles_per_block;

    auto getTriangle = [&] ( Natural v0_pos, Natural v1_pos, Natural v2_pos ) -> Triangle 
    {
        Triangle triangle;
        Natural v0, v1, v2;

        v0 = getVertexIndexFromPositionInBlock(v0_pos, bindex);
        v1 = getVertexIndexFromPositionInBlock(v1_pos, bindex);
        v2 = getVertexIndexFromPositionInBlock(v2_pos, bindex);

        triangle = {v0, v1, v2};

        return triangle;
    };

    switch( tpos )
    {
        case 0:
            triangle = getTriangle(0, 1, 4);
            break;

        case 1:
            triangle = getTriangle(0, 4, 3);
            break;

        case 2:
            triangle = getTriangle(1, 2, 4);
            break;

        case 3:
            triangle = getTriangle(2, 5, 4);
            break;

        case 4:
            triangle = getTriangle(3, 4, 6);
            break;

        case 5:
            triangle = getTriangle(4, 7, 6);
            break;

        case 6:
            triangle = getTriangle(4, 5, 8);
            break;

        case 7:
            triangle = getTriangle(4, 8, 7);
            break;

        default:
            break;
    }

    return true;
}

template<typename Point2D>
std::tuple<typename StructuredTriangleMesh2D<Point2D>::Natural, typename StructuredTriangleMesh2D<Point2D>::BaricentricCoordinates> 
    StructuredTriangleMesh2D<Point2D>::baricentricCoordinates(const Point2D& point) const
{
    Point2D p = projectOntoDomain(point);
    Natural tindex;
    triangleIndex(p, tindex);
    /* std::cout << "\n--- p = " << p[0] << ", " << p[1] << "; tindex = " << tindex; */

    /* if (liesInDomain(point)) */
    /* { */
    /*     triangleIndex(point, tindex); */
    /* } */
    /* else */
    /* { */
    /*     triangleIndex(projectOntoDomain(point), tindex); */
    /* } */

    Triangle tindices;
    triangle(tindex, tindices);

    Point2D p0, p1, p2;
    vertex(tindices[0], p0);
    vertex(tindices[1], p1);
    vertex(tindices[2], p2);

    /* Eigen::Map<Eigen::Matrix<double, 2, 1>> v(p.data()); */
    /* Eigen::Map<Eigen::Matrix<double, 2, 1>> v0(p0.data()); */
    /* Eigen::Map<Eigen::Matrix<double, 2, 1>> v1(p1.data()); */
    /* Eigen::Map<Eigen::Matrix<double, 2, 1>> v2(p2.data()); */

    /* Eigen::Matrix2d A; */

    /* A << v1-v0, v2-v0; */
    /* double area = A.determinant(); */
    double area = RobustPredicates::Get().signed_area(p0, p1, p2);
    /* std::cout << "; area = " << area; */

    /* A << v1-v, v2-v; */
    /* double w0 = A.determinant(); */
    double w0 = RobustPredicates::Get().signed_area(p, p1, p2);
    /* std::cout << "; w0 = " << w0; */

    /* A << v2-v, v0-v; */
    /* double w1 = A.determinant(); */
    double w1 = RobustPredicates::Get().signed_area(p, p2, p0);
    /* std::cout << "; w1 = " << w1; */

    /* A << v0-v, v1-v; */
    /* double w2 = A.determinant(); */
    double w2 = RobustPredicates::Get().signed_area(p, p0, p1);
    /* std::cout << "; w2 = " << w2 << "\n"; */

    return std::make_tuple(Natural{tindex}, BaricentricCoordinates{w0/area, w1/area, w2/area});
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::Natural StructuredTriangleMesh2D<Point2D>::numTrianglesPerBlock() const
{
    Natural num_triangles_per_block = 8;

    return num_triangles_per_block;
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::Natural StructuredTriangleMesh2D<Point2D>::getVertexIndex(Natural i, Natural j) const
{
    return ( i + j*nX_ ); 
}

template<typename Point2D>
bool StructuredTriangleMesh2D<Point2D>::getVertexIndices(Natural v, NaturalPair& indices) const
{
    if (/*(v < 0) ||*/ (v >= numVertices())) { 
        return false; 
    }

    indices[0] = v % nX_; 
    indices[1] = ( v - indices[0] )/nX_;  

    return true; 
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::Natural StructuredTriangleMesh2D<Point2D>::getBlockIndex(Natural i, Natural j) const
{
    return i + j*discretization_X_;
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::NaturalPair StructuredTriangleMesh2D<Point2D>::getBlockIndices(Natural b) const
{
    NaturalPair indices;

    indices[0] = b % discretization_X_;
    indices[1] = (b - indices[0])/discretization_X_;

    return indices;
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::Natural StructuredTriangleMesh2D<Point2D>::getVertexIndexFromPositionInBlock( Natural vpos,  Natural bindex ) const
{
    auto indices = getBlockIndices(bindex);;

    auto i = vpos % 3;
    auto j = (vpos - i)/3;

    auto iv = 2*indices[0] + i;
    auto jv = 2*indices[1] + j;

    return getVertexIndex(iv, jv);
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::Natural StructuredTriangleMesh2D<Point2D>::getTriangleIndexFromPositionInBlock( Natural tpos,  Natural bindex ) const
{
    auto num_triangles = numTriangles();
    Natural num_triangles_per_block = numTrianglesPerBlock();

    if ( (tpos > num_triangles) || (bindex > numBlocks()) )
        return num_triangles;

    return tpos + bindex * num_triangles_per_block;
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::Triangle StructuredTriangleMesh2D<Point2D>::getTriangleConnectivityFromIndex(Natural tindex) const
{
    Natural num_triangles_per_block = numTrianglesPerBlock();
    auto num_triangles = numTriangles();

    Triangle triangle{};
    if ( tindex >= num_triangles )
    {
        return triangle;
    }

    auto tpos = tindex % num_triangles_per_block;
    auto bindex = (tindex - tpos) / num_triangles_per_block;

    auto getTriangle = [&] ( Natural v0_pos, Natural v1_pos, Natural v2_pos ) -> Triangle 
    {
        Triangle triangle;
        Natural v0, v1, v2;

        v0 = getVertexIndexFromPositionInBlock(v0_pos, bindex);
        v1 = getVertexIndexFromPositionInBlock(v1_pos, bindex);
        v2 = getVertexIndexFromPositionInBlock(v2_pos, bindex);

        triangle = {v0, v1, v2};

        return triangle;
    };

    switch( tpos )
    {
        case 0:
            triangle = getTriangle(0, 1, 4);
            break;

        case 1:
            triangle = getTriangle(0, 4, 3);
            break;

        case 2:
            triangle = getTriangle(1, 2, 4);
            break;

        case 3:
            triangle = getTriangle(2, 5, 4);
            break;

        case 4:
            triangle = getTriangle(3, 4, 6);
            break;

        case 5:
            triangle = getTriangle(4, 7, 6);
            break;

        case 6:
            triangle = getTriangle(4, 5, 8);
            break;

        case 7:
            triangle = getTriangle(4, 8, 7);
            break;

        default:
            break;
    }

    return triangle;
}

template<typename Point2D>
typename StructuredTriangleMesh2D<Point2D>::Natural StructuredTriangleMesh2D<Point2D>::getTriangleIndexFromConnectivity(const Triangle& t_connectivity ) const
{
    auto baseConnectivity = [] ( Natural tpos ) -> Triangle
    {
        Triangle triangle; 

        switch( tpos )
        {
            case 0:
                triangle = {0, 1, 4};
                break;

            case 1:
                triangle = {0, 4, 3};
                break;

            case 2:
                triangle = {1, 2, 4};
                break;

            case 3:
                triangle = {2, 5, 4};
                break;

            case 4:
                triangle = {3, 4, 6};
                break;

            case 5:
                triangle = {4, 7, 6};
                break;

            case 6:
                triangle = {4, 5, 8};
                break;

            case 7:
                triangle = {4, 8, 7};
                break;

            default:
                break;
        }

        return triangle;
    };

    /* Natural PlanarSurface::getVertexIndexFromPositionInBlock( Natural vpos,  Natural bindex ) const */
    /* { */
    /*     IndicesType indices = getBlockIndices(bindex);; */

    /*     Natural i = vpos % 3; */
    /*     Natural j = (vpos - i)/3; */

    /*     Natural iv = 2*indices[0] + i; */
    /*     Natural jv = 2*indices[1] + j; */

    /*     return getVertexIndex(iv, jv); */
    /* } */

    Natural num_blocks = numBlocks();

    auto bindexFromVindexAndVpos = [&] ( Natural vindex, Natural vpos ) -> Natural
    {
        Natural ib, jb;
        NaturalPair vindices; 

        bool success = getVertexIndices(vindex, vindices);
        if ( !success )
            return num_blocks;

        ib = (vindices[0] - vpos % 3);
        jb = (vindices[1] - ( vpos - (vpos%3) ));

        if ( ( (ib % 2) != 0 ) || ( (jb % 2) != 0 ) )
            return num_blocks;

        return getBlockIndex(ib/2, jb/2);
    };

    //
    // Given tpos get bindex
    //
    Natural num_triangles_per_block = numTrianglesPerBlock();

    Natural b0, b1, b2;
    Natural tpos = num_triangles_per_block;
    Natural bindex = num_blocks;
    Triangle base_connectivity;

    for ( Natural i = 0; i < num_triangles_per_block; ++i )
    {
        base_connectivity = baseConnectivity(i);

        b0 = bindexFromVindexAndVpos(t_connectivity[0], base_connectivity[0]);
        b1 = bindexFromVindexAndVpos(t_connectivity[1], base_connectivity[1]);
        b2 = bindexFromVindexAndVpos(t_connectivity[2], base_connectivity[2]);

        if ( (b0 == b1) && (b1 == b2) && (b0 < num_blocks) )
        {
            bindex = b0;
            tpos = i;
            break;
        }
    }

    // Get tindex from tpos & bindex
    return getTriangleIndexFromPositionInBlock(bindex, tpos);
}

/*
 * *-------*-------*
 * |\v6    |v7    /|v8
 * | \  t5 | t7  / |  
 * |  \    |    /  |  
 * |   \   |   /   |  
 * | t4 \  |  / t6 |  
 * |     \ | /     |  
 * |      \|/      |  
 * *-------*-------*  
 * |v3    /|\v4    |v5
 * |     / | \     |  
 * | t1 /  |  \ t3 |  
 * |   /   |   \   |  
 * |  /    |    \  |  
 * | /  t0 | t2  \ |  
 * |/      |      \|  
 * *-------*-------*
 *  v0      v1      v2
 *
 *  t0 = (v0, v1, v4)
 *  t1 = (v0, v4, v3)
 *  t2 = (v1, v2, v4)
 *  t3 = (v2, v5, v4)
 *  t4 = (v3, v4, v6)
 *  t5 = (v4, v7, v6)
 *  t6 = (v4, v5, v8)
 *  t7 = (v4, v8, v7)
 */ 

template<typename Point2D>
template<typename FList>
unsigned int StructuredTriangleMesh2D<Point2D>::triangleList( FList &flist ) 
{
    flist.clear(); 

    /* std::cout << "Getting face list for surface: " << getID() << "\n" << std::flush; */ 
    Natural num_blocks = numBlocks();
    Natural num_triangles_per_block = numTrianglesPerBlock();
    Natural num_triangles = numTriangles();
    Natural num_indices_per_triangle = 3;
    unsigned int triangle_count = 0; 

    flist.reserve(num_indices_per_triangle * num_triangles);

    for ( Natural b = 0; b < num_blocks; ++b ) 
    { 
        for ( Natural t = 0; t < num_triangles_per_block; ++t ) 
        { 
            auto tindex = getTriangleIndexFromPositionInBlock(t, b);
            auto triangle = getTriangleConnectivityFromIndex(tindex);

            flist.insert(flist.end(), triangle.begin(), triangle.end());
            ++triangle_count;
        }
    }

    return triangle_count; 
}

/* #if defined( BUILD_WITH_SERIALIZATION ) */
/*     #include "cereal/types/array.hpp" */
/*     #include "cereal/types/vector.hpp" */
/*     #include "cereal/types/list.hpp" */
/*     #include "cereal/types/memory.hpp" */

/*     template<typename Point2D> */
/*     template<typename Archive> */
/*     void StructuredTriangleMesh2D<Point2D>::save( Archive &ar, const std::uint32_t version ) const */
/*     { */
/*         (void)(version); */
/*         ar( */
/*            discretization_X_, */ 
/*            discretization_Y_, */ 
/*            tolerance_, */ 
/*            nX_, */ 
/*            nY_, */ 
/*            num_vertices_, */ 
/*            origin_, */ 
/*            length_ */
/*           ); */
/*     } */

/*     template<typename Point2D> */
/*     template<typename Archive> */
/*     void StructuredTriangleMesh2D<Point2D>::load( Archive &ar, const std::uint32_t version ) */
/*     { */
/*         (void)(version); */
/*         ar( */
/*            discretization_X_, */ 
/*            discretization_Y_, */ 
/*            tolerance_, */ 
/*            nX_, */ 
/*            nY_, */ 
/*            num_vertices_, */ 
/*            origin_, */ 
/*            length_ */
/*           ); */
/*     } */

/*     /1* CEREAL_CLASS_VERSION(StructuredTriangleMesh2D<>, 1); *1/ */

/* #else */
/*     template<typename Point2D> */
/*     template<typename Archive> */
/*     void StructuredTriangleMesh2D<Point2D>::save( Archive &, const std::uint32_t ) const {} */

/*     template<typename Point2D> */
/*     template<typename Archive> */
/*     void StructuredTriangleMesh2D<Point2D>::load( Archive &, const std::uint32_t ) {} */

/* #endif /1* BUILD_WITH_SERIALIZATION *1/ */

#endif 

