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
};

#endif // OBJECTTREE_H
