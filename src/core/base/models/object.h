#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>
#include <map>

#include "container.h"
#include "./surface.h"
#include "./polycurve.h"


class Object
{

    public:


        enum class Type{ STRATIGRAPHY, FAULT };
        using CrossSectionsContainer = Container< std::size_t, PolyCurve >;


        Object();


        void setIndex( const std::size_t id_ );
        std::size_t getIndex() const;


        void setName( const std::string& name_ );
        std::string getName() const;


        void setType( const Object::Type& type_ );
        Object::Type getType() const;


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


        bool isEmpty() const;


        void addCurve( std::size_t csection_id_, const PolyCurve& curve_ );
        PolyCurve getCurve( std::size_t csection_id_ );
        void removeCurve( std::size_t csection_id_ );


        Object::CrossSectionsContainer getCrossSectionCurves() const;
        void removeCrossSectionCurves();


        std::vector< std::tuple< PolyCurve, double > > getCrossSectionAndCurves(){}


        void addTrajectory( const PolyCurve& traj_ );
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




    protected:

        void defineIndex();



    private:

        static std::size_t number_of_objects;

        std::size_t index;
        std::string name;

        Type type;

        bool is_editable;
        bool is_selectable;
        bool is_selected;
        bool is_visible;

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


        const std::size_t CHANNEL_MAX_CSECTIONS = 1;
};

#endif // OBJECT_H
