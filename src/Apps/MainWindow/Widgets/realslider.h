#ifndef REALSLIDER_H
#define REALSLIDER_H

#include <QSlider>

class RealSlider: public QSlider
{
    Q_OBJECT

    public:

    RealSlider( Qt::Orientation o );
    RealSlider( Qt::Orientation o, int disc );



    signals:

        void sliderMoved( const double& v );


    public slots:

        void setRange( double min, double max );
        int getIndex( double value ) const ;
        double getDoubleValue( int index ) const;
        double getValue() const ;

        void setValue( double value );
        void setDiscretization( int disc );


    protected slots:

        void moveInDouble( int index );



    protected:

        void updateStep();




    protected:

        double maximum;
        double minimum;
        double step;
        int discretization;

};

#endif // REALSLIDER_H
