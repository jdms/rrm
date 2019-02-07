#ifndef ANGLEPICTURE_H
#define ANGLEPICTURE_H

#include <QLabel>
#include <QPixmap>

/**
 *  Auxiliary wrapper to hold and handle operations on the output dip angle picture.
 *  It inherits from QLabel.
 */

class AnglePicture: public QLabel
{

    public:

        /**
        * Constructor.
        * @param size_ size of the image
        * @param angle_ the output dip angle
        */
        AnglePicture(  const QSize& size_, double angle_ );


    public slots:


        /**
        * Method to update the output dip angle
        * @param angle_ the output dip angle
        * @return void..
        */
        void updateAngle( double angle_ );

    private slots:


        /**
        * Method to repaint the image
        * @see QPaintEvent contains event parameters for paint events
        * @return void.
        */
        void paintEvent(QPaintEvent* e);


    private:


        QSize size;                                     /**< The size of the image  */

        double angle = 0;                               /**< The output dip angle

*/
};

#endif // ANGLEPICTURE_H
