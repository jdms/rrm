/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file constants.hpp
 * @author Felipe
 * @author Clarissa C. Marques
 * @date Aug 23, 2015
 * @brief File containing RRM namespaces
 */

#ifndef _CORE_CONSTANTS_HPP_
#define _CORE_CONSTANTS_HPP_


#include <limits>
#include <cmath>
#include <string>
#include <vector>




namespace Math {

/**!
 * @brief Our own definition of Math constants
 */

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
        /*! */
        static const std::size_t MaxSize_t;
        /*! */
        static const std::size_t MinSize_t;

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



namespace Settings {


    enum class MeshResolution { LOW, REGULAR, GOOD, BETTER };

    class Volume
    {

        public:

            static const double VOLUME_WIDTH;
            static const double VOLUME_HEIGHT;
            static const double VOLUME_LENGTH;

            static const double VOLUME_ORIGINX;
            static const double VOLUME_ORIGINY;
            static const double VOLUME_ORIGINZ;

            static const std::string DEFAULT_VOLUME_NAME;

    };



    class CrossSection
    {
        public:

             enum class CrossSectionDirections { X, Y, Z };

             static const CrossSectionDirections DEFAULT_CSECTION_DIRECTION;
             static const CrossSectionDirections DEFAULT_TOPVIEW_DIRECTION;

             static const double INITIAL_CSECTIONX_POSITION;
             static const double INITIAL_CSECTIONY_POSITION;
             static const double INITIAL_CSECTIONZ_POSITION;

             static const std::size_t CSECTION_DISCRETIZATION;
             static const std::string DEFAULT_CSECTION_NAME;
    };



    class Stratigraphy
    {
        public:

            enum class StratigraphicRules : int {
                UNDEFINED = -1,
                NO_GEOLOGIC_RULE,
                REMOVE_ABOVE, // Remove above
                REMOVE_ABOVE_INTERSECTION, // Remove above intersection
                REMOVE_BELOW, // Remove below
                REMOVE_BELOW_INTERSECTION, // Remove below intersection
                TRUNCATE
            };


            static const StratigraphicRules DEFAULT_STRAT_RULES;
            static const std::string DEFAULT_STRAT_NAME;
            static const MeshResolution DEFAULT_MESH_RESOLUTION;
    };




    class Regions
    {
        public:

            static const std::string DEFAULT_REGION_NAME;
    };



    class Well
    {
        public:

            enum class WellType{ NONE, INJECTOR, PRODUCTOR };
            static const WellType DEFAULT_WELL_TYPE;
    };





    class Objects
    {
        public:
            enum class BoundaryRegion { NONE, ABOVE, BELOW, REGION };

            enum class ObjectType {
                NONE = 0,
                VOLUME = 1,
                CROSS_SECTION = 2,
                STRATIGRAPHY = 3,
                STRUCTURAL = 4,
                REGION = 5,
                DOMAINS = 6,
                WELL = 7
            };

            enum class ObjectProperties{ NAME, COLOR, INDEX, CURRENT, VISIBLE, SELECTABLE, SELECTED, ACTIVE };

            static const ObjectType DEFAULT_OBJECT_TYPE;
            static const BoundaryRegion DEFAULT_BOUNDARY_REGION;
            static const std::size_t MAX_CSECTION_NUMBER_FOR_CHANNEL;

    };



    class Application
    {

        public:

            enum class AppsCommands{ NEW, SAVE, LOAD, UNDO, REDO };
            enum class CommonCommands{ ADD, REMOVE, SET, GET };


            static const double APP_WIDTH;
            static const double APP_HEIGHT;

            static const double APP_ORIGIN_X;
            static const double APP_ORIGIN_Y;

            static const double APP_WIDTH_SCALE;
            static const double APP_HEIGHT_SCALE;

            static const bool DEFAULT_TOPVIEW_VISIBILITY;
            static const bool DEFAULT_CSECTION_VISIBILITY;
            static const bool DEFAULT_SIMULATOR_VISIBILITY;
            static const bool DEFAULT_SIDEBAR_VISIBILITY;

            static const std::string SHADERS_DIRECTORY;
            static const std::string IO_DIRECTORY;
            static const std::string SCREENSHOT_DIRECTORY;
            static const std::string OUTPUT_COMPUTATIONS;

    };



    class OpenGL
    {
        public:

            static const int DEPTH_BUFFER;
            static const int SAMPLES;
    };


}



namespace Variables {

    static const bool ON = true;
    static const bool OFF = false;

}




#endif /* _CORE_CONSTANTS_HPP_ */
