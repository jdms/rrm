#ifndef LATERALBAR_H
#define LATERALBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLCDNumber>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QDial>
#include <QPixmap>

#include "models/anglepicture.h"

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
        * Destructor. Delete all elements created in the widget.
        */
        ~LateralBar();


        /**
       * Method to get the output dip angle picture. The picture is used in the scene in order to facilitate the user knows which angle he/she should drawn the sketch.
       * @see QPixmap
       * @return const QPixmap* A const pointer to QPixmap, which contains the output dip angle.
       */
        const QPixmap* getDipAnglePicture() const;


    public slots:

       /**
       * Method (slot) called as using the vertical exaggeration slider.
       * It updates the dip angle value.
       * @param v_exagg_ Corresponds to the vertical exaggeration slider position (integer).
       * @return Void
       */
        void changeVerticalExaggeration( int v_exagg_ );


       /**
       * Method (slot) called as using the vertical exaggeration double spinbox.
       * It updates the dip angle value.
       * @param v_exagg_ Corresponds to the vertical exaggeration slider value (double).
       * @return Void
       */
        void changeVerticalExaggerationBySpinbox( double v_exagg_ );


       /**
       * Method (slot) to reset the value of the vertical exaggeration to 1.0. It is also called by the QPushButton btn_reset_exaggeration.
       * It updates the dip angle value.
       * @return Void
       */
        void resetVerticalExaggeration();


        /**
       * Method (slot) to change the value of the input dip angle value (double).
       * @param angle_ Corresponds to the the input dip angle value (double).
       * @return Void
       */
        void changeDipAngle( double angle_ );


        /**
       * Method (slot) to update the value of the output dip angle value (double) using the value of the current dl_input_dipangle value.
       * @see Javadoc_Test()
       * @see ~Javadoc_Test()
       * @see testMeToo()
       * @see publicVar()
       * @return Void
       */
        void updateDipAngle();


       /**
       * Method (slot) to clear all the widgets related to the dip angle and vertical exaggeration, including the pictures.
       * @return Void
       */
        void clear();


    signals:

       /**
       * Signal to send the vertical exaggeration value to the 2d and 3d scenes.
       * @param double Corresponds to the current vertical exaggeration value.
       * @return Void
       */
        void sgn_updateVerticalExaggeration( double );


       /**
       * Signal emitted to warn the 2d and 3d scenes to reset the vertical exaggeration images.
       * @return Void
       */
        void sgn_resetVerticalExaggeration();

       /**
       * Signal emitted to warn the 2d scene to exhibit or not the dip angle picture.
       * If the bool is true, the scene should show the output dip angle picture. The scene is able to get the picture using the public method getDipAnglePicture.
       * @param bool Value can be true if the scene should show the output dip angle picture, and false, otherwise.
       * @return Void
       */
        void sgn_sendDipAnglePicture( bool );

       /**
       * Signal emitted to warn the 2d scene to allow the user moves or not the dip angle picture.
       * @param bool Value can be true if the user is able to move the output dip angle picture, and false, otherwise.
       * @return Void
       */
        void sgn_setDipAnglePictureMovable( bool );


    protected:

       /**
       * Protected method responsible for initializing and exhibiting the widgets.
       * @return Void
       */
        void createInterface();

       /**
       * Protected method responsible for defining and initializing the connects related to the actions of the widgets.
       * @return Void
       */
        void createActions();


    protected:

        QSlider* sl_vertical_exagg = nullptr;                       /**< Integer slider to represent the user input vertical exaggeration value. */
        QPushButton* btn_reset_exaggeration = nullptr;              /**< Button to reset the vertical exaggeration value. */
        QDoubleSpinBox* sp_exagger_value = nullptr;                 /**< Spinbox with doubles values to represent the user input vertical exaggeration value. */

        QDial* dl_input_dipangle = nullptr;                         /**< A dial to represent the user input dip angle (double).  */
        AnglePicture* lb_input_dipangle = nullptr;                  /**< A custom data to illustrate the input dip angle graphically.  */
        QLCDNumber* lcd_input_dipangle = nullptr;                   /**< A widget to display the current input dip angle.  */

        AnglePicture* lb_output_dipangle = nullptr;                 /**< A custom data to illustrate the output dip angle graphically.  */
        QLCDNumber* lcd_output_dipangle = nullptr;                  /**< A widget to display the current output dip angle.  */

        QPushButton* btn_show_output_dipangle = nullptr;            /**< Button to display the current image related to output angle.  */
        QPushButton* btn_move_output_dipangle = nullptr;            /**< Button to allow the user to move the current image related to output angle.  */

        const double MIN_VALUE = 0.1;                               /**< Minimum value to the vertical exaggeration slider.  */
        const double MAX_VALUE = 10000.;                            /**< Maximum value to the vertical exaggeration slider.  */

        double min_log = -1;                                        /**< Log10 of the minimum value to the vertical exaggeration slider.  */
        double max_log = 4;                                         /**< Log10 of the maximum value to the vertical exaggeration slider.  */


        ///////////////////////////////////////////////////////////////////////////////
        //                                                                           //
        // log10 1 = min_log + v_exagg_*0.01* (max_log - min_log);                   //
        // log10 1 = -1 + v_exagg_*0.01* (4 - (-1) );                                //
        // 0 = -1 +  v_exagg_*0.01*5                                                 //
        // v_exagg_ = 100/5 => v_exagg_ = 20                                         //
        ///////////////////////////////////////////////////////////////////////////////

        const int RESET_VEXAG = 20;                               /**< Slider value to reset the vertical exaggeration to 1.0  */

        int count = 0;
};

#endif // LATERALBAR_H
