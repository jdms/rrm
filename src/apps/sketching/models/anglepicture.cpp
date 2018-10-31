#include <QPainter>
#include <QLinearGradient>
#include <QPainterPath>

#include "anglepicture.h"

AnglePicture::AnglePicture( const QSize& size_, double angle_ )
{
    //    pix = QPixmap( size_ );
    angle = angle_;
    size = size_;
    //    setPixmap( pix );
}

void AnglePicture::paintEvent(QPaintEvent * e)
{

    QLabel::paintEvent(e);

    int value = size.width();
    qreal xAxis = value/2;
    qreal yAxis = value/2;

    QPixmap pix( size );
    pix.fill( Qt::transparent );

    QPainter painter( &pix );


    QPen axes_pen_;
    axes_pen_.setWidth( 2 );
    axes_pen_.setColor( Qt::blue );

    painter.setPen( axes_pen_ );
    painter.drawLine(0, xAxis, value,  xAxis);

    axes_pen_.setColor( Qt::red );
    painter.setPen( axes_pen_ );
    painter.drawLine(yAxis, 0, yAxis, value);


    QPointF center = QPointF(yAxis, xAxis);

    QLineF angleline;
    angleline.setP1( center);
    angleline.setAngle( angle );
    angleline.setLength(value);

    QLineF angleline1;
    angleline1.setP1( center);
    angleline1.setAngle( 180 - angle );
    angleline1.setLength(value);

    QLineF angleline2;
    angleline2.setP1( center);
    angleline2.setAngle( 180 + angle );
    angleline2.setLength(value);

    QLineF angleline3;
    angleline3.setP1( center);
    angleline3.setAngle( 360 - angle );
    angleline3.setLength(value);


    QPen line_pen_;
    line_pen_.setColor( Qt::black );
    line_pen_.setWidth( 1 );
    painter.setPen( line_pen_ );
    painter.drawLine(angleline);
    painter.drawLine(angleline1);
    painter.drawLine(angleline2);
    painter.drawLine(angleline3);

    QPainterPath myPath, myPath1, myPath2, myPath3;
    myPath.moveTo(center);
    myPath1.moveTo(center);
    myPath2.moveTo(center);
    myPath3.moveTo(center);

    double x_ = center.x() - value/6;
    double y_ = center.y() - value/6;
    double w_ = 2*value/6;
    double h_ = 2*value/6;



    myPath.arcTo( x_, y_, w_, h_, 0, angle );
    myPath1.arcTo( x_, y_, w_, h_, 0, 180 - angle );
    myPath2.arcTo( x_, y_, w_, h_, 0, 180 + angle );
    myPath3.arcTo( x_, y_, w_, h_, 0, -angle );
//    myPath.arcTo( pix.rect() , 0, angle );
    painter.setPen( Qt::black );
    painter.setBrush( QBrush( QColor( 255, 255, 0, 125 ) ) );
    painter.drawPath(myPath);
    painter.drawPath(myPath1);
    painter.drawPath(myPath2);
    painter.drawPath(myPath3);

    painter.setRenderHint(QPainter::Antialiasing, true );
    setPixmap( pix );
}

void AnglePicture::updateAngle( double angle_ )
{
    angle = angle_;
    repaint();
}
