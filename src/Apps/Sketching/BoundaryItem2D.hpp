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
        void load();


        void setOriginX( int orig_x ){ origin_x = orig_x; }
        void setOriginY( int orig_y ){ origin_y = orig_y; }


        int getOriginX(){ return origin_x; }
        int getOriginY(){ return origin_y; }


        void setWidth( int w ){ width = w; }
        void setHeight( int h ){ height = h; }


        int getWidth(){ return width; }
        int getHeight(){ return height; }


    protected:

        void loadBackGroud();


    protected:


        int origin_x;
        int origin_y;

        int width;
        int height;

        QRectF boundary;
        QString image_file;
        QImage background_image;
        QPointF image_position;

};

#endif // BOUNDARYCONTROLLER_H
