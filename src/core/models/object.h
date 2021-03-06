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
 * @file object.h
 * @author Clarissa C. Marques
 * @brief File containing the class Object
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <QJsonObject>

#include <iostream>
#include <string>
#include <map>
#include <set>

#include "./core/definitions/constants.hpp"
#include "container.h"
#include "./surface.h"
#include "./polycurve.h"

/**
 *  A graphical representation to the object to save its metadata
 */


class Object
{

    public:


       /**
        *  \var CrossSectionsContainer a simplified name to mean a container of cross-sections
        */
        using CrossSectionsContainer = Container< double, PolyCurve >;


        /**
        * Constructor.
        */
        Object();


        /**
        * Copy constructor.
        * @param obj_ a const reference to another Object instance
        */
        Object( const Object& obj_ );


        /**
        * Assignment operator.
        * @param obj_ a const reference to another Object instance
        */
        Object& operator=( const Object & obj_ );


        /**
        * Destructor.
        */
        ~Object();


        /**
        * Method to set a new index to the object
        * @param id a new index to the object
        * @return void.
        */
        virtual void setIndex( std::size_t id_ );


        /**
        * Method to return the index of the object
        * @return std::size_t the index of the object
        */
        std::size_t getIndex() const;


        /**
        * Method to set a name to the object
        * @param name a new name to the object
        * @return void.
        */
        void setName( const std::string& name_ );


        /**
        * Method to return the name of the object
        * @return std::string the name of the object
        */
        std::string getName() const;


        /**
        * Method to set the color of the object
        * @param r the red component of the color (integer)
        * @param g the green component of the color (integer)
        * @param b the blue component of the color (integer)
        * @return void.
        */
        void setColor(int r_, int g_, int b_);


        /**
        * Method to get the color of the object
        * @param r reference to the red component of the color (integer)
        * @param g reference to the green component of the color (integer)
        * @param b reference to the blue component of the color (integer)
        * @return void.
        */
        void getColor(int& r_, int& g_, int& b_) const;


        /**
        * Method to set the bounding box of the object
        * @param maxx the maximum value of X
        * @param maxy the maximum value of Y
        * @param maxz the maximum value of Z
        * @param minx the minimum value of X
        * @param miny the minimum value of Y
        * @param minz the minimum value of Z
        * @return void.
        */
        void setBoundingBox( double xmin_, double xmax_, double ymin_, double ymax_,
                             double zmin_, double zmax_ );


        /**
        * Method to get the bounding box of the object
        * @param maxx reference to the maximum value of X
        * @param maxy reference to the maximum value of Y
        * @param maxz reference to the maximum value of Z
        * @param minx reference to the minimum value of X
        * @param miny reference to the minimum value of Y
        * @param minz reference to the minimum value of Z
        * @return void.
        */
        void getBoundingBox( double& xmin_, double& xmax_, double& ymin_, double& ymax_,
                             double& zmin_, double& zmax_ );


        /**
        * Method to set the maximum point of the object bounding box
        * @param maxx the maximum value of X
        * @param maxy the maximum value of Y
        * @param maxz the maximum value of Z
        * @return void.
        */
        void setMaximumBounding( double xmax_, double ymax_, double zmax_ );


        /**
        * Method to set the minimum point of the object bounding box
        * @param minx reference to the minimum value of X
        * @param miny reference to the minimum value of Y
        * @param minz reference to the minimum value of Z
        * @return void.
        */
        void setMinimumBounding( double xmin_, double ymin_, double zmin_ );


        /**
        * Method to set the object editable or not
        * @param status if the value is true the object is editable and false otherwise
        * @return void.
        */
        void setEditable( bool status_ );


        /**
        * Method to check if the object is editable or not
        * @return returns true if the object is editable and false otherwise
        */
        bool isEditable() const;


        /**
        * Method to set the object selectable or not
        * @param status if the value is true the object is selectable and false otherwise
        * @return void.
        */
        void setSelectable( bool status_ );


