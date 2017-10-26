#ifndef CROSSSECTION_H
#define CROSSSECTION_H


#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "container.h"


class Volume;
class PolyCurve;

class CrossSection
{
    public:

        enum class Direction{ X, Y, Z };

        using Object = std::size_t;
        using ObjectsContainer = Container< Object, PolyCurve* >;


        CrossSection();
        CrossSection( Volume* const& raw_, const CrossSection::Direction& dir_, const double depth_ );


        void defineIndex();

        void setIndex( const std::size_t id_ );
        std::size_t getIndex() const;


        void setVolume( Volume* const& raw_ );
        Volume* getVolume() const;

        void getCoordinates( std::vector< double >& vertices_ );
        void getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                        double& minx_, double& miny_, double& minz_ ) const;


        void setDirection( const CrossSection::Direction& dir_ );
        CrossSection::Direction getDirection() const;


        void setDepth( double depth_ );
        double getDepth() const;


        void setImagePath( const std::string& path_ );
        std::string getImagePath() const;


        void setVisible( const bool status_ );
        bool isVisible() const;


        bool addObject(const Object& id_, PolyCurve* const& curve_  );
        const PolyCurve *getObjectCurve( const Object& id_ );
        bool removeObjectCurve( const Object& id_ ) ;


        ObjectsContainer getObjects() const;
        void removeObjects();


        void clear();
        void initialize();


    private:

        std::size_t index;
        static std::size_t number_of_csections;

        double depth;
        Direction direction;

        bool is_visible;
        std::string image_path;

        ObjectsContainer objects;
        Volume* volume;

};

#endif // CROSSSECTION_H
