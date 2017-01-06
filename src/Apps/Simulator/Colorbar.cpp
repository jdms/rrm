#include "ColorBar.h"

ColorBar::ColorBar() : QWidget()
{

    colors.push_back( QVector3D( 0.5, 0.5, 0.5 ) );
    values.push_back( 0.0 );
    min = 0.0;
    max = 0.0;


	x = 10;
	y = 30;
	length = 300;
	width = 20;

	this->setMaximumWidth(width + 60);
	this->setMaximumHeight(length + 60);

}

void ColorBar::setSize( const int& l, const int& w )
{
 //   x = 10;
 //   y = 5;
	//length = 300;
	//width = 20;

 //   this->setMaximumWidth( width + 60 );
 //   this->setMaximumHeight( l - 5 );
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

        //float value = max - i*value_step;
        //painter.drawText( x + width + 5, y + 7 + position, QString("%1").arg( value ) );

        id += id_step;
        position += step;
    }

	
	QRect rect(x, y, width, length);
	int left = rect.left() + rect.width() + 10;

	float text_step = rect.height() / 10;

	QFontMetrics fm(painter.font());

	painter.drawText(left, rect.top() - fm.height(), "Max");
	//painter.drawText(left, rect.top() + rect.height() / 2, "50.0%");
	painter.drawText(left, rect.top() + rect.height() + fm.height(), "Min");

	float label_valeu = (max - min) / rect.height();

	for (auto i = 0; i < 10; i ++)
	{
		//painter.drawText(left, rect.top(), QString("%1").arg(value));
		painter.drawText(left, rect.top() + i*text_step + fm.height(), QString("%1").arg(max - i*label_valeu));
	}

    //if( colors.size() == 1 )
    //    painter.drawText( x + width + 5, length - 3, QString("%1").arg( min ) );
    //else
    //    painter.drawText( x + width + 5, y + position - 3, QString("%1").arg( min ) );


    //brush = QBrush( gradient );
	//painter.fillRect(x, y, width, length, brush);

	QPen pen;
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
	pen.setColor(qRgb(0, 0, 0));
	pen.setWidth(2);
	
    painter.setPen(pen);
	painter.setBrush(QBrush(gradient));
	painter.drawRect(x, y, width, length/*length*/);


}


void ColorBar::updateColorMap( const std::vector < QVector3D >& c, float _min, float _max/*, const std::vector< double >& v */)
{
    colors = c;
//    values = v;
    min = _min;
    max = _max;

    repaint();
}

void ColorBar::clear()
{
    colors.push_back( QVector3D( 0.5, 0.5, 0.5 ) );
    values.push_back( 0.0 );
    min = 0.0;
    max = 0.0;
}
