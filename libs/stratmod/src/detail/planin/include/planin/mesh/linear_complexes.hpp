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


#ifndef PLANIN_LINEAR_COMPLEXES_HPP
#define PLANIN_LINEAR_COMPLEXES_HPP

#include <limits>
#include <cmath>
#include <set>
#include <map>

#include "../core.hpp"

template<typename VertexList, size_t dim = 3>
class Segment
{
    private:
        VertexList &vlist;
        /* const size_t dim; */
        size_t source, sink;
        bool source_is_valid, sink_is_valid;
        /* std::set<double> points = {}; // = { 0, 1 }; */
        std::map<double, std::pair<size_t, bool>> points_dictionary;
        double tolerance = 1E-6;

        size_t insertPointInVlist( double t, bool point_is_valid = true )
        {
            long int pindex = vlist.size()/dim;

            if ( insertPointInVlist(t, pindex, point_is_valid) == false )
            {
                return std::numeric_limits<size_t>::max();
            }

            return pindex;
        }

        bool insertPointInVlist( double t, size_t pindex, bool point_is_valid = true )
        {
            if ( insertPointInDictionary(t, pindex, point_is_valid) == false )
            {
                return false;
            }

            std::array<double, dim> p = getPoint<std::array<double, dim>>(t);

            /* vlist.push_back(p.x); */
            /* vlist.push_back(p.y); */
            /* vlist.push_back(p.z); */

            for ( size_t i = 0; i < dim; ++i )
            {
                vlist.push_back(p[i]);
            }

            return true;
        }

        bool insertPointInDictionary( double t, size_t pindex, bool point_is_valid = true )
        {
            if ( (t<0) || (t>1) )
            {
                return false;
            }

            /* points.insert(t); */
            points_dictionary[t] = std::make_pair(pindex, point_is_valid);

            return true;
        }

        template<typename SegmentType>
        friend double computeVerticalIntersection( SegmentType &sa, SegmentType &sb );

        template<typename SegmentType>
        friend double computeHorizontalIntersection( SegmentType &sa, SegmentType &sb, Point3 &intersection_point );

    public: 
        PointWrapper<VertexList, dim> p0;
        PointWrapper<VertexList, dim> p1;

        double p0Height() const
        {
            return p0[dim-1];
        }

        double p1Height() const
        {
            return p1[dim-1];
        }

        Segment( VertexList &input_vlist, size_t source_index, size_t sink_index, bool source_is_valid_ = true, bool sink_is_valid_ = true ) : 
            vlist(input_vlist), // dim( dim_ > 0 ? dim_ : 3 ), 
            source(source_index), sink(sink_index), 
            source_is_valid(source_is_valid_), sink_is_valid(sink_is_valid_), 
            p0(vlist, source), p1(vlist, sink) 
        {
            points_dictionary[0.] = std::make_pair(source_index, source_is_valid);
            points_dictionary[1.] = std::make_pair(sink_index, sink_is_valid);
        }

        template<typename ElementType>
        size_t getConnectivity( ElementType &connectivity ) const
        {
            using OutNaturalType = typename ElementType::value_type;
            size_t num_segments = 0;

            connectivity.clear();
            /* connectivity.push_back(source); */
            OutNaturalType pindex, next_pindex;
            auto it = points_dictionary.begin();
            auto next = it; next++;

            /* std::cout << "Segment [(" << p0[0] << ", " << p0[1] << "), (" << p1[0] << ", " << p1[1] << ")]\n"; */
            /* std::cout << "Connectivity list: "; */
            for ( ; next != points_dictionary.end(); ++it, ++next )
            {
                if ( it->second.second || next->second.second )
                {
                    pindex = static_cast<OutNaturalType>(it->second.first);
                    next_pindex = static_cast<OutNaturalType>(next->second.first);
                    connectivity.push_back(pindex);
                    connectivity.push_back(next_pindex);
                    ++num_segments;
                    /* std::cout << "(" << pindex << ", " << next_pindex << "), "; */
                }
            }
            /* std::cout << "\n" << std::flush; */

            return num_segments;
        }

        size_t getVertices( VertexList &new_vlist ) const
        {
            size_t num_new_vertices = points_dictionary.size();

            new_vlist.clear();
            size_t pindex;

            for ( auto &entry : points_dictionary )
            {
                /* p[0] = (1 - t)*p0[0] + t*p1[0]; */
                /* p[1] = (1 - t)*p0[1] + t*p1[1]; */
                /* p[2] = (1 - t)*p0[2] + t*p1[2]; */

                pindex = entry.second.first;

                /* new_vlist.push_back(vlist[pindex + 0]); */
                /* new_vlist.push_back(vlist[pindex + 1]); */
                /* new_vlist.push_back(vlist[pindex + 2]); */

                for ( size_t i = 0; i < dim; ++i )
                {
                    new_vlist.push_back(vlist[pindex + i]);
                }
            }

            return num_new_vertices;
        }

