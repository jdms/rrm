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


        bool addCrossSection( std::size_t id_, CrossSection* const& csection_ );
        bool removeCrossSection( std::size_t id_ );
        Volume::CrossSectionsContainer getCrossSections() const;


        void setVisible( bool status_ );
        bool isVisible() const;


        void setResizable( bool status_ );
        bool isResizable() const;


        void clear();
        void initialize();


    private:

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

};

#endif // VOLUME_H
