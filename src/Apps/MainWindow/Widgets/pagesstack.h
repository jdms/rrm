#ifndef PAGESSTACK_H
#define PAGESSTACK_H

#include <QStackedWidget>

class PagesStack: public QStackedWidget
{
    Q_OBJECT

    public:

        PagesStack();


    protected:

        void createVolumePropertiesPage();
        void createRulesHistoryPage();

};

#endif // PAGESSTACK_H