        double getTolerance() const
        {
            return tolerance;
        }

        template<typename Point>
        Point getPoint( double t ) const
        {
            Point p;
            if ( p.size() < dim )
            {
                return p;
            }

            for ( size_t i = 0; i < dim; ++i )
            {
                p[i] = (1 - t) * p0[i] + t * p1[i];
                /* p[0] = (1 - t)*p0[0] + t*p1[0]; */
                /* p[1] = (1 - t)*p0[1] + t*p1[1]; */
                /* p[2] = (1 - t)*p0[2] + t*p1[2]; */
            }

            return p;
        }

        size_t computeVerticalIntersection( const Segment &to_intersect )
        {
            // check if 2D base points are equal
            double tol = getTolerance();
            size_t pindex = std::numeric_limits<size_t>::max();

            // compute intersection

            /* std::cout << "\n Computing intersection: "; */
            // sa.p0[2] + t ( sa.p1[2] - sa.p0[2] ) = sb.p0[2] + t ( sb.p1[2] - sb.p0[2] ) 
            double num = to_intersect.p0Height() - p0Height(); 
            double den = (p1Height() - p0Height()) - (to_intersect.p1Height() - to_intersect.p0Height());

            /* std::cout << "num = " << num << ", " << "den = " << den; */ 
            if ( std::abs(den) < tol )
            {
                /* std::cout << ", error |den| < tol\n\n" << std::flush; */
                return pindex;
            }

            double t = num/den;
            /* std::cout << ", t = " << t; */


            // TODO: the code that verifies the admissibility of a new point can only work properly for the first intersection
            // point_is_valid is true is it's two neighbours are valid
            // I problably want to have the last valid point in segment marked as true instead of false, though
            bool point_is_valid = source_is_valid && sink_is_valid;
            pindex = insertPointInVlist(t, point_is_valid);

            /* if ( (t >= 0) || (t <= 1) ) */
            /* { */
            /*     pindex = insertPointInVlist(t, point_is_valid); */
                /* std::cout << ", inserting in sa(" << pindex << ", " << (point_is_valid ? "true" : "false" ) << ")"; */
            /* } */
            /* std::cout << "\n\n" << std::flush; */

            return pindex;
        }
};


template<typename SegmentType>
double computeVerticalIntersection( SegmentType &sa, SegmentType &sb )
{
    // check if 2D base points are equal
    /* double tol = std::min(sa.getTolerance(), sb.getTolerance()); */
    double tol = sa.getTolerance();

    // compute intersection
    
    double t = -1;

    /* std::cout << "\n Computing intersection: "; */
    // sa.p0[2] + t ( sa.p1[2] - sa.p0[2] ) = sb.p0[2] + t ( sb.p1[2] - sb.p0[2] ) 
    double num = sb.p0Height() - sa.p0Height(); 
    double den = (sa.p1Height() - sa.p0Height()) - (sb.p1Height() - sb.p0Height());
    
    /* std::cout << "num = " << num << ", " << "den = " << den; */ 
    if ( std::abs(den) < tol )
    {
        /* std::cout << ", error |den| < tol\n\n" << std::flush; */
        return t;
    }

    t = num/den;
    /* std::cout << ", t = " << t; */

    /* size_t new_index = sa.insertPoint(t); */
    /* sb.insertPoint(t, new_index); */

    // TODO: the code that verifies the admissibility of a new point can only work properly for the first intersection
    // point_is_valid is true is it's two neighbours are valid
    // I problably want to have the last valid point in segment marked as true instead of false, though
    bool point_is_valid_in_sa = sa.source_is_valid && sa.sink_is_valid;
    bool point_is_valid_in_sb = sb.source_is_valid && sb.sink_is_valid;
    if ( (t >= 0) || (t <= 1) )
    {
        long int pindex = sa.insertPointInVlist(t, point_is_valid_in_sa);
        /* std::cout << ", inserting in sa(" << pindex << ", " << (point_is_valid_in_sa ? "true" : "false" ) << ")"; */
        if ( pindex > 0 )
        {
            /* std::cout << ", inserting in sb(" << pindex << ", " << (point_is_valid_in_sb ? "true" : "false" ) << ")"; */
            sb.insertPointInDictionary(t, pindex, point_is_valid_in_sb);
        }
    }
    /* std::cout << "\n\n" << std::flush; */

    return t;
}

template<typename SegmentType>
bool computeHorizontalIntersection( SegmentType &sa, SegmentType &sb, Point3 &intersection_point )
{
    // check if 2D base points are coplanar
    UNUSED(sa);
    UNUSED(sb);

    UNUSED(intersection_point);

    return false;
}


struct TriangleFacet
{
};

#endif
