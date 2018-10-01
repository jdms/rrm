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

    QPixmap pix( size );
    QPainter painter( &pix );
    QLinearGradient gradient(0,0, 0, value);
    gradient.setColorAt(0.0, QColor(250, 250, 250));
    gradient.setColorAt(1.0, QColor(234, 234, 234));
    QBrush brush(gradient);

    painter.setPen(QColor(0, 0, 255, 64));
    qreal xAxis = value/2;
    qreal yAxis = value/2;
    painter.drawLine(0, xAxis, value,  xAxis);
    painter.drawLine(yAxis, 0, yAxis, value);

    QPointF center = QPointF(yAxis, xAxis);

    QLineF angleline;
    angleline.setP1( center);
    angleline.setAngle( angle );
    angleline.setLength(value);
    painter.drawLine(angleline);

    QPainterPath myPath;
    myPath.moveTo(center);
    myPath.arcTo( pix.rect() , 0, angle );
    painter.setBrush( QBrush( Qt::yellow ) );
    painter.drawPath(myPath);

    painter.setRenderHint(QPainter::Antialiasing, true );
    setPixmap( pix );
}

void AnglePicture::updateAngle( double angle_ )
{
    angle = angle_;
   repaint();
}