        /**
        * Method to check if the object is selectable or not
        * @return returns true if the object is selectable and false otherwise
        */
        bool isSelectable() const;


        /**
        * Method to set the object selected or not
        * @param status if the value is true the object is selected and false otherwise
        * @return void.
        */
        void setSelected( bool status_ );


        /**
        * Method to check if the object is selected or not
        * @return returns true if the object is selected and false otherwise
        */
        bool isSelected() const;


        /**
        * Method to set the object visible or not
        * @param status if the value is true the object is visible and false otherwise
        * @return void.
        */
        void setVisible( bool status_ );


        /**
        * Method to check if the object is visible or not
        * @return returns true if the object is visible and false otherwise
        */
        bool isVisible() const;


        /**
        * Method to set the object active or not
        * @param status if the value is true the object is active and false otherwise
        * @return void.
        */
        void setActive( bool status_ );


        /**
        * Method to check if the object is active or not
        * @return returns true if the object is active and false otherwise
        */
        bool isActive() const;


        /**
        * Method to set the cross-section direction in which the object was created
        * @param dir the cross-section direction in which the object was created
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void setCrossSectionDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method to get the cross-section direction in which the object was created
        * @see Settings::CrossSection::CrossSectionDirections
        * @return CrossSectionDirections the cross-section direction in which the object was created
        */
        Settings::CrossSection::CrossSectionDirections getCrossSectionDirection() const;


        /**
        * Method to remove all cross-sections curves
        * @return void.
        */
        void removeCrossSectionCurves();


        /**
        * Method to remove all preview curves, i.e., the curves that were not added by the user
        * @return void.
        */
        void clearPreviewCurves();


        /**
        * Method to get the curves that were added to the object
        * @see PolyCurve
        * @return std::map< double, PolyCurve > a mapping between the cross-section depth and the curve associated
        */
        std::map< double, PolyCurve > getCurves() ;


        /**
        * Method to get all the curves in a vector of double
        * This method allows swap the coordinates if desired
        * @param swap if swap is true the coordinates should be swapped before returning
        * @return std::vector< double > all the curves in a vector of double
        */
        std::vector< double > getCurves2D( bool swap_ = false );


        /**
        * Method to get the curve in coordinates 3d, considering the depth and the direction that the curve was done
        * @return std::vector< double > all the curves in a vector of double
        */
        std::vector< double > getCurves3D();


        /**
        * Method to get the curve in coordinates 3d, adding the depth that each curve was made in the X coordinate
        * @return std::vector< double > all the curves in a vector of double
        */
        std::vector< double > getCurves3DX();


        /**
        * Method to get the curve in coordinates 3d, adding the depth that each curve was made in the Y coordinate
        * @return std::vector< double > all the curves in a vector of double
        */
        std::vector< double > getCurves3DY();


        /**
        * Method to get the curve in coordinates 3d, adding the depth that each curve was made in the Z coordinate
        * @return std::vector< double > all the curves in a vector of double
        */
        std::vector< double > getCurves3DZ();


        /**
        * Method to remove all curves from the object
        * @return void.
        */
        void removeCurves();


        /**
        * Method to add a curve to the object
        * @param csection_id the cross-section depth where the curve_ was made
        * @param curve the curve (sketch) made by the user
        * @see PolyCurve
        * @return bool returns true if the curve was added properly and false otherwise
        */
        bool addCurve( double csection_id_, const PolyCurve& curve_ );


        /**
        * Method to remove the curve made in the cross-section csection_id_
        * @param csection_id the cross-section depth where the curve_ was made
        * @return bool returns true if the curve was removed properly and false otherwise
        */
        bool removeCurve( double csection_id_ );

        /**
        * Method to check if exists a curve added by the user in the given cross-section depth
        * @param csection_id the depth of the cross-section
        * @return bool return true if exists a curve in the given cross-section depth
        */
        bool hasCurve( double csection_id_ ) const;


