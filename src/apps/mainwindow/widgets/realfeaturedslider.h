#ifndef REALFEATUREDSLIDER_H
#define REALFEATUREDSLIDER_H


#include <QSlider>
#include "./core/base/models/slider.h"


class RealFeaturedSlider: public Slider
{
    Q_OBJECT

    public:

        RealFeaturedSlider( Qt::Orientation o_ );
        RealFeaturedSlider( Qt::Orientation o_, int disc_ );

        void getDoubleMarkers( std::map< double, int >& markers_ );
        double maximumValue() const;
        double minimumValue() const;


    signals:



            void sliderMoved( const double& value_ );
            void markValue( const double& value );
            void unmarkValue( double value );
            void hightlightValue( double value );


    public slots:


            void addMarker( double value );
            void removeMarker( double value );


            void setRange( double min_, double max_ );
            void setDiscretization( int disc_ );

            void setValue( double value_ );
            double getDoubleValue( int index_ ) const;
            double getValue() const ;


            int getIndex( double value_ ) const ;

            void increaseValue();
            void decreaseValue();


    protected slots:

            void moveInDouble( int index_ );



    protected:

            void updateStep();


            void hightlightMarker( int pos_ );
            bool isValidMarker( int value );
            void updateMarkerPositions();
            void saveMarkerPosition( int value, int slider_length, int slider_min, int slider_max );
            void getSubControlDimensions( int& slider_length, int& slider_min, int& slider_max ) const ;




    protected:

        double maximum;
        double minimum;
        double step;
        int discretization;




};

#endif // REALFEATUREDSLIDER_H
