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


#ifndef PLANIN_PIECEWISE_LINEAR_GRAPH_HPP
#define PLANIN_PIECEWISE_LINEAR_GRAPH_HPP


#include <array>
#include <cmath>
#include <tuple>
#include <vector> 
#include <iostream>

#include "planin/structured_triangle_mesh_2d.hpp"

#include <Eigen/Dense>

template<typename Point2D = std::array<double, 2>>
class PiecewiseLinearGraph : public StructuredTriangleMesh2D<Point2D> {
    public:
        using Natural = typename StructuredTriangleMesh2D<Point2D>::Natural;
        using NaturalPair = typename StructuredTriangleMesh2D<Point2D>::NaturalPair;
        using BaricentricCoordinates = typename StructuredTriangleMesh2D<Point2D>::BaricentricCoordinates;

        PiecewiseLinearGraph()
        {
            auto num_vertices = StructuredTriangleMesh2D<Point2D>::numVertices();

            heights_ = std::vector<double>(num_vertices, 0.);
            normals_ = std::vector<double>(3*num_vertices, 0.);
            /* normals_ = std::vector<Eigen::Vector3d>(num_vertices); */
        }

        ~PiecewiseLinearGraph() = default;
        PiecewiseLinearGraph(const PiecewiseLinearGraph&) = default;
        PiecewiseLinearGraph& operator=(const PiecewiseLinearGraph&) = default;
        PiecewiseLinearGraph(PiecewiseLinearGraph&&) = default;
        PiecewiseLinearGraph& operator=(PiecewiseLinearGraph&&) = default;

        virtual bool setDiscretization(const NaturalPair& discretization) override
        {
            auto num_vertices = StructuredTriangleMesh2D<Point2D>::numVertices();
            bool success = StructuredTriangleMesh2D<Point2D>::setDiscretization(discretization);
            if (success)
            {
                heights_ = std::vector<double>(num_vertices, 0.);
                normals_ = std::vector<double>(3*num_vertices, 0.);
                /* normals_ = std::vector<Eigen::Vector3d>(num_vertices); */
            }

            return success;
        }

        virtual bool setDiscretization(Natural discretization_x, Natural discretization_y) override
        {
            auto num_vertices = StructuredTriangleMesh2D<Point2D>::numVertices();
            bool success = StructuredTriangleMesh2D<Point2D>::setDiscretization(discretization_x, discretization_y);
            if (success)
            {
                heights_ = std::vector<double>(num_vertices, 0.);
                normals_ = std::vector<double>(3*num_vertices, 0.);
                /* normals_ = std::vector<Eigen::Vector3d>(num_vertices); */
            }

            return success;
        }

        double height(const Point2D& point) const
        {
            auto [tindex, coords] = StructuredTriangleMesh2D<Point2D>::baricentricCoordinates(point);

            return height(tindex, coords);
        }

        double height(Natural triangle_index, const BaricentricCoordinates& coords) const
        {
            auto t = StructuredTriangleMesh2D<Point2D>::getTriangleConnectivityFromIndex(triangle_index);

            double height = heights_[t[0]]*coords[0] + heights_[t[1]]*coords[1] + heights_[t[2]]*coords[2]; 

            return height;
        }

        double height(Natural vertex_index) const &&
        {
            return heights_[vertex_index];
        }

        double& height(Natural vertex_index) &
        {
            return heights_[vertex_index];
        }

        bool setData(const NaturalPair& discretization, std::vector<double> heights, std::vector<double> normals = {})
        {
            bool success = StructuredTriangleMesh2D<Point2D>::setDiscretization(discretization);
            if (!success)
            {
                return false;
            }
            auto num_vertices = StructuredTriangleMesh2D<Point2D>::numVertices();

            if (heights.size() != num_vertices)
            {
                return false;
            }
            heights_ = std::move(heights);

            if (normals.size() != 3*num_vertices)
            {
                computeNormals();
            }
            else
            {
                normals_ = std::move(normals);
            }

            return true;
        }

        std::vector<double> getHeights()
        {
            return heights_;
        }

        Eigen::Vector3d normal(const Point2D& point) const
        {
            auto [tindex, coords] = StructuredTriangleMesh2D<Point2D>::baricentricCoordinates(point);

            return normal(tindex, coords);
        }

        Eigen::Vector3d normal(Natural triangle_index, const BaricentricCoordinates& coords) const
        {
            auto t = StructuredTriangleMesh2D<Point2D>::getTriangleConnectivityFromIndex(triangle_index);

            Eigen::Vector3d n0(normals_[3*t[0] + 0], normals_[3*t[0] + 1], normals_[3*t[0] + 2]); 
            Eigen::Vector3d n1(normals_[3*t[1] + 0], normals_[3*t[1] + 1], normals_[3*t[1] + 2]); 
            Eigen::Vector3d n2(normals_[3*t[2] + 0], normals_[3*t[2] + 1], normals_[3*t[2] + 2]); 
            /* auto& n0 = normals_[t[0]]; */
            /* auto& n1 = normals_[t[1]]; */
            /* auto& n2 = normals_[t[2]]; */

            Eigen::Vector3d normal = n0*coords[0] + n1*coords[1] + n2*coords[2];
            normal = normal/normal.norm();

            return normal;
        }

        Eigen::Vector3d normal(Natural vertex_index) const &&
        {
            auto num_vertices = StructuredTriangleMesh2D<Point2D>::numVertices();
            Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> normals(const_cast<double*>(normals_.data()), 3, num_vertices);

            return normals.col(vertex_index);
            /* return normals_[vertex_index]; */
        }

        template<typename Derived>
        Eigen::Block<Derived>& normal(Natural /* vertex_index */) &
        {
            auto num_vertices = StructuredTriangleMesh2D<Point2D>::numVertices();
            Eigen::Map<Eigen::Matrix<double, 3, Eigen::Dynamic, Eigen::RowMajor>> normals(normals_.data(), 3, num_vertices);


            return Eigen::Block<Derived>(normals.derived(), 0, num_vertices, 3, 1);
            /* return normals.col(vertex_index); */
            /* return normals_[vertex_index]; */
        }

        void computeNormals()
        {
        }

        /* bool setNormals(const std::vector<double>& normals) */
        /* { */
        /*     if (normals.size() != 3*StructuredTriangleMesh2D<Point2D>::numVertices()) */
        /*     { */
        /*         return false; */
        /*     } */

        /*     for (size_t i = 0; i < StructuredTriangleMesh2D<Point2D>::numVertices(); ++i) */
        /*     { */
        /*         normals_[i] = Eigen::Vector3d(normals[3*i + 0], normals[3*i + 1], normals[3*i + 2]); */
        /*     } */

        /*     return true; */
        /* } */

        std::vector<double> getNormals()
        {
            /* std::vector<double> normals(3*normals_.size()); */
            /* for (std::size_t i = 0; i < normals_.size(); ++i) */
            /* { */
            /*     normals[3*i + 0] = normals_[i][0]; */
            /*     normals[3*i + 1] = normals_[i][1]; */
            /*     normals[3*i + 2] = normals_[i][2]; */
            /* } */

            return normals_;
        }

    private:
        std::vector<double> heights_;
        std::vector<double> normals_;
};

#endif
