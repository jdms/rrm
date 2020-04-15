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

/* #include <Eigen/Dense> */

/* #include "planin/core.hpp" */

#include "planin/predicates.hpp"
#include "predicates/predicates.h"


RobustPredicates::RobustPredicates()
{
    ::exactinit();
}


RobustPredicates& RobustPredicates::Get()
{
    static RobustPredicates predicates;

    return predicates;
}

double RobustPredicates::call_orient2d(const double* a, const double* b, const double* c) const
{
    return ::orient2d(const_cast<double*>(a), const_cast<double*>(b), const_cast<double*>(c)); 
}

double RobustPredicates::call_incircle(const double* a, const double* b, const double* c, const double* d) const
{
    return ::incircle(const_cast<double*>(a), const_cast<double*>(b), const_cast<double*>(c), const_cast<double*>(d)); 
}

double RobustPredicates::call_orient2d_fast(const double* a, const double* b, const double* c) const
{
    return ::orient2dfast(const_cast<double*>(a), const_cast<double*>(b), const_cast<double*>(c)); 
}

double RobustPredicates::call_orient2d_exact(const double* a, const double* b, const double* c) const
{
    return ::orient2dexact(const_cast<double*>(a), const_cast<double*>(b), const_cast<double*>(c)); 
}

double RobustPredicates::call_orient3d(const double* a, const double* b, const double* c, const double* d) const
{
    return ::orient3d(const_cast<double*>(a), const_cast<double*>(b), const_cast<double*>(c), const_cast<double*>(d)); 
}

double RobustPredicates::call_insphere(const double* a, const double* b, const double* c, const double* d, const double* e) const
{
    return ::insphere(const_cast<double*>(a), const_cast<double*>(b), const_cast<double*>(c), const_cast<double*>(d), const_cast<double*>(e)); 
}


double RobustPredicates::call_orient3d_fast(const double* a, const double* b, const double* c, const double* d) const
{
    return ::orient3dfast(const_cast<double*>(a), const_cast<double*>(b), const_cast<double*>(c), const_cast<double*>(d)); 
}

double RobustPredicates::call_orient3d_exact(const double* a, const double* b, const double* c, const double* d) const
{
    return ::orient3dexact(const_cast<double*>(a), const_cast<double*>(b), const_cast<double*>(c), const_cast<double*>(d)); 
}

/* template<> double RobustPredicates::orient2d<Point2>(const Point2& a, const Point2& b, const Point2& c) const; */
/* template<> double RobustPredicates::orient2d<Eigen::Vector2d>(const Eigen::Vector2d& a, const Eigen::Vector2d& b, const Eigen::Vector2d& c) const; */
/* template<> double RobustPredicates::orient2d<std::array<double, 2>>(const std::array<double, 2>& a, const std::array<double, 2>& b, const std::array<double, 2>& c) const; */
