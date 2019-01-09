#ifndef LATERALBAR_H
#define LATERALBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLCDNumber>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QDial>
#include <QPixmap>

#include "models/anglepicture.h"
//#include "./core/widgets/realfeaturedslider.h"

/**
 *   Container to Dip Angle and Vertical Exaggeration widgets.
 */

class LateralBar: public QWidget
{
    Q_OBJECT

    public:

        /**
        * Constructor.
        */
        LateralBar();

        /**
        * Destructor. Delete all elements created in the window.
        */
        ~LateralBar();


        const QPixmap* getDipAnglePicture() const;


    public slots:

        void changeVerticalExaggeration( int v_exagg_ );
        void changeVerticalExaggerationBySpinbox( double v_exagg_ );
        void resetVerticalExaggeration();
        void changeDipAngle( double angle_ );
        void updateDipAngle();
        void clear();


    signals:

        void sgn_updateVerticalExaggeration( double );
        void sgn_resetVerticalExaggeration();
        void sgn_sendDipAnglePicture( bool );
        void sgn_setDipAnglePictureMovable( bool );


    protected:

        void createInterface();
        void createActions();


    protected:

        QSlider* sl_vertical_exagg = nullptr;            /**< Integer slider to represent the user input vertical exaggeration value. */
        QPushButton* btn_reset_exaggeration = nullptr;              /**< Button to reset the vertical exaggeration value. */
        QDoubleSpinBox* sp_exagger_value = nullptr;                 /**< Spinbox with doubles values to represent the user input vertical exaggeration value. */

        QDial* dl_input_dipangle = nullptr;                         /**< A dial to represent the user input dip angle (double).  */
        AnglePicture* lb_input_dipangle = nullptr;                  /**< A custom data to illustrate the input dip angle graphically.  */
        QLCDNumber* lcd_input_dipangle = nullptr;                   /**< A widget to display the current input dip angle.  */

        AnglePicture* lb_output_dipangle = nullptr;                 /**< A custom data to illustrate the output dip angle graphically.  */
        QLCDNumber* lcd_output_dipangle = nullptr;                  /**< A widget to display the current output dip angle.  */

        QPushButton* btn_show_output_dipangle = nullptr;            /**< Button to display the current image related to output angle.  */
        QPushButton* btn_move_output_dipangle = nullptr;            /**< Button to allow the user to move the current image related to output angle.  */

        double min_exagg = -1;                                      /**< Minimum value to the vertical exxageration slider.  */
        double max_exagg = 4;                                       /**< Maximum value to the vertical exxageration slider.  */

        int count = 0;
};

#endif // LATERALBAR_H
