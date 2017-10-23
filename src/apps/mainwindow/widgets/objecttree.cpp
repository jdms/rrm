#include "objecttree.h"

ObjectTree::ObjectTree( QWidget *parent )
{
    setHeaderHidden( true );
    setColumnCount( COLUMNS_NUMBER );

    ObjectTreeItem* test = new ObjectTreeItem();
    test->setCheckState( 0, Qt::Checked );
    test->setText( 1, QString( "Volume" ) );

    addTopLevelItem( test );

    ObjectTreeItem* test1 = new ObjectTreeItem();
    test1->setCheckState( 0, Qt::Checked );
    test1->setText( 1, QString( "Surface" ) );

    test->addChild( test1 );

}
