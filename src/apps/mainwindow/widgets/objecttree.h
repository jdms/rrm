#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <QTreeWidget>

#include "objecttreeitem.h"


class ObjectTree: public QTreeWidget
{
    Q_OBJECT


    public:

        ObjectTree( QWidget* parent = 0 );



    private:

        const int COLUMNS_NUMBER = 3;
        const int COLUMN_STATUS = 0;
        const int COLUMN_NAME = 1;
        const int COLUMN_COLOR = 2;
};

#endif // OBJECTTREE_H
