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


    public slots:

        void changeVolumeSize( double width, double height, double depth );
        void changeRangeSize( double width, double height, double depth );
        void setEnabledVolumeResize( bool status );


    signals:

        void widthVolumeChanged( int value );
        void heightVolumeChanged( int value );
        void depthVolumeChanged( int value );


    protected:

        void createVolumePropertiesPage();
        void createRulesHistoryPage();

    private:

        Ui::Form wd_volume_resize;
};

#endif // PAGESSTACK_H