        /**
        * Method to return the curve from the given cross-section depth
        * @param csection_id the depth of the cross-section
        * @see PolyCurve
        * @return PolyCurve the curve existent in the given cross-section depth
        */
        PolyCurve getCurve( double csection_id_ );


        /**
        * Method to update the curve of the given cross-section depth
        * @param csection_id the depth of the cross-section
        * @param curve the curve to replace the old one
        * @see PolyCurve
        * @return void.
        */
        void updateCurve( double csection_id_, const PolyCurve& curve_ );


        /**
        * Method to remove all the level curves from the object
        * @return void.
        */
        void removeLevelCurves();


        /**
        * Method to return the number of cross-sections used, i.e., the number of curves
        * @return std::size_t the number of cross-sections used, i.e., the number of curves
        */
        std::size_t getNumberOfCrossSections() const;


        /**
        * Method to add a trajectory to the object
        * @param traj the trajectory made by the user
        * @see PolyCurve
        * @return bool returns true if the trajectory was added properly and false otherwise
        */
        bool addTrajectory( const PolyCurve& traj_ );


        /**
        * Method to remove the trajectory from the object
        * @return void.
        */
        void removeTrajectory();


        /**
        * Method to return the trajectory of the object, if it exists
        * @see PolyCurve
        * @return PolyCurve the trajectory of the object
        */
        PolyCurve getTrajectory();


        /**
        * Method to check if exists a trajectory in the object
        * @return bool returns true if exists a trajectory and false otherwise
        */
        bool hasTrajectory() const;


        /**
        * Method to set the surface generate from the curves added to the object
        * @param surface the surface generate from the curves added to the object
        * @return void.
        */
        void setSurface( const Surface& surface_ );


        /**
        * Method to return the surface generate from the curves added to the object
        * @return the surface generate from the curves added to the object
        */
        const Surface& getSurface() const;


        /**
        * Method to check if the object already has a surface
        * @return bool returns true if the object already has a surface and false otherwise
        */
        bool hasSurface(){ return !surface.isEmpty(); }


        /**
        * Method to remove the object surface
        * @return void.
        */
        void removeSurface();


        /**
        * Method to remove all the curves, trajectories and surfaces, except if the object was made in the height direction
        * This method was implement because after a surface is created all the curves should be created automatically by
        * the rules_processor. If the object was created in the Height direction the level curves should not be removed, since
        * the rules_processor do not return the level-curves intersection.
        * @return void.
        */
        void removed();


        /**
        * Method to set the type of the object.
        * Currently, the types available are: 'NONE', 'VOLUME', 'CROSS_SECTION', 'STRATIGRAPHY', 'STRUCTURAL', 'REGION',
        * 'DOMAINS', 'WELL'
        * @param type the type of the object
        * @see Settings::Objects::ObjectType
        * @return void.
        */
        void setType( const Settings::Objects::ObjectType& type_ );


        /**
        * Method to return the type of the object.
        * Currently, the types available are: 'NONE', 'VOLUME', 'CROSS_SECTION', 'STRATIGRAPHY', 'STRUCTURAL', 'REGION',
        * 'DOMAINS', 'WELL'
        * @see Settings::Objects::ObjectType
        * @return ObjectType the type of the object
        */
        Settings::Objects::ObjectType getType() const;


        /**
        * Method to set if the object is done.
        * The object is considered done when the surface is created and assigned to it.
        * If the object is done, it is not allowed to add more curves to it.
        * @param status if status is true the object is done and false otherwise
        * @return void.
        */
        void setDone( const bool status_ );


        /**
        * Method to check if the object is done.
        * The object is considered done when the surface is created and assigned to it.
        * If the object is done, it is not allowed to add more curves to it.
        * @return bool returns true if the object is done and false otherwise
        */
        bool isDone() const;


        /**
        * Method to set an object log information
        * @param text an object log information
        * @return void.
        */
        void saveInformation( const std::string& text_ );


        /**
        * Method to get the object log information
        * @return std::string object log information
        */
        const std::string& getInformation();


