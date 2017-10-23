#include "objecttree.h"

ObjectTree::ObjectTree( QWidget *parent )
{
    setHeaderHidden( true );
    setColumnCount( COLUMNS_NUMBER );

    ObjectTreeItem* test = new ObjectTreeItem();
    test->setCheckState( COLUMN_STATUS, Qt::Checked );
    test->setText( COLUMN_NAME, QString( "Volume" ) );

    addTopLevelItem( test );

    ObjectTreeItem* test1 = new ObjectTreeItem();
    test1->setCheckState( COLUMN_STATUS, Qt::Checked );
    test1->setText( COLUMN_NAME, QString( "Surface" ) );

    test->addChild( test1 );

}
