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
/* #include "util/prettyprint.hpp" */

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

/* DivFreeVectorField2D::DivFreeVectorField2D( Kernel k, unsigned int poly_dim ) : */ 
/*     k_(k), dim_(2) */ 
/* { */
/*     UNUSED(dim_); */ 

/*     if ( (poly_dim == 2) || (poly_dim == 3) ) */ 
/*         poly_dim_ = poly_dim; */ 
/*     else */
/*         poly_dim_ = 3; */ 
/* } */  

DivFreeVectorField2D::DivFreeVectorField2D( VFKernel k, unsigned int poly_dim ) : k_(k), poly_dim_(poly_dim) 
{
    if ( poly_dim < 2 || poly_dim > 4 )
    {
        poly_dim_ = 4;
    }
} 

Matrix22 DivFreeVectorField2D::mKernelEval( double x, double y )
{
    Matrix22 M;

    double H11 = k_.Dxx(x, y);
    double H12 = k_.Dxy(x, y);
    double H21 = k_.Dyx(x, y);
    double H22 = k_.Dyy(x, y);

    double L = H11 + H22;

    M(0,0) = H11 - L;
    M(0,1) = H12;
    M(1,0) = H21;
    M(1,1) = H22 - L;

    return M;
};

void DivFreeVectorField2D::mKernelEval( double x, double y, Matrix22 &M )
{
    double H11 = k_.Dxx(x, y);
    double H12 = k_.Dxy(x, y);
    double H21 = k_.Dyx(x, y);
    double H22 = k_.Dyy(x, y);

    double L = H11 + H22;

    M(0,0) = H11 - L;
    M(0,1) = H12;
    M(1,0) = H21;
    M(1,1) = H22 - L;
}

