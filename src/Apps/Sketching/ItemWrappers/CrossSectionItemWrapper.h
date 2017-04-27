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

    protected:

        double width;
        double depth;


};

#endif // CROSSSECTIONITEMWRAPPER_H
