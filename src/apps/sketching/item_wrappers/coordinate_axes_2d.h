#ifndef COORDINATEAXES2D_H
#define COORDINATEAXES2D_H

#include <QGraphicsLineItem>
#include <QPainter>
#include <QColor>

class CoordinateAxes2d: public QGraphicsLineItem
{
    public:

        enum class Plane{ XY, XZ };


        CoordinateAxes2d( const Plane& pl = Plane::XY );

        void setPlane( const Plane& pl );
        void setAxisXLenght( double l );
        void setAxisYLenght( double l );

        QRectF boundingRect() const;

    protected:

         void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
         void drawAxisX( QPainter* p );
         void drawAxisY( QPainter* p );

    protected:

        double axisx_length;
        double axisy_length;

        QString text_axisx;
        QString text_axisy;

        const QColor width_color = QColor( 255, 0, 0 );
        const QColor height_color = QColor( 0, 255, 0 );
        const QColor depth_color = QColor( 0, 0, 255 );

        QColor current_y;
        Plane plane;
        int scale;

};

#endif // COORDINATEAXES2D_H
