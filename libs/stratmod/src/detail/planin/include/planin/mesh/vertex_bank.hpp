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


#ifndef PLANIN_VERTEX_BANK_HPP
#define PLANIN_VERTEX_BANK_HPP

#include <map>
#include <vector>

template<typename VertexType, typename NameType>
class VertexBank 
{
    using Integer = long long int;

    public:
        VertexBank(double tolerance) {}
        ~VertexType() = default;
        bool add(const VertexType& v, const NameType& name);

        Integer id(const VertexType& v) const;
        Integer id(const NameType& name) const;
        VertexType vertex(const Integer& id) const;
        VertexType vertex(const NameType& name) const;

    private:
        std::map<VertexType, Integer> vertex_map_;
        std::map<NameType, Integer> name_map_;
        std::vector<VertexType> vertex_list_;
};

#endif
