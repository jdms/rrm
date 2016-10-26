#ifndef BOUNDARYITEM2D_H
#define BOUNDARYITEM2D_H

#include <vector>

#include <QPainter>
#include <QGraphicsPathItem>
#include <QPixmap>
#include <QString>

#include "Model/BoundaryRenderable.h"


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
        void setBackGroundImage( const QString& url );

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

        void loadBackGroud();


    protected:


        QRectF boundary;
        QString image_file;
        QImage background_image;
        QPointF image_position;

};

#endif // BOUNDARYCONTROLLER_H
