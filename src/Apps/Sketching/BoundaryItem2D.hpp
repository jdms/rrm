#ifndef BOUNDARYITEM2D_H
#define BOUNDARYITEM2D_H

#include <vector>

#include <QPainter>
#include <QGraphicsPathItem>
#include <QPixmap>
#include <QString>

#include "./src/Core/Geology/Models/BoundaryRenderable.h"


#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"

using namespace std;


class BoundaryItem2D: public QGraphicsPathItem, public BoundaryRenderable
{


    public:

        BoundaryItem2D();
        ~BoundaryItem2D();

        void edit( const qreal& x, const qreal& y, const qreal& width, const qreal& height );

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );
        QRectF boundingRect() const;

        void clear();
        void update( const Eigen::Affine3f &m );
//        void load();


        int getOriginX() const { return boundary.x(); }
        int getOriginY() const { return boundary.y(); }

        int getWidth() const { return boundary.width(); }
        int getHeight() const { return boundary.height(); }



    protected:


        QRectF boundary;
        QPointF image_position;

};

#endif // BOUNDARYCONTROLLER_H
