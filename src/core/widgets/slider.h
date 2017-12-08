#ifndef SLIDER_H
#define SLIDER_H

#include <QtWidgets/QSlider>
#include <QString>
#include <QMouseEvent>

#include "./core/definitions/constants.hpp"


class Slider: public QSlider
{
    Q_OBJECT

    public:

        Slider( QWidget* parent = 0 );

        int getNumberofMarkers() const;
        void getMarkers( std::map< int, int >& markers_ ) const;


    public slots:

        void addMarker( int value );
        void removeMarker( int value );


    signals:

        void markValue( int value );
        void unmarkValue( int value );
        void hightlightValue( int value );



    protected:

        void hightlightMarker( int pos_ );
        bool isValidMarker( int value );
        void updateMarkerPositions();
        void saveMarkerPosition( int value, int slider_length, int slider_min, int slider_max );
        void getSubControlDimensions( int& slider_length, int& slider_min, int& slider_max ) const ;


        void paintEvent( QPaintEvent *ev );
        void mousePressEvent( QMouseEvent *event );
        void resizeEvent( QResizeEvent *event );




    protected:


        std::map< int, int > markers;
};

#endif // SLIDER_H
