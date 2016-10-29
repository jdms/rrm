#include "ColorBar.h"

ColorBar::ColorBar()
{

    colors.push_back( QVector3D( 0.5, 0.5, 0.5 ) );
    values.push_back( 0.0 );
    min = 0.0;
    max = 0.0;


}

void ColorBar::setSize( const int& l, const int& w )
{
    x = 10;
    y = 5;
    length = l;
    width = w;

    this->setMaximumWidth( width + 60 );
    this->setMaximumHeight( l - 5 );
//    this->setMinimumHeight( l );

}

void ColorBar::paintEvent( QPaintEvent *event )
{


    if( colors.empty() == true ) return;


    QPainter painter(this);
    QBrush brush;
    QLinearGradient gradient( QPointF( x, y ), QPointF( x, length ) );


    int number_of_colors = colors.size() - 1;



    if( number_of_colors == 0 )
        number_of_colors = 1;

    qreal id = 0.0;
    qreal id_step = (qreal)(1.0f/number_of_colors );
    qreal step = length/ number_of_colors;
    float value_step = (max - min)/(number_of_colors + 1 );

    qreal position = 0.0;
    for( int i = 0; i < number_of_colors; ++i )
    {
        gradient.setColorAt( id ,  QColor::fromRgbF( colors[ i ].x(), colors[ i ].y(), colors[ i ].z(), 1) );

        float value = max - i*value_step;
        painter.drawText( x + width + 5, y + 7 + position, QString("%1").arg( value ) );

        id += id_step;
        position += step;
    }



    if( colors.size() == 1 )
        painter.drawText( x + width + 5, length - 3, QString("%1").arg( min ) );
    else
        painter.drawText( x + width + 5, y + position - 3, QString("%1").arg( min ) );


    brush = QBrush( gradient );
    painter.fillRect( x, y, width, y + position/*length*/ , brush );

    painter.setPen(Qt::black);
    painter.drawRect( x, y, width, position/*length*/ );


}


void ColorBar::updateColorMap( const std::vector < QVector3D >& c, float m, float M/*, const std::vector< double >& v */)
{
    colors = c;
//    values = v;
    min = m;
    max = M;

    repaint();
}

void ColorBar::clear()
{
    colors.push_back( QVector3D( 0.5, 0.5, 0.5 ) );
    values.push_back( 0.0 );
    min = 0.0;
    max = 0.0;
}
