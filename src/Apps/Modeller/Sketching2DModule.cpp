#include "Sketching2DModule.h"

Sketching2DModule::Sketching2DModule ( QWidget* parent )
{
	createWindow ( );
	createActions ( parent );

	emit setColor ( 0 , 0 , 128 );
}

void Sketching2DModule::createWindow ( )
{
//    dc_2DModule = new QDockWidget( this );
	this->setAllowedAreas ( Qt::LeftDockWidgetArea );
	this->setWindowTitle ( "2D View" );
}

void Sketching2DModule::createActions ( QWidget* parent )
{

}

void Sketching2DModule::applyRemoveAbove ( )
{
//    bool flag = ac_removeabove->isChecked();
//    if( flag == false ) return;

//    ac_removebelow->setChecked( false );
	emit applyremoveabove ( );
//    statusBar()->showMessage( "Applying remove above intersection" );

}

void Sketching2DModule::applyRemoveBelow ( )
{
//    bool flag = ac_removebelow->isChecked();
//    if( flag == false ) return;

//    ac_removeabove->setChecked( false );
	emit applyremovebelow ( );

//    statusBar()->showMessage( "Applying remove below intersection" );
}

void Sketching2DModule::pointerSelection ( bool flag )
{
//    bool flag = ac_select->isChecked();

	if ( flag == false )
	{
		emit sketchingMode ( );
		return;
	}

	emit selectMode ( );
}

void Sketching2DModule::emitColor ( int R , int G , int B )
{

	emit setColor ( R , G , B );
}

void Sketching2DModule::emitModeSelect ( )
{
	emit selectMode ( );
}

void Sketching2DModule::emitModeSketching ( )
{
	emit sketchingMode ( );
}

void Sketching2DModule::clearCanvas2D ( )
{

}
