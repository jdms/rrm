#ifndef BOUNDARYITEM_H
#define BOUNDARYITEM_H

#include <vector>

#include <QPainter>
#include <QGraphicsPathItem>
#include <QPixmap>
#include <QString>

#include "model_new/Boundary.hpp"


using namespace std;


class BoundaryRenderable
{

    public:

        BoundaryRenderable(){}
        ~BoundaryRenderable(){}


        inline void setGeoData( Boundary* const& b ){ bd = b; }
        inline Boundary* getGeoData(){ return bd; }

        virtual void init(){}
        virtual void draw(){}


    protected:

        Boundary* bd;


};

#endif // BOUNDARYCONTROLLER_H
