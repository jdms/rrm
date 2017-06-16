#ifndef PAGESSTACK_H
#define PAGESSTACK_H

#include <QStackedWidget>


#include "ui_VolumeResizeWidget.h"

class PagesStack: public QStackedWidget
{
    Q_OBJECT

    public:

        PagesStack();

        void createVolumeActions();
        void clear();


    public slots:

        void changeVolumeSize( double width, double height, double depth );
        void changeRangeSize( double width, double height, double depth );
        void setEnabledVolumeResize( bool status );


        void setVolumeWidth( double w );
        void setVolumeHeight( double h );
        void setVolumeDepth( double d );


    signals:

        void widthVolumeChanged( int value );
        void heightVolumeChanged( int value );
        void depthVolumeChanged( int value );


    protected:

        void createVolumePropertiesPage();
        void createRulesHistoryPage();

    private:

        Ui::Form wd_volume_resize;

        const int MAX_WIDTH = 10000;
        const int MAX_HEIGHT = 10000;
        const int MAX_DEPTH = 10000;

        const int SINGLE_STEP = 10;

        const bool VOLUME_RESIZING = true;
};

#endif // PAGESSTACK_H
