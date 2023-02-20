/* This file is part of the stratmod library */
/* Copyright (C) 2017-2020, Julio Daniel Machado Silva. */

/* The stratmod library is free software: you can redistribute it and/or modify it */
/* under the terms of the GNU General Public License as published by the Free */
/* Software Foundation, either version 3 of the License, or (at your option) any */
/* later version. */

/* The stratmod library is distributed in the hope that it will be useful, but */
/* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or */
/* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more */
/* details. */

/* You should have received a copy of the GNU General Public License */
/* along with the stratmod library.  If not, see <https://www.gnu.org/licenses/>. */

#ifndef STRATMOD_CURVE_3D
#define STRATMOD_CURVE_3D

#include <map>
#include <memory>
#include <string>
#include <optional>
#include <vector>

#include "stratmod/misc/stratmod_primitives.hpp"
#include "stratmod/metadata.hpp"

#include "Eigen/Dense"

namespace stratmod {

struct STRATMODLIB_DLL_HANDLER Curve3D {
    Eigen::Matrix<double, Eigen::Dynamic, 3>  vertices;
    Eigen::Matrix<int, Eigen::Dynamic, 2>  edges;
    CurveMetadata metadata;

    template<typename T>
    bool setVertices(const std::vector<T>& vlist)
    {
        if ((vlist.size() % 3) != 0)
        {
            return false;
        }
        std::size_t num_vertices = vlist.size()/3;
        vertices = Eigen::Matrix<double, Eigen::Dynamic, 3>(num_vertices, 3);

        for (std::size_t i = 0; i < num_vertices; ++i)
        {
            vertices(i, 0) = static_cast<double>(vlist[3*i + 0]);
            vertices(i, 1) = static_cast<double>(vlist[3*i + 1]);
            vertices(i, 2) = static_cast<double>(vlist[3*i + 2]);
        }

        return true;
    }

    template<typename T>
    std::vector<T> getVertices()
    {
        std::vector<T> vlist(static_cast<std::size_t>(vertices.rows()*vertices.cols()));
        for (int i = 0; i < vertices.rows(); ++i)
        {
            vlist[3*i + 0] = static_cast<T>(vertices(i, 0));
            vlist[3*i + 1] = static_cast<T>(vertices(i, 1));
            vlist[3*i + 2] = static_cast<T>(vertices(i, 2));
        }

        return vlist;
    }

    template<typename T>
    bool setEdges(const std::vector<T>& elist)
    {
        if ((elist.size() % 2) != 0)
        {
            return false;
        }
        std::size_t num_edges = elist.size()/2;
        edges = Eigen::Matrix<int, Eigen::Dynamic, 2>(num_edges, 2);

        for (std::size_t i = 0; i < num_edges; ++i)
        {
            edges(i, 0) = static_cast<int>(elist[2*i + 0]);
            edges(i, 1) = static_cast<int>(elist[2*i + 1]);
        }

        return true;
    }

    template<typename T>
    std::vector<T> getEdges()
    {
        std::vector<T> elist(static_cast<std::size_t>(edges.rows()*edges.cols()));
        for (int i = 0; i < edges.rows(); ++i)
        {
            elist[2*i + 0] = static_cast<T>(edges(i, 0));
            elist[2*i + 1] = static_cast<T>(edges(i, 1));
        }

        return elist;
    }
};

} // namespace stratmod

#endif
