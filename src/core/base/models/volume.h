#ifndef VOLUME_H
#define VOLUME_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "./core/base/models/container.h"
#include "object.h"
#include "crosssection.h"


class Volume
{


    public:

        struct Color
        {
          int r = 255;
          int g = 0;
          int b = 0;
        };

        using ObjectsContainer = Container< std::size_t, Object* >;
        using CrossSectionsContainer = Container< std::size_t, CrossSection* >;


        Volume();


        void setOrigin( double ox_, double oy_, double oz_ );
        void getOrigin( double& ox_, double& oy_, double& oz_ ) const;


        void setGeometry( double w_, double h_, double l_ );
        void getGeometry( double& w_, double& h_, double& l_ ) const;


        void setWidth( double w_ );
        double getWidth() const;


        void setHeight( double h_  );
        double getHeight() const;


        void setLenght( double l_ );
        double getLenght() const;


        void setName( const std::string name_ );
        const std::string& getName() const;


        void getFrontFace( std::vector< double >& vertices_ ) const;
        void getRightFace( std::vector< double >& vertices_ ) const;
        void getBackFace( std::vector< double >& vertices_ ) const;
        void getLeftFace( std::vector< double >& vertices_ ) const;
        void getTopFace( std::vector< double >& vertices_ ) const;
        void getBottomFace( std::vector< double >& vertices_ ) const;



        bool addObject( std::size_t id_, Object* const& obj_ );
        bool removeObject( std::size_t id_ );
        Volume::ObjectsContainer getObjects() const;
        void removeAllObjects();



        void addTetrahedralFaces( const std::vector< std::size_t >& faces_ );
        void getFaces( std::vector< std::size_t >& faces_ ) const ;
        void setVertices( const std::vector< double >& vertices_  );
        void getVertices( std::vector< double >& vertices_  ) const;


        void addRegion( std::size_t index_, const std::vector< std::size_t >& faces_, const Volume::Color& c_ );
        void removeRegion( std::size_t index_ );
        void getRegion( std::size_t index_, std::vector< std::size_t >& faces_, Volume::Color& c_ ) const;
        void removeAllRegions();


        bool addCrossSection( std::size_t id_, CrossSection* const& csection_ );
        bool removeCrossSection( std::size_t id_ );
        Volume::CrossSectionsContainer getCrossSections() const;
        void removeAllCrossSections();


        void setVisible( bool status_ );
        bool isVisible() const;


        void setResizable( bool status_ );
        bool isResizable() const;


        bool isEmpty() const;

        void clear();
        void initialize();


    private:


        std::string name = "VOLUME";

        double origin_x;
        double origin_y;
        double origin_z;

        double width;
        double height;
        double lenght;

        bool is_visible;
        bool is_resizable;


        ObjectsContainer objects;
        CrossSectionsContainer csections;

        std::vector< double > vertices;
        std::vector< std::size_t > faces;
        std::map< std::size_t, std::vector< std::size_t > > regions;
        std::map< std::size_t, Color > regions_colors;


};

#endif // VOLUME_H
