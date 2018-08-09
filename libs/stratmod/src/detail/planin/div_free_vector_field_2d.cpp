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


#include "div_free_vector_field_2d.hpp"

/* Disable MSVC warnings due to the following (GCC) unknown pragmas */
/* https://msdn.microsoft.com/en-us/library/2c8f766e.aspx 
 */
#pragma warning (disable : 4068)

// Eigen & GCC 5 : class std::binder2nd is deprecated
// http://stackoverflow.com/questions/30535933/eigen-gcc-5-class-stdbinder2nd-is-deprecated
// FIXED: for some weird reason, the "system" keyword is not working when
// including Eigen's headers in the "CMakeLists.txt" file.  Solution: used
// incorrect call to Eigen3_INCLUDE_DIRS, which needed to be changed to
// EIGEN3_INCLUDE_DIR
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
/* TODO: include special inclusion for Eigen in Windows */
#include "Eigen/Dense" 
#pragma GCC diagnostic pop 

/* #include "prettyprint.hpp" */ //debug 

DivFreeVectorField2D::DivFreeVectorField2D( Kernel k, unsigned int poly_dim ) : 
    k_(k), dim_(2) 
{
    UNUSED(dim_); 

    if ( (poly_dim == 2) || (poly_dim == 3) ) 
        poly_dim_ = poly_dim; 
    else
        poly_dim_ = 3; 
}  

Matrix22 DivFreeVectorField2D::mKernelEval( double x, double y )
{
    Matrix22 M;

    /* H11 = D2phi(X, Y, zeros(size(X)), 1, 1, delta); */
    /* H12 = D2phi(X, Y, zeros(size(X)), 1, 2, delta); */
    /* H21 = D2phi(X, Y, zeros(size(X)), 2, 1, delta); */
    /* H22 = D2phi(X, Y, zeros(size(X)), 2, 2, delta); */

    /* L = H11 + H22; */

    /* M11 = H11 - L; */
    /* M12 = H12; */
    /* M21 = H21; */
    /* M22 = H22 - L; */

    return M;
};

Vector2 DivFreeVectorField2D::operator()( double x, double y ) 
{
    Vector2 Fxy {};

    if ( !interpolantIsSet() ) { 
        return Fxy; 
    }

    const size_t size = points_.size(); 
    Matrix22 M;

    for ( size_t i = 0; i < size; ++i )
    {
        M = mKernelEval(x - points_[i].x, y - points_[i].y);
        Fxy.u += weights_[i] * M(1,1) * lambdas_[i].u + M(1,2) * lambdas_[i].v; 
        Fxy.v += weights_[i] * M(2,1) * lambdas_[i].u + M(2,2) * lambdas_[i].v; 
    }

    /* for ( size_t i = 0; i < size; ++i ) */ 
    /* { */
    /*     z += weights_[i] * k_( x - points_[i].x, y - points_[i].y ); */ 
    /* } */
    /* if ( poly_dim_ >= 2 ) { */
    /*     z += weights_[size]*1.0 + weights_[size + 1]*x; */ 
    /* } */
    
    /* if ( poly_dim_ == 3 ) { */
    /*     z += weights_[size + 2]*y; */
    /* } */

    return Fxy; 
}

Vector2 DivFreeVectorField2D::operator()( Point2 &p ) 
{
    return operator()(p.x, p.y); 
}

/* double DivFreeVectorField2D::Dx( double x, double y ) */ 
/* { */ 
/*     if ( !interpolantIsSet() ) { */ 
/*         return 0; */ 
/*     } */

/*     if ( k_.isSmooth() < 1 ) { */ 
/*         return 0; */ 
/*     } */

/*     double z = 0; */ 
/*     const size_t size = points_.size(); */ 

/*     for ( size_t i = 0; i < size; ++i ) */ 
/*     { */
/*         z += weights_[i] * k_.Dx( x - points_[i].x, y - points_[i].y ); */ 
/*     } */

/*     if ( poly_dim_ >= 2 ) { */
/*         z += weights_[size + 1]; */ 
/*     } */

/*     return z; */ 
/* } */
 
/* double DivFreeVectorField2D::Dx( Point2 &p ) */ 
/* { */ 
/*     return Dx(p.x, p.y); */
/* } */

/* double DivFreeVectorField2D::Dy( double x, double y ) */ 
/* { */
/*     if ( !interpolantIsSet() ) { */ 
/*         return 0; */ 
/*     } */

/*     if ( k_.isSmooth() < 1 ) { */ 
/*         return 0; */ 
/*     } */

/*     if ( poly_dim_ < 3 ) { */ 
/*         return 0; */ 
/*     } */

/*     double z = 0; */ 
/*     const size_t size = points_.size(); */ 

/*     for ( size_t i = 0; i < size; ++i ) */ 
/*     { */
/*         z += weights_[i] * k_.Dy( x - points_[i].x, y - points_[i].y ); */ 
/*     } */
/*     z += weights_[size + 2]; */ 

/*     return z; */    
/* } */ 

/* double DivFreeVectorField2D::Dy( Point2 &p ) */ 
/* { */ 
/*     return Dy(p.x, p.y); */
/* } */

int DivFreeVectorField2D::isSmooth() 
{ 
    return k_.isSmooth();
}

bool DivFreeVectorField2D::interpolantIsSet() 
{
    return interpolant_is_set_; 
} 

