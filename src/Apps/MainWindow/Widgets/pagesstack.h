#ifndef PAGESSTACK_H
#define PAGESSTACK_H

#include <QStackedWidget>


#include "ui_VolumeResizeWidget.h"

class PagesStack: public QStackedWidget
{
    Q_OBJECT

    public:

        PagesStack();

    public slots:

        void changeVolumeSize( int width, int height, int depth );
        void setEnabledVolumeResize( bool status );

    protected:

        void createVolumePropertiesPage();
        void createRulesHistoryPage();

    private:

        Ui::Form wd_volume_resize;
};

#endif // PAGESSTACK_H
