#ifndef ANGLEPICTURE_H
#define ANGLEPICTURE_H

#include <QLabel>
#include <QPixmap>

class AnglePicture: public QLabel
{

    public:

        AnglePicture(  const QSize& size_, double angle_ );


    public slots:
        void updateAngle( double angle_ );

    private slots:

        void paintEvent(QPaintEvent * e);


    private:


        QSize size;
        double angle = 0;
};

#endif // ANGLEPICTURE_H
