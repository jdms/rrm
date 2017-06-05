#ifndef CROSSSECTIONITEMWRAPPER_REFACTORED_H
#define CROSSSECTIONITEMWRAPPER_REFACTORED_H

#include <QGraphicsLineItem>
#include <QPen>

class QPainter;


class CrossSectionItemWrapper_Refactored: public QGraphicsLineItem
{
    public:

        CrossSectionItemWrapper_Refactored( double w, double d );

        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w );

        void setDimensions( double w, double d );
        void getDimensions( double& w, double& d ) const;

        void setWidth( double w );
        void setDepth( double f );

        double getWidth() const;
        double getDepth() const;

        void setupPens();

        void setCurrent( bool status );

    protected:

        double width;
        double depth;

        QPen dynamic_csection_pen;
        QPen fixed_csection_pen;
        QPen current_pen;
};

#endif // CROSSSECTIONITEMWRAPPER_REFACTORED_H
