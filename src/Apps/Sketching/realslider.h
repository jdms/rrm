#ifndef REALSLIDER_H
#define REALSLIDER_H

#include <QSlider>

class RealSlider: public QSlider
{

    public:

        RealSlider();


    protected:


        void setDoubleRange( double min, double max );
        void setDoubleValue( double value );
        double getDoubleValue() const ;

        void setDoubleSingleStep( double step );
        double getDoubleSingleStep() const ;


    protected:

        double max_;
        double min_;
        double step_;
        double value_;

};

#endif // REALSLIDER_H
