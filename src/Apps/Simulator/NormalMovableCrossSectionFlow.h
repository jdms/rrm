#ifndef NORMALMOVABLECROSSSECTIONFLOW_H
#define NORMALMOVABLECROSSSECTIONFLOW_H

#include <QWidget>

#include "ui_NormalMovableCrossSectionFlow.h"



class NormalMovableCrossSectionFlow : public QWidget,
                                      private Ui::NormalMovableCrossSectionFlow
{
    Q_OBJECT

    public:

        explicit NormalMovableCrossSectionFlow(QWidget *parent = 0);
        ~NormalMovableCrossSectionFlow();

        void clear();

    private slots:

        void on_btn_acceptnormal_accepted();
        void on_btn_acceptnormal_rejected();

    signals:

        void sendCrossSectionNormalCoordinates( float x, float y, float z );
        void canceled();



};

#endif // NORMALMOVABLECROSSSECTIONFLOW_H
