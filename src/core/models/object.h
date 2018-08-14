
/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
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


class Object
{

    public:


        using CrossSectionsContainer = Container< double, PolyCurve >;


        Object();
        Object( const Object& obj_ );
        Object& operator=( const Object & obj_ );

        void setLog(const std::string& log_);
        std::string getLog() const;

        ~Object();


        ///======================================================================


        virtual void setIndex( std::size_t id_ );
        std::size_t getIndex() const;


        void setName( const std::string& name_ );
        std::string getName() const;

        void setColor(int r_, int g_, int b_);
        void getColor(int& r_, int& g_, int& b_) const;

        void setBoundingBox( double xmin_, double xmax_, double ymin_, double ymax_,
                             double zmin_, double zmax_ );
        void getBoundingBox( double& xmin_, double& xmax_, double& ymin_, double& ymax_,
                             double& zmin_, double& zmax_ );

        void setMaximumBounding( double xmax_, double ymax_, double zmax_ );
        void setMinimumBounding( double xmin_, double ymin_, double zmin_ );


        void setEditable( bool status_ );
        bool isEditable() const;

        void setSelectable( bool status_ );
        bool isSelectable() const;

        void setSelected( bool status_ );
        bool isSelected() const;

        void setVisible( bool status_ );
        bool isVisible() const;

        void setActive( bool status_ );
        bool isActive() const;


        // remove after creating structural, or another structure representing obj geologicals as surfaces


        void setCrossSectionDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );

        Settings::CrossSection::CrossSectionDirections getCrossSectionDirection() const;

        const PolyCurve&  getCurve( double csection_ ) const { return PolyCurve();  }

        std::map< double, PolyCurve > getCurves() ;


        std::vector< double > getCurves2D( bool swap_ = false );
        std::vector< double > getCurves3D();
        std::vector< double > getCurves3DX();
        std::vector< double > getCurves3DY();
        std::vector< double > getCurves3DZ();

        void removeCurves();

        bool getTrajectory( PolyCurve& traj_ ) const { return false; }

        void surfaceDone() {}


        bool addCurve( double csection_id_, const PolyCurve& curve_ );
        bool removeCurve( double csection_id_ );

        std::size_t getNumberOfCrossSections() const;

        bool addTrajectory( const PolyCurve& traj_ );
        void removeTrajectory();

        void setSurface( const Surface& surface_ );
        const Surface& getSurface() const;

        void removed();

        ///======================================================================






        void setType( const Settings::Objects::ObjectType& type_ );
        Settings::Objects::ObjectType getType() const;

        void setDone( const bool status_ );
        bool isDone() const;

        void saveInformation( const std::string& text_ );
        const std::string& getInformation();
        void clearInformation();

        bool isEmpty() const;

        bool hasCurve( double csection_id_ ) const;
        PolyCurve getCurve( double csection_id_ );

        void updateCurve( double csection_id_, const PolyCurve& curve_ );

        Object::CrossSectionsContainer getCrossSectionCurves() const;
        void removeCrossSectionCurves();

        PolyCurve getTrajectory();
        bool hasTrajectory() const;

        void clearPreviewCurves();

        bool isCurveAdmissible();
        bool isTrajectoryAdmissible();

        void removeSurface();

        virtual void clear();
        void initialize();

        static void resetAllObjects();

        inline void write( QJsonObject &json ) const
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


        inline void read( const QJsonObject &json )
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

        void defineIndex();


    std::size_t index = 0;

    private:


        struct Color {
            int red = 255;
            int green = 0;
            int blue = 0;
        };

        struct Point
        {
            double x = 0.0;
            double y = 0.0;
            double z = 0.0;
        };


        std::string name;
        std::string log;

        Color color;
        Point min, max;

        bool editable = true;
        bool selectable = false;
        bool selected = false;
        bool visible = true;
        bool active = true;


        std::vector< double > used_depth;
        std::map< double, PolyCurve > csection_curves1;
        Settings::CrossSection::CrossSectionDirections direction;



        ///==========================================

        static std::size_t number_of_objects;

        Settings::Objects::ObjectType type;

        std::string text_information;

        bool is_editable;
        bool is_selectable;
        bool is_selected;
        bool is_visible;
        bool is_active;
        bool is_done;

        CrossSectionsContainer csection_curves;
        Surface surface;
        PolyCurve trajectory;

        const std::size_t CHANNEL_MAX_CSECTIONS = 2;
        std::set< double > user_entered;

};

#endif // OBJECT_H