        /**
        * Method to clear the object log information
        * @return void.
        */
        void clearInformation();


        /**
        * Method to check if the object is empty
        * An object is empty if there is no curve or level curve added to it.
        * @return bool return true if there is no curve or level curve added to it, and false otherwise
        */
        bool isEmpty() const;


        /**
        * Method to check if the object still allows to add curves, i.e., the object is not done yet
        * @return bool returns true if the object is not done and false otherwise
        */
        bool isCurveAdmissible();


        /**
        * Method to check if the object still allows to add trajectories, i.e., the object is not done yet
        * The method return false if the object already has a trajectory or there are more than a cross-section curve added
        * @return bool returns true if the object still allows to add trajectories and false otherwise
        */
        bool isTrajectoryAdmissible();


        /**
        * This method removes all metadata from the structure.
        * @return void.
        */
        virtual void clear();


        /**
        * This method sets the default values for the data.
        * @return void.
        */
        void initialize();


        /**
        * This method reset the number of cross-section to zero in all application
        * @return void.
        */
        static void resetAllObjects();


        /**
        * This method saves the object metadata into a file using json
        * @param json_ a JSON object from Qt
        * @see QJsonObject
        * @return void.
        */
        inline virtual void write( QJsonObject &json ) const
        {
            json[ "name" ] = QString( name.c_str() );
            json[ "text_information" ] = QString( text_information.c_str() );
            json[ "is_editable" ] = editable;
            json[ "is_selectable" ] = selectable;
            json[ "is_selected" ] = selected;
            json[ "is_visible" ] = visible;
            json[ "is_active" ] = active;
            json[ "is_done" ] = is_done;
            json[ "red" ] = color.red;
            json[ "green" ] = color.green;
            json[ "blue" ] = color.blue;


            double object_type_ = 0;
            if( type == Settings::Objects::ObjectType::VOLUME )
                object_type_ = 1;
            else if( type == Settings::Objects::ObjectType::CROSS_SECTION )
                object_type_ = 2;
            else if( type == Settings::Objects::ObjectType::STRATIGRAPHY )
                object_type_ = 3;
            else if( type == Settings::Objects::ObjectType::STRUCTURAL )
                object_type_ = 4;
            else if( type == Settings::Objects::ObjectType::REGION )
                object_type_ = 5;
            else if( type == Settings::Objects::ObjectType::WELL )
                object_type_ = 6;

            json[ "type" ] = object_type_;


            double object_direction_ = 0;
            if( direction == Settings::CrossSection::CrossSectionDirections::Y )
                object_direction_ = 1;
            else if( direction == Settings::CrossSection::CrossSectionDirections::Z )
                object_direction_ = 2;

            json[ "direction" ] = object_direction_;

        }


