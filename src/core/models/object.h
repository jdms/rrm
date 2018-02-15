#ifndef OBJECT_H
#define OBJECT_H

#include <QJsonObject>

#include <iostream>
#include <string>
#include <map>

#include "./core/definitions/constants.hpp"
#include "container.h"
#include "./surface.h"
#include "./polycurve.h"


class Object
{

    public:


        using CrossSectionsContainer = Container< double, PolyCurve >;


        Object();


        void setIndex( const std::size_t id_ );
        std::size_t getIndex() const;


        void setName( const std::string& name_ );
        std::string getName() const;


        void setType( const Settings::Objects::ObjectType& type_ );
        Settings::Objects::ObjectType getType() const;


        void setColor( int r, int g, int b );
        void getColor( int& r, int& g, int& b ) const ;


        void setEditable( const bool status_ );
        bool isEditable() const;


        void setSelectable( const bool status_ );
        bool isSelectable() const;


        void setSelected( const bool status_ );
        bool isSelected() const;


        void setVisible( const bool status_ );
        bool isVisible() const;


        void setActive( const bool status_ );
        bool isActive() const;


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



        bool isCurveAdmissible();
        bool isTrajectoryAdmissible();


        void setSurface( const Surface& surface_ );
        Surface getSurface() const;
        void removeSurface();


        void setMaxMin( double maxx_, double maxy_, double maxz_,
                        double minx_, double miny_, double minz_ );
        void getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                        double& minx_, double& miny_, double& minz_ ) const;


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
            json[ "red" ] = color.r;
            json[ "green" ] = color.g;
            json[ "blue" ] = color.b;

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
                color.r = json["red"].toInt();

            if (json.contains("green") && json["green"].isDouble())
                color.g = json["green"].toInt();

            if (json.contains("blue") && json["blue"].isDouble())
                color.b = json["blue"].toInt();

        }



    protected:

        void defineIndex();



    private:

        static std::size_t number_of_objects;

        std::size_t index;
        std::string name;

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


        struct Color
        {
            int r = 255;
            int g = 0;
            int b = 0;
        } color;

        struct Point
        {
            double x = 0;
            double y = 0;
            double z = 0;
        } max, min;


        const std::size_t CHANNEL_MAX_CSECTIONS = 2;

};

#endif // OBJECT_H