bool DivFreeVectorField2D::addVectorEvaluation( const Point2 &p, const Vector2 &vector_eval)
{
    Vector2 lu = {{{ 1, 0 }}}, lv = {{{ 0, 1 }}};
    bool success = true;

    success &= addComponentEvaluation(p, lu, vector_eval.u );
    success &= addComponentEvaluation(p, lv, vector_eval.v );

    return true;
}

bool DivFreeVectorField2D::addVectorEvaluation( Point2 &&p, Vector2 &&vector_eval)
{
    return addVectorEvaluation(p, vector_eval);
}

bool DivFreeVectorField2D::addVectorEvaluations( std::vector<Point2> &points, std::vector<Vector2> &vector_evals)
{
    if ( points.size() != vector_evals.size() )
    {
        return false;
    }

    bool success = true;
    for ( size_t i = 0; i < points.size(); ++i )
    {
        success &= addVectorEvaluation(points[i], vector_evals[i]);
    }

    return success;
}

bool DivFreeVectorField2D::addComponentEvaluation(  const Point2 &p, const Point2 &l, double lom_eval ) 
{
    points_.push_back(p); 
    lambdas_.push_back(l);
    lom_evals_.push_back(lom_eval); 

    return true; 
} 

bool DivFreeVectorField2D::addComponentEvaluation(  Point2 &&p, const Point2 &l, double lom_eval ) 
{
    return addComponentEvaluation(p, l, lom_eval); 
}

bool DivFreeVectorField2D::addComponentEvaluations(  std::vector<Point2> &points, std::vector<Point2> &lambdas, std::vector<double> &lom_evals ) 
{
    if ( (points.size() != lambdas.size()) || (points.size() != lom_evals.size()) ) { 
        return false; 
    }

    std::copy( points.begin(), points.end(), points_.end() ); 
    std::copy( lambdas.begin(), lambdas.end(), lambdas_.end() ); 
    std::copy( lom_evals.begin(), lom_evals.end(), lom_evals_.end() ); 

    return true; 
}

bool DivFreeVectorField2D::interpolate() 
{
    /* TODO: this method assumes that input points_ is always valid, beware */
    if ( points_.size() != lom_evals_.size() ) { 
        return false; 
    }
    
    if ( points_.size() == 0 )
    {
        return false;
    }

    size_t size = points_.size(); 
    weights_.resize(size); 

    /* auto local_fevals = fevals_; */ 
    /* for ( size_t i = 0; i < poly_dim_; ++i ) { */ 
    /*     local_fevals.push_back(0.0); */
    /* } */

    const double eigenvalues_lower_bound = 0.01; 
    double aij = 0; 
    Point2 pi, pj; 
    Vector2 li, lj;
    Matrix22 M;
    Eigen::MatrixXd A(size, size);
    /* Eigen::MatrixXd A(size + poly_dim_, size + poly_dim_); */ 
    /* Eigen::MatrixXd P(size, poly_dim_); */ 
    Eigen::Map<Eigen::VectorXd> weights( weights_.data(), size, 1); 
    Eigen::Map<Eigen::VectorXd> lom_evaluations( lom_evals_.data(), size, 1); 

    for ( size_t i = 0; i < size; ++i )
    {
        pi = points_[i]; 
        li = lambdas_[i];

        for ( size_t j = 0; j < size; ++j )
        {
            pj = points_[j]; 
            lj = lambdas_[j]; 
            M = mKernelEval(pi.x - pj.x, pi.y - pj.y);

            aij = ( li.u * M(1,1) + li.v * M(2,1) ) * lj.u + ( li.u * M(1,2) + li.v * M(2,2) ) * lj.v; 
            aij += ( i == j ) ? eigenvalues_lower_bound : 0.0; 

            A(i,j) = aij;
        }
    }

    weights = A.householderQr().solve(lom_evaluations); 

    /* for ( size_t i = 0; i < size; ++i ) { */ 
    /*     pi = points_[i]; */ 
    /*     P(i,0) = 1; */ 
    /*     P(i,1) = pi.x; */ 
    /*     if ( poly_dim_ == 3 ) */
    /*         P(i,2) = pi.y; */ 

    /*         /1* std::cout << "P(" << i << ") = " << 1 << ", " << pi.x << ", " << pi.y << std::endl; *1/ //debug */
    /*     for ( size_t j = 0; j < size; ++j ) { */ 
    /*         pj = points_[j]; */ 
    /*         aij = k_(pi.x - pj.x, pi.y - pj.y); */ 
    /*         aij += ( i == j ) ? eigenvalues_lower_bound : 0.0; */ 

    /*         A(i,j) = aij; */ 
    /*         /1* std::cout << "A(" << i << ", " << j << ") = " << aij << std::endl; *1/ //debug */
    /*     } */
    /* } */

    /* A.block(0, size, size, poly_dim_) = P; */ 
    /* A.block(size, 0, poly_dim_, size) = P.transpose(); */ 
    /* A.block(size, size, poly_dim_, poly_dim_) = Eigen::MatrixXd::Constant(poly_dim_, poly_dim_, 0); */ 

    /* weights = A.householderQr().solve(fevaluations); */ 
    /* /1* std::cout << "weights: " << weights << std::endl; *1/ //debug */
    /* /1* std::cout << "fevals: " << fevals_ << std::endl; *1/ //debug */

    interpolant_is_set_ = true; 
    return true; 
}

void DivFreeVectorField2D::clear() 
{
    points_.clear(); 
    lambdas_.clear(); 
    lom_evals_.clear(); 
    weights_.clear(); 

    interpolant_is_set_ = false; 
}


