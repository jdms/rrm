#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>
#include <map>

#include "./surface.h"
#include "./polycurve.h"


class Object
{

    public:


        enum class Type{ STRATIGRAPHY, FAULT };


        Object();


        void setIndex( const std::size_t id_ );
        std::size_t getIndex() const;


        void setName( const std::string& name_ );
        void getName( std::string& name ) const;


        void setType( const Object::Type& type_ );
        void getType( Object::Type& type_ ) const;


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
        PolyCurve getCurve( std::size_t csection_id_ ) const;
        void removeCurve( std::size_t csection_id_ );


        std::map< std::size_t, PolyCurve > getCrossSectionCurves() const;
        void removeCrossSectionCurves();


        void addTrajectory( const PolyCurve& traj_ );
        void removeTrajectory();
        bool hasTrajectory() const;


        void setSurface( const Surface& surface_ );
        void removeSurface();


        void clear();
        void initialize();




    protected:

        void defineIndex();



    private:

        static std::size_t number_of_surfaces;

        std::size_t index;
        std::string name;

        Type type;

        bool is_editable;
        bool is_selectable;
        bool is_selected;
        bool is_visible;

        std::map< std::size_t, PolyCurve > csection_curves;
        Surface surface;
        PolyCurve trajectory;


        struct Color
        {
            int r;
            int g;
            int b;
        } color;

};

#endif // OBJECT_H
