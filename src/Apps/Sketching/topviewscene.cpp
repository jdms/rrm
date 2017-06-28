#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QSvgGenerator>

#include "topviewscene.h"

#include "Core/Geology/Models/Volume.h"
#include "Core/Geology/Models/object_refactored.h"
#include "./ItemWrappers/crosssectionitemwrapper.h"


TopViewScene::TopViewScene()
{
    csection = nullptr;
    draw_csections = true;
    setupPen();
}



void TopViewScene::addVolume( Volume* const& vol )
{
    volume.defineSectionPlane( VolumeItemWrapper::Section::XZ );
    volume.setRawVolume( vol );
    setSceneRect( volume.boundingRect() );
    addItem( &volume );

    axes.setPlane( CoordinateAxes2d::Plane::XZ );
    addItem( &axes );

    createCurrentCrossSection( volume.getHeight() );
}


void TopViewScene::updateVolume()
{
    volume.updateItem();
    setSceneRect( volume.boundingRect() );

    csection->setDimensions( volume.getWidth(), volume.getHeight() );

    axes.setAxisXLenght( volume.getWidth() );
    axes.setAxisYLenght( volume.getHeight() );

    update();
}


void TopViewScene::createCurrentCrossSection( double depth )
{
    csection = new CrossSectionItemWrapper( volume.getWidth(), depth );
    csection->setCurrent( true );
    csection->setVisible( true );
    addItem( csection );
    update();
}


void TopViewScene::moveCurrentCrossSection( double depth )
{
    csection->setDepth( depth );
    update();
}


void TopViewScene::addCrossSection( double depth )
{
    if( isAddedCrossSection( depth ) == true ) return;

    csections[ depth ] = new CrossSectionItemWrapper( volume.getWidth(), depth );
    csections[ depth ]->setCurrent( false );
    csections[ depth ]->setVisible( draw_csections );

    addItem( csections[ depth ] );
    update();

}


bool TopViewScene::isAddedCrossSection( double depth )
{
    auto search = csections.find( depth );
    if( search == csections.end() ) return false;
    return true;
}


void TopViewScene::showCrossSections( bool status )
{
    draw_csections = status;
    for( auto &it: csections )
        (it.second)->setVisible( status );
    update();
}


bool TopViewScene::isAddedObject( std::size_t id )
{
    auto search = objects.find( id );
    if( search == objects.end() ) return false;
    return true;
}



void TopViewScene::addObject( Object_Refactored * const &obj )
{

    if( isAddedObject( obj->getId() ) == true )
        updateObject( obj->getId() );

    int r, g, b;
    obj->getColor( r, g, b );
    trajectory_pen.setColor( QColor( r, g, b ) );



    TrajectoryItemWrapper* wrapper = new TrajectoryItemWrapper();
    wrapper->setRawObject( obj );

    objects[ obj->getId() ] = wrapper;
    addItem( wrapper );

    update();


}



void TopViewScene::updateObject( std::size_t id )
{
    if( isAddedObject( id ) == false ) return;

    TrajectoryItemWrapper* const& wrapper = objects[ id ];
    wrapper->updateObject();
    update();
}


void TopViewScene::savetoRasterImage( const QString& filename )
{

    QStringList name_and_extension = filename.split('.', QString::SkipEmptyParts );

    QString filename_csection;
    if( name_and_extension.size() > 1 ){
        filename_csection = name_and_extension[ 0 ].append( "_topview." );
        filename_csection.append( name_and_extension[1] );
    }
    else
        filename_csection = filename;


    QImage image( sceneRect().size().toSize(), QImage::Format_ARGB32 );
//    image.fill( Qt::transparent );

    QPainter painter( &image );
    render( &painter );
    painter.end();

    image = image.mirrored( false, true );
    image.save( filename_csection );

}

void TopViewScene::savetoVectorImage( const QString& filename )
{


    QStringList name_and_extension = filename.split('.', QString::SkipEmptyParts );

    QString filename_csection;
    if( name_and_extension.size() > 1 ){
        filename_csection = name_and_extension[ 0 ].append( "_topview." );
        filename_csection.append( name_and_extension[1] );
    }
    else
        filename_csection = filename;



    QSvgGenerator svgGen;

    svgGen.setFileName( filename_csection );
    svgGen.setSize( QSize( width(), height() ) );

    svgGen.setViewBox( sceneRect() );
    svgGen.setTitle( tr( "Rapid Reservoir Modelling - SVG generated by Qt5" ) );
    svgGen.setDescription( tr( "SVG output of Rapid Reservoir Modelling Software" ) );

    QPainter painter( &svgGen );
    painter.scale( 1.0, -1.0 );
    painter.translate( QPointF( 0.0, -height() ) );

    render( &painter );
    painter.end();
}



bool TopViewScene::acceptSketch( Curve2D& curve )
{
    if( sketch->isEmpty() == true ) return false;

    curve = PolyQtUtils::qPolyginFToCurve2D( sketch->getSketch() );
    processCurve( curve );

    return true;
}


void TopViewScene::removeCurve()
{

    if( ( current_interaction != UserInteraction::EDITING_SCENE ) ||
        ( is_delete_enabled == false ) ) return;

    QList < QGraphicsItem* > items = selectedItems();
    if( items.empty() == true ) return;

    QGraphicsItem* item = items[ 0 ];

    removeItem( item );
    delete item;

    emit removeTrajectory();
    update();
}



void TopViewScene::setupPen()
{
    trajectory_pen.setStyle( Qt::SolidLine );
    trajectory_pen.setCapStyle( Qt::RoundCap );
    trajectory_pen.setJoinStyle( Qt::RoundJoin );
    trajectory_pen.setWidth( 3 );
}



void TopViewScene::showPopUp()
{
    QFrame* w = new QFrame;
    w->setMaximumHeight( 20 );
    w->setStyleSheet( "background-color: red");


    QLabel *lb_popup = new QLabel( "It's not possible to add two cross-sections and "
                                   "add a trajectory!" );

    QGridLayout* gl_popup = new  QGridLayout();
    gl_popup->addWidget( lb_popup, 0, 1 );

    w->setLayout( gl_popup );
    w->show();

}



void TopViewScene::clear()
{
    clearData();
    draw_csections = true;

    addItem( &axes );
    createCrossSectionImageItem();
    update();
}


void TopViewScene::clearData()
{

    for( auto &it: items() )
        removeItem( it );

    if( csection != nullptr )
    {
        csection->clear();
        delete csection;
    }
    csection = nullptr;

    for( auto &it: csections )
    {
        ( it.second )->clear();
        delete ( it.second );
    }
    csections.clear();

    for( auto &it: csections )
    {
        ( it.second )->clear();
        delete ( it.second );
    }
    csections.clear();


    for( auto &it: objects )
    {
//        ( it.second )->clear();
        delete ( it.second );
    }
    objects.clear();

}




void TopViewScene::keyPressEvent( QKeyEvent *event )
{
    switch( event->key() )
    {
        case Qt::Key_Delete:
            removeCurve();
            break;
        default:
            break;
    };
}
