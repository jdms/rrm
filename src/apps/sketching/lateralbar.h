#ifndef LATERALBAR_H
#define LATERALBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLCDNumber>
#include <QDoubleSpinBox>

#include <QDial>

#include "models/anglepicture.h"
#include "./core/widgets/realfeaturedslider.h"


class LateralBar: public QWidget
{
    Q_OBJECT

    public:

        LateralBar();


    public slots:

        void changeVerticalExaggeration( int v_exagg_ );


    protected:

        void createInterface();
        void createActions();


    protected:

        RealFeaturedSlider* sl_vertical_exagg;

        QPushButton* btn_reset_exaggeration;
        QLabel* lb_exagger_value;
        QDoubleSpinBox* sp_exagger_value;

        QDial* dl_input_dipangle;
        AnglePicture* lb_input_dipangle;
        QLCDNumber* lcd_input_dipangle;

        AnglePicture* lb_output_dipangle;
        QLCDNumber* lcd_output_dipangle;

        QPushButton* btn_show_output_dipangle;
        QPushButton* btn_move_output_dipangle;

        int nsteps_exagg = 100;
        double min_exagg = -1;
        double max_exagg = 4;
        double steps_exagg = 0;

        int count = 0;
};

#endif // LATERALBAR_H
