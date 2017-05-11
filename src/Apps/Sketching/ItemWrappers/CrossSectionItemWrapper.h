#ifndef CROSSSECTIONITEMWRAPPER_H
#define CROSSSECTIONITEMWRAPPER_H

#include <QGraphicsLineItem>
#include <QPainter>

class CrossSectionItemWrapper: public QGraphicsLineItem
{

    public:

        CrossSectionItemWrapper( double width_, double depth_ );

        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w );

        inline void setDimensions( double width_, double depth_ ){ width = width_; depth = depth_; }
        inline void getDimensions( double& width_, double& depth_ ){ width_ = width; depth_ = depth; }
        inline double getDepth(){ return depth; }
        inline void setDepth( double depth_ ){ depth = depth_; update(); }

        inline void setCurrent( bool status_ )
        {
            if( status_ == true )
            {
                pen_color.setStyle( Qt::SolidLine );
                pen_color.setColor( Qt::yellow );
                pen_color.setWidth ( 3 );
            }
            else
            {
                pen_color.setStyle( Qt::DotLine );
                pen_color.setColor( Qt::black );
                pen_color.setWidth ( 2 );
            }
        }

    protected:

        double width;
        double depth;

        QPen pen_color;

};

#endif // CROSSSECTIONITEMWRAPPER_H
