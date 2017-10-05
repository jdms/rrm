#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>
#include <map>


class Object
{

    public:

        using Surface = double;
        using Curve2D = double;

        enum class Type{ STRATIGRAPHY, FAULT };


        Object();


        void setIndex( const std::size_t id_ );
        std::size_t getIndex() const;


        void setName( const std::string& name_ );
        void getName( std::string& name ) const;


        void setType( const Object::Type& type_ );
        void getType( Object::Type& type_ ) const;


        void setEditable( const bool status_ );
        bool isEditable() const;


        void setSelectable( const bool status_ );
        bool isSelectable() const;


        void setSelected( const bool status_ );
        bool isSelected() const;


        void setVisible( const bool status_ );
        bool isVisible() const;


        bool isEmpty() const;


        void addCurve( std::size_t csection_id_, const Curve2D& curve_ );
        Curve2D& getCurve( std::size_t csection_id_ ) const;
        void removeCurve( std::size_t csection_id_ );


        std::map< std::size_t, Curve2D >& getCrossSectionCurves() const;
        void removeCrossSectionCurves();


        void addTrajectory( const Curve2D& traj_ );
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

        mutable std::map< std::size_t, Curve2D > csection_curves;
        Surface surface;
        Curve2D trajectory;

};

#endif // OBJECT_H
