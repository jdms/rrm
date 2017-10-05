#ifndef CROSSSECTION_H
#define CROSSSECTION_H


#include <iostream>
#include <string>
#include <map>
#include <vector>


class Volume;

class CrossSection
{
    public:

        enum class Direction{ X, Y, Z };

        using Object = std::size_t;
        using Curve2D = double;


        CrossSection();
        CrossSection( Volume* const& raw_, const CrossSection::Direction& dir_, const double depth_ );


        void defineIndex();

        void setIndex( const std::size_t id_ );
        std::size_t getIndex() const;


        void setVolume( Volume* const& raw_ );
        void getCoordinates( std::vector< double >& vertices_ ) const;
        void getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                        double& minx_, double& miny_, double& minz_ ) const;


        void setDirection( const CrossSection::Direction& dir_ );
        void getDirection( CrossSection::Direction& dir_ ) const;


        void setDepth( double depth_ );
        double getDepth() const;


        void setImagePath( const std::string& path_ );
        std::string getImagePath() const;


        void setVisible( const bool status_ );
        bool isVisible() const;


        void addObject( const Object& id_, Curve2D* const& curve_ );
        Curve2D* const& getObjectCurve( const Object& id_ ) const;
        void removeObjectCurve( const Object& id_ ) ;


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

        mutable std::map< Object, Curve2D* > objects;
        Volume* volume;

};

#endif // CROSSSECTION_H