        /**
        * This method reads the object metadata from a file using json
        * @param json_ a reference to a JSON object from Qt
        * @see QJsonObject
        * @return void.
        */
        inline virtual void read( const QJsonObject &json )
        {


            if (json.contains("name") && json["name"].isString())
                name = json["name"].toString().toStdString();

            if (json.contains("text_information") && json["text_information"].isString())
                text_information = json["text_information"].toString().toStdString();

            if (json.contains("is_editable") && json["is_editable"].isBool())
                editable =json["is_editable"].toBool();

            if (json.contains("is_selectable") && json["is_selectable"].isBool())
                selectable = json["is_selectable"].toBool();

            if (json.contains("is_selected") && json["is_selected"].isBool())
                selected = json["is_selected"].toBool();

            if (json.contains("is_visible") && json["is_visible"].isBool())
                visible = json["is_visible"].toBool();

            if (json.contains("is_active") && json["is_active"].isBool())
                active = json["is_active"].toBool();

            if (json.contains("is_done") && json["is_done"].isBool())
                is_done = json["is_done"].toBool();

            if (json.contains("red") && json["red"].isDouble())
                color.red = json["red"].toInt();

            if (json.contains("green") && json["green"].isDouble())
                color.green = json["green"].toInt();

            if (json.contains("blue") && json["blue"].isDouble())
                color.blue = json["blue"].toInt();

            if (json.contains("type") && json["type"].isDouble() )
            {
                Settings::Objects::ObjectType object_type_ = Settings::Objects::ObjectType::NONE;

                if( json["type"] == 1 )
                    object_type_ = Settings::Objects::ObjectType::VOLUME;
                else if( json["type"] == 2 )
                    object_type_ = Settings::Objects::ObjectType::CROSS_SECTION;
                else if( json["type"] == 3 )
                    object_type_ = Settings::Objects::ObjectType::STRATIGRAPHY;
                else if( json["type"] == 4 )
                    object_type_ = Settings::Objects::ObjectType::STRUCTURAL;
                else if( json["type"] == 5 )
                    object_type_ = Settings::Objects::ObjectType::REGION;
                else if( json["type"] == 6 )
                    object_type_ = Settings::Objects::ObjectType::WELL;

                type = object_type_;
            }


            if (json.contains("direction") && json["direction"].isDouble() )
            {
                Settings::CrossSection::CrossSectionDirections object_direction_ = Settings::CrossSection::CrossSectionDirections::X;

                if( json["type"] == 1 )
                    object_direction_ = Settings::CrossSection::CrossSectionDirections::Y;
                else if( json["type"] == 2 )
                    object_direction_ = Settings::CrossSection::CrossSectionDirections::Z;

                direction = object_direction_;
            }

        }



    protected:


        /**
        * This method defines automatically the index of the stratigraphy
        * @return void.
        */
        void defineIndex();


        std::size_t index = 0;                                                  /**< The index of the object */


    protected:
        /**
        * @struct Color
        * @brief It is a data structure to represent color
        * @var red red component of a color
        * @var green green component of a color
        * @var blue blue component of a color
        */
        struct Color {
            int red = 255;
            int green = 0;
            int blue = 0;
        };

        Color color;                                                        /**< The color of the object*/

        std::string name;                                                   /**< The name of the object */


    private:
        /**
        * @struct Point
        * @brief It is a data structure to represent a point
        * @var x x coordinate of the point
        * @var y y coordinate of the point
        * @var z z coordinate of the point
        */
        struct Point
        {
            double x = 0.0;
            double y = 0.0;
            double z = 0.0;
        };


        std::string log;

        Settings::Objects::ObjectType type;                                 /**< The type of the object */

        Point min;                                                          /**< The minimum point of the bounding box */

        Point max;                                                          /**< The maximum point of the bounding box */

        bool editable = true;                                               /**< A boolean to define if the object is editable or not */

        bool selectable = false;                                            /**< A boolean to define if the object is selectable or not */

        bool selected = false;                                              /**< A boolean to define if the object is selected or not */

        bool visible = true;                                                /**< A boolean to define if the object is visible or not */

        bool active = true;                                                 /**< A boolean to define if the object is active or not */

        bool is_done = false;                                               /**< A boolean to define if the object is done or not */

        std::set< double > user_entered;                                    /**< A vector of the cross-sections depth which were used by the user */

        std::map< double, PolyCurve > csection_curves1;                     /**< The mapping between the cross-sections depths and the curves contained in them */

        std::map< double, PolyCurve > level_curves1;                        /**< The mapping between the height of the cross-sections and the curves done in them */

        Settings::CrossSection::CrossSectionDirections direction;           /**< The direction which was made the object */

        static std::size_t number_of_objects;                               /**< The total number of objects in the application, i.e., the number of instances of this
                                                                                  class existents in the application */

        std::string text_information;                                       /**< The log information of the object */

        Surface surface;                                                    /**< The surface of the object */

        PolyCurve trajectory;                                               /**< The trajectory of the object */

        const std::size_t CHANNEL_MAX_CSECTIONS = 2;                        /**< The maximum number of cross-section allowed to create a channel */

 //        CrossSectionsContainer csection_curves;

};

#endif // OBJECT_H
