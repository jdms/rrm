#ifndef CROSSSECTIONITEMWRAPPER_H
#define CROSSSECTIONITEMWRAPPER_H

#include <QGraphicsLineItem>
#include <QPen>

class QPainter;


class CrossSectionItemWrapper: public QGraphicsLineItem
{
    public:

        CrossSectionItemWrapper( double w, double d );

        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w );

        void setDimensions( double w, double d );
        void getDimensions( double& w, double& d ) const;

        void setWidth( double w );
        void setDepth( double d );

        double getWidth() const;
        double getDepth() const;

        void setupPens();

        void setCurrent( bool status );

        void clear();

    protected:

        double width;
        double depth;

        QPen dynamic_csection_pen;
        QPen fixed_csection_pen;
        QPen current_pen;
};

#endif // CROSSSECTIONITEMWRAPPER_H