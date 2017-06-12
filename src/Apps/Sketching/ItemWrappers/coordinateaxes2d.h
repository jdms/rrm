#ifndef COORDINATEAXES2D_H
#define COORDINATEAXES2D_H

#include <QGraphicsLineItem>
#include <QPainter>


class CoordinateAxes2d: public QGraphicsLineItem
{
    public:

        CoordinateAxes2d();


        void setAxisXLenght( double l );
        void setAxisYLenght( double l );

    protected:

         void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    protected:

        double axisx_length;
        double axisy_length;

        QString text_axisx;
        QString text_axisy;

};

#endif // COORDINATEAXES2D_H
