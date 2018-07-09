
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


        void setIndex( const std::size_t id_ );
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



        ///======================================================================






        void setType( const Settings::Objects::ObjectType& type_ );
        Settings::Objects::ObjectType getType() const;






        void setDone( const bool status_ );
        bool isDone() const;


        void saveInformation( const std::string& text_ );
        const std::string& getInformation();
        void clearInformation();


        bool isEmpty() const;


        bool addCurve( double csection_id_, const PolyCurve& curve_ );
        bool hasCurve( double csection_id_ ) const;
        PolyCurve getCurve( double csection_id_ );
        bool removeCurve( double csection_id_ );
        void updateCurve( double csection_id_, const PolyCurve& curve_ );


        Object::CrossSectionsContainer getCrossSectionCurves() const;
        void removeCrossSectionCurves();


        bool addTrajectory( const PolyCurve& traj_ );
        PolyCurve getTrajectory();
        void removeTrajectory();
        bool hasTrajectory() const;


        void clearPreviewCurves();


        bool isCurveAdmissible();
        bool isTrajectoryAdmissible();


        void setSurface( const Surface& surface_ );
        Surface getSurface() const;
        void removeSurface();


//        void setMaxMin( double maxx_, double maxy_, double maxz_,
//                        double minx_, double miny_, double minz_ );
//        void getMaxMin( double& maxx_, double& maxy_, double& maxz_,
//                        double& minx_, double& miny_, double& minz_ ) const;


        void clear();
        void initialize();

        static void resetAllObjects();


        inline void write( QJsonObject &json ) const
        {
            json[ "name" ] = QString( name.c_str() );
            json[ "text_information" ] = QString( text_information.c_str() );
            json[ "is_editable" ] = is_editable;
            json[ "is_selectable" ] = is_selectable;
            json[ "is_selected" ] = is_selected;
            json[ "is_visible" ] = is_visible;
            json[ "is_active" ] = is_active;
            json[ "is_done" ] = is_active;
            json[ "red" ] = color.red;
            json[ "green" ] = color.green;
            json[ "blue" ] = color.blue;

        }


        inline void read( const QJsonObject &json )
        {


            if (json.contains("name") && json["name"].isString())
                name = json["name"].toString().toStdString();

            if (json.contains("text_information") && json["text_information"].isString())
                text_information = json["text_information"].toString().toStdString();

            if (json.contains("is_editable") && json["is_editable"].isBool())
                is_editable =json["is_editable"].toBool();

            if (json.contains("is_selectable") && json["is_selectable"].isBool())
                is_selectable = json["is_selectable"].toBool();

            if (json.contains("is_selected") && json["is_selected"].isBool())
                is_selected = json["is_selected"].toBool();

            if (json.contains("is_visible") && json["is_visible"].isBool())
                is_visible = json["is_visible"].toBool();

            if (json.contains("is_active") && json["is_active"].isBool())
                is_active = json["is_active"].toBool();

            if (json.contains("is_done") && json["is_done"].isBool())
                is_done = json["is_done"].toBool();

            if (json.contains("red") && json["red"].isDouble())
                color.red = json["red"].toInt();

            if (json.contains("green") && json["green"].isDouble())
                color.green = json["green"].toInt();

            if (json.contains("blue") && json["blue"].isDouble())
                color.blue = json["blue"].toInt();

        }



    protected:

        void defineIndex();



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

        std::size_t index = 0;

        std::string name;
        std::string log;

        Color color;
        Point min, max;

        bool editable = true;
        bool selectable = false;
        bool selected = false;
        bool visible = true;
        bool active = true;



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


//        struct Color
//        {
//            int r = 255;
//            int g = 0;
//            int b = 0;
//        } color;

//        struct Point
//        {
//            double x = 0;
//            double y = 0;
//            double z = 0;
//        } max, min;


        const std::size_t CHANNEL_MAX_CSECTIONS = 2;
        std::set< double > user_entered;

};

#endif // OBJECT_H
