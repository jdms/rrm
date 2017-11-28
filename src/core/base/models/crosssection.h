#ifndef CROSSSECTION_H
#define CROSSSECTION_H


#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "./core/base/constants/constants.hpp"
#include "container.h"


class Volume;
class PolyCurve;

class CrossSection
{
    public:


        using Object = std::size_t;
        using ObjectsContainer = Container< Object, PolyCurve* >;


        CrossSection();
        CrossSection( const Volume* raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

        CrossSection& operator=( const CrossSection& other_ );

        void defineIndex();

        void setIndex( const std::size_t id_ );
        std::size_t getIndex() const;


        void setVolume( const Volume* raw_ );
        const Volume* getVolume() const;

        void getCoordinates( std::vector< double >& vertices_ );
        void getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                        double& minx_, double& miny_, double& minz_ ) const;


        void setDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );
        Settings::CrossSection::CrossSectionDirections getDirection() const;


        void setDepth( double depth_ );
        double getDepth() const;


        void setImagePath( const std::string& path_ );
        const std::string& getImagePath();


        void setVisible( const bool status_ );
        bool isVisible() const;


        bool addObject(const Object& id_, PolyCurve* const& curve_  );
        const PolyCurve *getObjectCurve( const Object& id_ );
        bool removeObjectCurve( const Object& id_ ) ;


        ObjectsContainer getObjects() const;
        void removeObjects();

        bool hasObjects() const;


        void clear();
        void initialize();

        static void resetAllCrossSections();

        ~CrossSection();

    private:

        std::size_t index;
        static std::size_t number_of_csections;

        double depth;
        Settings::CrossSection::CrossSectionDirections direction;

        bool is_visible;
        std::string image_path;

        ObjectsContainer objects;
        const Volume* volume;

};

#endif // CROSSSECTION_H