Vector2 DivFreeVectorField2D::operator()( double x, double y ) 
{
    Vector2 Fxy {};

    if ( !interpolantIsSet() ) { 
        return Fxy; 
    }

    const size_t size = points_.size(); 
    Matrix22 M;
    double Ui, Vi;

    for ( size_t i = 0; i < size; ++i )
    {
        /* M = mKernelEval(x - points_[i].x, y - points_[i].y); */
        mKernelEval(x - points_[i].x, y - points_[i].y, M);
        Ui = M(0,0) * lambdas_[i].u + M(0,1) * lambdas_[i].v; 
        Vi = M(1,0) * lambdas_[i].u + M(1,1) * lambdas_[i].v; 

        Fxy.u += weights_[i] * Ui; 
        Fxy.v += weights_[i] * Vi; 
    }

    if ( poly_dim_ >= 2 ) 
    {
        /* Vector2 lu = {{{ 1, 0 }}}, lv = {{{ 0, 1 }}}; */
        Fxy.u += weights_[size + 0];
        Fxy.v += weights_[size + 1];
    }

    if ( poly_dim_ >= 3 )
    {
        Fxy.u += weights_[size + 2] * x;
        Fxy.v += weights_[size + 2] * (-y);
    }

    if ( poly_dim_ >= 4 )
    {
        Fxy.u += weights_[size + 3] * y;
        Fxy.v += weights_[size + 3] * (-x);
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

Vector2 DivFreeVectorField2D::operator()( const Point2 &p ) 
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

    /* std::cout << "Point (" << p.x << ", " << p.y << ") :: Vector (" << vector_eval.x << ", " << vector_eval.y << ")\n"; */
    success &= addFunctionalEvaluation(p, lu, vector_eval.u );
    success &= addFunctionalEvaluation(p, lv, vector_eval.v );

    return success;
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

bool DivFreeVectorField2D::addFunctionalEvaluation(  const Point2 &p, const Point2 &l, double lom_eval ) 
{
    points_.push_back(p); 
    lambdas_.push_back(l);
    lom_evals_.push_back(lom_eval); 

    return true; 
} 

bool DivFreeVectorField2D::addFunctionalEvaluation(  Point2 &&p, const Point2 &l, double lom_eval ) 
{
    return addFunctionalEvaluation(p, l, lom_eval); 
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

    if ( points_.size() != lambdas_.size() ) {
        return false;
    }
    
    if ( points_.size() == 0 )
    {
        return false;
    }

    size_t size = points_.size(); 
    weights_.resize(size + poly_dim_); 

    /* auto local_fevals = fevals_; */ 
    /* for ( size_t i = 0; i < poly_dim_; ++i ) { */ 
    /*     local_fevals.push_back(0.0); */
    /* } */

    auto local_lom_evals_ = lom_evals_;
    for ( size_t i = 0; i < poly_dim_; ++i )
    {
        local_lom_evals_.push_back(0.0);
    }

    const double eigenvalues_lower_bound = 1E-9; 
    double aij = 0; 
    Point2 pi, pj; 
    Vector2 li, lj;
    Vector2 lu = {{{ 1, 0 }}}, lv = {{{ 0, 1 }}};
    Matrix22 M;
    /* Eigen::MatrixXd A(size, size); */
    Eigen::MatrixXd A(size + poly_dim_, size + poly_dim_); 
    Eigen::MatrixXd P(size, poly_dim_); 
    Eigen::Map<Eigen::VectorXd> weights( weights_.data(), size + poly_dim_, 1); 
    Eigen::Map<Eigen::VectorXd> lom_evaluations( local_lom_evals_.data(), size + poly_dim_, 1); 
    /* std::cout << "Size = " << size << "\n" << std::flush; */
    /* std::cout << "Points = " << points_ << "\n" << std::flush; */
    /* std::cout << "Points = "; */
    /* for ( size_t i = 0; i < points_.size(); ++i ) */
    /* { */
        /* std::cout << "(" << points_[i].x << ", " << points_[i].y << "), "; */
    /* } */
    /* std::cout << "\n" << std::flush; */
    /* std::cout << "Lambdas = " << lambdas_ << "\n" << std::flush; */
    /* std::cout << "LOM = " << lom_evals_ << "\n" << std::flush; */

    for ( size_t i = 0; i < size; ++i )
    {
        /* std::cout << "Getting point_i\n" << std::flush; */
        pi = points_[i]; 
        /* std::cout << "Getting lambda_i\n" << std::flush; */
        li = lambdas_[i];
        /* std::cout << "Got point_i and lambda_i\n" << std::flush; */

        if ( poly_dim_ >= 2 )
        {
            P(i,0) = li.dot(lu); 
            P(i,1) = li.dot(lv); 
            /* std::cout << "i = " << i << " :: P(i,0) = " << P(i,0) << ", P(i,1) = " << P(i, 1) << "\n" << std::flush; */
        }

        if ( poly_dim_ >= 3 )
        {
            P(i,2) = li.dot({{{pi.x, -pi.y}}});
        }

        if ( poly_dim_ >= 4 )
        {
            P(i,3) = li.dot({{{pi.y, -pi.x}}});
        }

        for ( size_t j = 0; j < size; ++j )
        {
            /* std::cout << "Getting point_j\n" << std::flush; */
            pj = points_[j]; 
            /* std::cout << "Getting lambda_j\n" << std::flush; */
            lj = lambdas_[j]; 
            /* std::cout << "Got point_j and lambda_j\n" << std::flush; */
            /* M = mKernelEval(pi.x - pj.x, pi.y - pj.y); */
            mKernelEval(pi.x - pj.x, pi.y - pj.y, M);

            aij = ( li.u * M(0,0) + li.v * M(1,0) ) * lj.u + ( li.u * M(0,1) + li.v * M(1,1) ) * lj.v; 
            aij += ( i == j ) ? eigenvalues_lower_bound : 0.0; 

            A(i,j) = aij;
            /* std::cout << "i = " << i << ", j = " << j << " :: aij = " << aij << "\n" << std::flush; */
        }
    }

    /* std::cout << "Trying to solve linear system\n" << std::flush; */
    A.block(0, size, size, poly_dim_) = P; 
    A.block(size, 0, poly_dim_, size) = P.transpose(); 
    A.block(size, size, poly_dim_, poly_dim_) = Eigen::MatrixXd::Constant(poly_dim_, poly_dim_, 0); 
    /* weights = A.householderQr().solve(lom_evaluations); */ 
    weights = A.llt().solve(lom_evaluations); 

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


