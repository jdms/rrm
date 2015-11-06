/*
 * Constants.hpp
 *
 *  Created on: Aug 23, 2015
 *      Author: felipe
 */

#ifndef _CORE_CONSTANTS_HPP_
#define _CORE_CONSTANTS_HPP_

#include <limits>
#include <cmath>

namespace Math {

class Constants {

	/// TODO Infinity and Zero are for double.

	public:
		/*! @name Constants */
		//@{
		/*!  */
		static const double Pi;
		/*! */
	    static const double HalfPi;
	    /*!
	     * \code
	     *  /f[
		 * \sqrt{ 2 }
		 * /f]
	     * \endcode
	     *  */
	    static const double SqrtTwo;
	    /*!
	     * \code
	     *  /f[
		 * \sqrt{ 3 }
		 * /f]
	     * \endcode
	     * */
	    static const double SqrtThree;
	    /*!
	     *  \code
	     *  /f[
		 * 1 / \sqrt{ 2 }
		 * /f]
	     * \endcode
	     * */
	    static const double InverseSqrtTwo;
	    /*!
	     * \code
	     *  /f[
		 * 1 / \sqrt{ 3 }
		 * /f]
	     * \endcode
	     *  */
	    static const double InverseSqrtThree;
	    /*!
	     * * \code
	     * /f[
		 * 1 / 3
		 * /f]
	     * \endcode
	     * */
	    static const double InverseThree;
	    /*! Dregrees to radians conversion
	     * Only multiply the number in degrees by it
	     * */
	    static const double	Degree2Radian;
	    /*! Radians to dregrees conversion
	     * * Only multiply the number in radians by it
	     * */
	    static const double	Radian2Degree;
	    /*! */
	    static const double	Infinty;
	    /*! */
	    static const double 	Epsilon;
	    //@}

		/*! @name functions ... */
		//@{
	    /*! */
	    static bool CloseEnough(double p, double q)
	    {
	        // Determines whether the two double-point values f1 and f2 are
	        // close enough together that they can be considered equal.

	        return fabs((p - q) / ((q == 0.0) ? 1.0 : q)) < Epsilon;
	    }
	    /*!
	     * \attention maybe move to another package
	     * */

		static double Hypotenuse (double x, double y)
		{
			return std::sqrt( (x * x) + (y * y) );
		}

	};

} /* namespace Math */

#endif /* _CORE_CONSTANTS_HPP_ */
