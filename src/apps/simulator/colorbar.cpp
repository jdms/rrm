/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "colorbar.h"

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

    this->label_step = 10;

    this->setMaximumWidth(width + 100);
    this->setMaximumHeight(length + 60);

    this->setMinimumWidth(width + 100);
    this->setMinimumHeight(length + 60);

    is_gradient = true;

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


    if (colors.empty() == true)
    {
        return;
    }


    QPainter painter(this);
    QBrush brush;
    QLinearGradient gradient( QPointF( x, y ), QPointF( x, length ) );

    int number_of_colors = colors.size() - 1;

    if( number_of_colors == 0 )
        number_of_colors = 1;



    if( is_gradient ){

        qreal id = 0.0;
        qreal id_step = (qreal)(1.0f/number_of_colors );
        qreal step = length/ number_of_colors;
        float value_step = (max - min)/(number_of_colors + 1 );

        qreal position = 0.0;
        for( int i = 0; i < number_of_colors; ++i )
        {
            gradient.setColorAt( id ,  QColor::fromRgbF( colors[ i ].x(), colors[ i ].y(), colors[ i ].z(), 1) );

            id += id_step;
            position += step;
        }

        QRect rect(x, y, width, length);
        int left = rect.left() + rect.width() + this->label_step;

        QFontMetrics fm(painter.font());

        float text_step = (rect.height() - 2*fm.height()) / (this->label_step - 1);


        painter.drawText(left, rect.top() - fm.height(), "Max");
        painter.drawText(left, rect.top() + rect.height() + fm.height(), "Min");

        float label_valeu = (max - min) / (this->label_step - 1);

        QString number;

        for (auto i = 0; i < this->label_step; i++)
        {
            //number = QString::number(max - i*label_valeu);
            //if (number.size() > 4)
            //{
                number = QString::number(max - i*label_valeu,'e',2);
            //}

            painter.drawText(left, rect.top() + i*text_step + fm.height(), number);
        }

        QPen pen;
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        pen.setColor(qRgb(0, 0, 0));
        pen.setWidth(2);

        painter.setPen(pen);
        painter.setBrush(QBrush(gradient));
        painter.drawRect(x, y, width, length);

    }
    else
    {


        qreal step = (qreal)(1.0/colors.size());

        qreal dl = 0 ;
        qreal step_text = (qreal)( length/colors.size());
        for( auto i = 0; i <  colors.size(); ++i )
        {
            gradient.setColorAt( dl ,  QColor::fromRgbF( colors[ i ].x(), colors[ i ].y(), colors[ i ].z(), 1) );
            gradient.setColorAt( dl + step ,  QColor::fromRgbF( colors[ i ].x(), colors[ i ].y(), colors[ i ].z(), 1) );
            dl += step;
        }



        QRect rect(x, y, width, length);
        int left = rect.left() + rect.width() + colors.size();

        QFontMetrics fm(painter.font());


        int ncolors = (colors.size() - 1);
        if( ncolors == 0 ) ncolors = 1;
        float text_step = (rect.height() - 2*fm.height()) / ncolors;


        painter.drawText(left, rect.top() - fm.height(), "Max");
        painter.drawText(left, rect.top() + rect.height() + fm.height(), "Min");


        QString number;

        for (auto i = 0; i < colors.size(); i++)
        {
            number = QString::number( values[ i ],'e', 2 );
            painter.drawText(left, rect.top() + i*text_step + fm.height(), number);
        }


        QPen pen;
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        pen.setColor(qRgb(0, 0, 0));
        pen.setWidth(2);

        painter.setPen(pen);
        painter.setBrush(QBrush(gradient));
        painter.drawRect(x, y, width, length);


    }




}

void ColorBar::updateColorMap( const std::vector < QVector3D >& c, const std::vector< int >& ids_, const std::vector< double >& values_ )
{
    is_gradient = false;
    ids = ids_;
    values = values_;
    colors = c;

    std::reverse(colors.begin(), colors.end());
     std::reverse(values.begin(), values.end());

    repaint();
}

void ColorBar::updateColorMap(const std::vector < QVector3D >& c, float _min, float _max, int label_step)
{
    is_gradient = true;

    colors = c;
    // The color is paint top down.
    std::reverse(colors.begin(), colors.end());
    this->label_step = label_step;
    min = _min;
    max = _max;

    repaint();
}

void ColorBar::clear()
{

    colors.clear();
    colors.push_back( QVector3D( 0.5, 0.5, 0.5 ) );
    values.clear();
    values.push_back( 0.0 );
    min = 0.0;
    max = 0.0;
}
