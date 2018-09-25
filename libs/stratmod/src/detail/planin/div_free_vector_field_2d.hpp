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


#ifndef __DIV_FREE_VECTOR_FIELD_2D__
#define __DIV_FREE_VECTOR_FIELD_2D__

#include <vector> 
#include <cstdint>

/* #pragma warning (disable : 4068) */

/* // Eigen & GCC 5 : class std::binder2nd is deprecated */
/* // http://stackoverflow.com/questions/30535933/eigen-gcc-5-class-stdbinder2nd-is-deprecated */
/* // FIXED: for some weird reason, the "system" keyword is not working when */
/* // including Eigen's headers in the "CMakeLists.txt" file.  Solution: used */
/* // incorrect call to Eigen3_INCLUDE_DIRS, which needed to be changed to */
/* // EIGEN3_INCLUDE_DIR */
/* #pragma GCC diagnostic push */
/* #pragma GCC diagnostic ignored "-Wdeprecated-declarations" */
/* /1* TODO: include special inclusion for Eigen in Windows *1/ */
/* #include "Eigen/Dense" */ 
/* #pragma GCC diagnostic pop */ 

#include "serialization_primitives.hpp"

#include "core.hpp" // Dummy definitions to be replaced by the project's definitions. 
#include "kernels.hpp"
#include "polynomial.hpp" 

using Kernel = Wendland23; 

class DivFreeVectorField2D 
{ 
    public: 
        DivFreeVectorField2D( Kernel k ); 

        DivFreeVectorField2D( const DivFreeVectorField2D & ) = default; 
        DivFreeVectorField2D& operator=( const DivFreeVectorField2D & ) = default;

        /* Interpolant2D( Interpolant2D && ) = default; */ 
        /* Interpolant2D& operator=( Interpolant2D && ) = default; */ 

        Vector2 operator()( double x, double y ); 
        Vector2 operator()( Point2 &p ); 

        /* double Dx( double x, double y ); */ 
        /* double Dx( Point2 &p ); */ 

        /* double Dy( double x, double y ); */ 
        /* double Dy( Point2 &p ); */ 

        bool interpolantIsSet(); 
        int isSmooth(); 

        bool addVectorEvaluation( const Point2 &p, const Vector2 &v_eval);
        bool addVectorEvaluation( Point2 &&p, Vector2 &&v_eval);
        bool addVectorEvaluations( std::vector<Point2> &points, std::vector<Vector2> &v_evals);

        bool addComponentEvaluation( const Point2 &p, const Point2 &l, double lom_eval ); 
        bool addComponentEvaluation( Point2 &&p, const Point2 &l, double lom_eval ); 
        bool addComponentEvaluations( std::vector<Point2> &points, std::vector<Point2> &lambdas, std::vector<double> &lom_evals ); 

        bool interpolate(); 

        void clear(); 

    private:
        Kernel k_; 
        unsigned int dim_; 
        unsigned int poly_dim_; 

        std::vector<Point2> points_;
        std::vector<Point2> lambdas_;
        std::vector<double> lom_evals_; 
        std::vector<double> weights_; 

        bool interpolant_is_set_ = false;  

        Matrix22 mKernelEval( double x, double y );

        // Cereal provides an easy way to serialize objects
        friend class cereal::access;

        template<typename Archive>
        void serialize( Archive &ar, const std::uint32_t version );
};

#if defined( BUILD_WITH_SERIALIZATION )
    #include "cereal/types/vector.hpp"

    template<typename Archive>
    void DivFreeVectorField2D::serialize( Archive &ar, const std::uint32_t version )
    {
        (void)(version);
        ar(
            dim_, 
            poly_dim_, 
            points_, 
            lambdas_, 
            lom_evals_, 
            weights_, 
            interpolant_is_set_
          );
    }

    CEREAL_CLASS_VERSION(DivFreeVectorField2D, 1);

#else
    template<typename Archive>
    void DivFreeVectorField2D::serialize( Archive &, const std::uint32_t ) {}

#endif /* BUILD_WITH_SERIALIZATION */

#endif
