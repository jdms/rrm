/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <memory>
#include <QCheckBox>

#include "objecttree.h"
#include "core/widgets/color_picker.h"


ObjectTree::ObjectTree( QWidget *parent )
{
    setHeaderHidden( true );
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setContextMenuPolicy( Qt::CustomContextMenu );
    setColumnCount( COLUMNS_NUMBER );
    setAcceptDrops( true );
    setMouseTracking( true );


    createMenu();

    connect( this, &ObjectTree::itemChanged, this, &ObjectTree::filterAction );

    label_stratigraphy = new QTreeWidgetItem();
    label_stratigraphy->setText( COLUMN_NAME, "STRATIGRAPHY" );
    label_stratigraphy->setCheckState( COLUMN_STATUS, Qt::Checked );

    label_structural = new QTreeWidgetItem();
    label_structural->setText( COLUMN_NAME, "STRUCTURAL" );
    label_structural->setCheckState( COLUMN_STATUS, Qt::Checked );

    label_domains = new QTreeWidgetItem();
    label_domains->setText( COLUMN_NAME, "DOMAINS" );
    label_domains->setCheckState( COLUMN_STATUS, Qt::Checked );
    label_domains->setHidden( true );

    connect( this, &ObjectTree::itemClicked, [=]( QTreeWidgetItem* item_ )
    {
        ObjectTreeItem* obj_item_ = static_cast< ObjectTreeItem* >( item_ );
        emit objectSelected( obj_item_->getType() );
    } );


}


void ObjectTree::createMenu()
{
    ac_create_domain = new QAction( "Create domain", this );
    ac_addto_domain = new QAction( "Add to domain", this );
    ac_removefrom_domain = new QAction( "Remove from domain", this );
    ac_remove_domain = new QAction( "Delete domain", this );

    connect( ac_create_domain, &QAction::triggered, [=]()
    {
        emit createDomainOfRegions( 0 );

    } );


    connect( ac_removefrom_domain, &QAction::triggered, this, &ObjectTree::removeFromDomain1 );

    connect( ac_remove_domain, &QAction::triggered, this, &ObjectTree::deleteDomains1 );


    /*

    connect( ac_create_domain, &QAction::triggered, this, &ObjectTree::createDomain );

    connect( ac_addto_domain, &QAction::triggered, this, &ObjectTree::addToDomain );

    connect( ac_removefrom_domain, &QAction::triggered, this, &ObjectTree::removeFromDomain );

    connect( ac_remove_domain, &QAction::triggered, this, &ObjectTree::deleteDomain );

*/



    mn_menu = new QMenu();
    mn_menu->addAction( ac_create_domain );

    mn_submenu = new QMenu( "Add to domain" );
    mn_menu->addMenu( mn_submenu );

    mn_menu->addAction( ac_removefrom_domain );
    mn_menu->addAction( ac_remove_domain );

    connect( this, &ObjectTree::customContextMenuRequested, this, &ObjectTree::showMenu );


    connect( mn_submenu, &QMenu::aboutToShow, [=]()
    {
        mn_submenu->show();
    } );


}


void ObjectTree::showMenu( const QPoint& pos_ )
{
    mn_menu->exec( pos_ );
}


void ObjectTree::filterAction( QTreeWidgetItem* item_, std::size_t column_ )
{
    ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( item_ );

    if( item_ == label_stratigraphy && column_ == COLUMN_STATUS )
        setStratigraphiesVisible( item_->checkState( COLUMN_STATUS ) );


    else if( item_ == label_structural && column_ == COLUMN_STATUS )
        setStructuralsVisible( item_->checkState( COLUMN_STATUS ) );

    else if( item_ == label_domains && column_ == COLUMN_STATUS )
        setDomainsVisible( item_->checkState( COLUMN_STATUS ) );


    else if( obj_->getType() == Settings::Objects::ObjectType::VOLUME )
    {
        if( column_ == COLUMN_STATUS )
        {
            setVolumeVisibility( obj_->getIndex(), obj_->checkState( COLUMN_STATUS ) );
        }
        else if( column_ == COLUMN_NAME )
        {
            emit setVolumeName( obj_->getIndex(), obj_->text( COLUMN_NAME ).toStdString() );
        }
    }

    else if( obj_->getType() == Settings::Objects::ObjectType::STRATIGRAPHY )
    {
        if( column_ == COLUMN_STATUS )
        {
            bool status_ = false;
            if( obj_->checkState( COLUMN_STATUS ) == Qt::Checked )
                status_ = true;

            emit setObjectVisible( obj_->getIndex(), status_ );
        }
        else if( column_ == COLUMN_NAME )
        {
            emit setObjectName( obj_->getIndex(), obj_->text( COLUMN_NAME ).toStdString() );
        }

    }

    else if( obj_->getType() == Settings::Objects::ObjectType::STRUCTURAL )
    {
        if( column_ == COLUMN_STATUS )
        {
            bool status_ = false;
            if( obj_->checkState( COLUMN_STATUS ) == Qt::Checked )
                status_ = true;

            emit setObjectVisible( obj_->getIndex(), status_ );
        }
        else if( column_ == COLUMN_NAME )
        {
            emit setObjectName( obj_->getIndex(), obj_->text( COLUMN_NAME ).toStdString() );
        }

    }

    else if( obj_->getType() == Settings::Objects::ObjectType::CROSS_SECTION )
    {
        if( column_ == COLUMN_STATUS )
        {
            bool status_ = false;
            if( obj_->checkState( COLUMN_STATUS ) == Qt::Checked )
                status_ = true;

            emit setCrossSectionVisible( obj_->getIndex(), status_ );
        }
        else if( column_ == COLUMN_NAME )
        {
            emit setCrossSectionName( obj_->getIndex(), obj_->text( COLUMN_NAME ).toStdString() );
        }
    }

    else if( obj_->getType() == Settings::Objects::ObjectType::REGION )
    {
        if( column_ == COLUMN_STATUS )
        {
            bool status_ = false;
            if( obj_->checkState( COLUMN_STATUS ) == Qt::Checked )
                status_ = true;

            emit setRegionVisible( obj_->getIndex(), status_ );
        }
        else if( column_ == COLUMN_NAME )
        {
            emit setRegionName( obj_->getIndex(), obj_->text( COLUMN_NAME ).toStdString() );
        }
    }

    else if( obj_->getType() == Settings::Objects::ObjectType::DOMAINS )
    {
        if( column_ == COLUMN_STATUS )
        {
            bool status_ = false;
            if( obj_->checkState( COLUMN_STATUS ) == Qt::Checked )
                status_ = true;

            setDomainsVisibility( obj_->getIndex(), status_ );

//            emit setDomainsVisible( obj_->getIndex(), status_ );
        }
        else if( column_ == COLUMN_NAME )
        {
//            emit setRegionName( obj_->getIndex(), obj_->text( COLUMN_NAME ).toStdString() );
        }
    }

}




void ObjectTree::setDomainsVisible( const Qt::CheckState& state_ )
{
    int nchildren = label_domains->childCount();
    bool status_ = ( state_ == Qt::Checked )? true:false;

    for( int j = 0; j < nchildren; ++j )
    {
        ObjectTreeItem* obj_ = (ObjectTreeItem* )( label_domains->child( j ) );
        setDomainsVisibility( obj_->getIndex(), status_ );
    }

}


void ObjectTree::setDomainsVisibility( std::size_t index_, bool status_ )
{

    if( domains.findElement( index_ ) == false ) return;

    ObjectTreeItem* domain_ = domains.getElement( index_ );

    Qt::CheckState state_ = ( status_ == true )? Qt::Checked:Qt::Unchecked;

    int nchildren = domain_->childCount();
    for( int j = 0; j < nchildren; ++j )
    {
        ObjectTreeItem* obj_ = (ObjectTreeItem* )( domain_->child( j ) );
        obj_->setCheckState( COLUMN_STATUS, state_ );
//        setRegionVisible( obj_->getIndex(), status_ );
    }

}


void ObjectTree::addInputVolume()
{
    ObjectTreeItem* vol_ = new ObjectTreeItem();
    vol_->setText( COLUMN_NAME, "VOLUME" );
    vol_->setType( Settings::Objects::ObjectType::VOLUME );
    vol_->setIndex( 0 );
    vol_->setCheckState( COLUMN_STATUS, Qt::Checked );
    addTopLevelItem( vol_ );

    vol_->addChild( label_stratigraphy );
    label_stratigraphy->setHidden( true );
    vol_->addChild( label_structural );
    label_structural->setHidden( true );

}


void ObjectTree::hideInputVolume()
{
    ObjectTreeItem* vol_ = (ObjectTreeItem* )( topLevelItem( 0 ) );
    vol_->setHidden( true );
    setVolumeVisible( 0, false );

    int nchildren = vol_->childCount();
    for( int j = 0; j < nchildren; ++j )
    {
        ObjectTreeItem* obj_ = (ObjectTreeItem* )( vol_->child( j ) );
        obj_->setHidden( true );
        setObjectVisibility( obj_->getIndex(), false );

    }

    update();

}


void ObjectTree::hideOutputVolume()
{
    ObjectTreeItem* vol_ = (ObjectTreeItem* )( topLevelItem( 1 ) );
    vol_->setHidden( true );
    setVolumeVisible( 1, false );

    update();

}


void ObjectTree::addOutputVolume()
{
    ObjectTreeItem* vol_ = new ObjectTreeItem();
    vol_->setText( COLUMN_NAME, "REGIONS" );
    vol_->setType( Settings::Objects::ObjectType::VOLUME );
    vol_->setIndex( 1 );
    vol_->setCheckState( COLUMN_STATUS, Qt::Checked );
    addTopLevelItem( vol_ );
}


void ObjectTree::removeOutputVolume()
{
    if( topLevelItemCount() < 2 )
        return;

    int nchildren = topLevelItem( 1 )->childCount();
    for( int j = 0; j < nchildren; ++j )
    {
        ObjectTreeItem* obj_ = (ObjectTreeItem* )( topLevelItem( 1 )->child( j ) );

        ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );
        removeItemWidget( obj_, COLUMN_COLOR );
        delete colorpicker_;

        delete obj_;
        obj_ = nullptr;
    }

    ObjectTreeItem* vol1_ = (ObjectTreeItem* )( topLevelItem( 1 ) );
    if( vol1_ != nullptr )
    {
        delete vol1_;
        vol1_ = nullptr;
   }

    regions.clear();
}


void ObjectTree::addObject( std::size_t index_, const Settings::Objects::ObjectType& type_,
                            const std::string& name_,  const int& red_,
                            const int& green_,  const int& blue_ )
{

    ObjectTreeItem* vol_ = ( ObjectTreeItem* ) topLevelItem( 0 );
    if( vol_ == nullptr ) return;


    ObjectTreeItem* obj_ = new ObjectTreeItem();
    obj_->setIndex( index_ );
    obj_->setType( type_ );
    obj_->setText( COLUMN_NAME, QString( name_.c_str() ) );
    obj_->setCheckState( COLUMN_STATUS, Qt::Checked );

    ColorPicker* colorpicker_ = new ColorPicker( this );
    colorpicker_->setColor( QColor( red_, green_, blue_ ) );
    connect( colorpicker_, &ColorPicker::colorSelected, [=]( const QColor& color_ )
    { emit setObjectColor( index_, color_ ); } );


    if( type_ == Settings::Objects::ObjectType::STRATIGRAPHY )
    {
        if( stratigraphies.empty() == true )
        {
            label_stratigraphy->setHidden( false );
        }

        label_stratigraphy->addChild( obj_ );
        stratigraphies.addElement( index_, obj_ );

    }

    else if( type_ == Settings::Objects::ObjectType::STRUCTURAL )
    {
        if( structurals.empty() == true )
        {
            label_structural->setHidden( false );
        }

        label_structural->addChild( obj_ );
        structurals.addElement( index_, obj_ );

    }

    items.addElement( index_, obj_ );
    setItemWidget( obj_, COLUMN_COLOR, colorpicker_ );


}


void ObjectTree::removeObject( std::size_t index_ )
{
    if( items.findElement( index_ ) == false ) return;
    ObjectTreeItem* obj_ = items.getElement( index_ );
    if( obj_ == nullptr ) return;


    ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );
    removeItemWidget( obj_, COLUMN_COLOR );
    delete colorpicker_;


    ObjectTreeItem* vol_ = ( ObjectTreeItem* ) topLevelItem( 0 );
    if( vol_ == nullptr ) return;
    vol_->removeChild( obj_ );

    delete obj_;
    obj_ = nullptr;

}


void ObjectTree::updateObjectColor( std::size_t index_, int red_, int green_, int blue_)
{
    if( items.findElement( index_ ) == false ) return;

    ObjectTreeItem* obj_ = items.getElement( index_ );
    ColorPicker* cp_color_ = static_cast< ColorPicker* > ( itemWidget( obj_, COLUMN_COLOR ) );
    cp_color_->setColor( QColor( red_, green_, blue_ ) );
}


void ObjectTree::updateObjectName( std::size_t index_, const std::string& name_ )
{
    if( items.findElement( index_ ) == false ) return;

    ObjectTreeItem* obj_ = items.getElement( index_ );
    obj_->setText( COLUMN_NAME, QString( name_.c_str() ) );
    update();

}


void ObjectTree::setObjectVisibility( std::size_t index_, bool status_ )
{
    if( items.findElement( index_ ) == false ) return;

    ObjectTreeItem* obj_ = items.getElement( index_ );
    obj_->setHidden( !status_ );
}


void ObjectTree::setVolumeVisibility( std::size_t index_, const Qt::CheckState& status_ )
{

    bool is_visible_ = ( status_ == Qt::Checked? true:false );
    ObjectTreeItem* vol_ = (ObjectTreeItem* )( topLevelItem( index_ ) );

    if( index_ == 0 )
    {
        label_stratigraphy->setCheckState( COLUMN_STATUS, status_ );
        label_structural->setCheckState( COLUMN_STATUS, status_ );
    }
    emit setVolumeVisible( index_, is_visible_ );
    update();

}




void ObjectTree::setStratigraphiesVisible( const Qt::CheckState& status_  )
{
    int nchildren_ = label_stratigraphy->childCount();
    for( int i = 0; i < nchildren_; ++i )
    {
        ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( label_stratigraphy->child( i ) );
        obj_->setCheckState( COLUMN_STATUS, status_ );
    }

}


void ObjectTree::setStructuralsVisible( const Qt::CheckState& status_ )
{
    int nchildren_ = label_structural->childCount();
    for( int i = 0; i < nchildren_; ++i )
    {
        ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( label_structural->child( i ) );
        obj_->setCheckState( COLUMN_STATUS, status_ );
    }

}


void ObjectTree::addRegion( std::size_t index_, const std::string& name_,  const int& red_, const int& green_,  const int& blue_ )
{
    ObjectTreeItem* region_ = new ObjectTreeItem();
    region_->setIndex( index_ );
    region_->setType( Settings::Objects::ObjectType::REGION );
    region_->setText( COLUMN_NAME, QString( name_.c_str() ) );
    region_->setCheckState( COLUMN_STATUS, Qt::Checked );


    ObjectTreeItem* vol1_ = ( ObjectTreeItem* ) topLevelItem( 1 );
    if( vol1_ == nullptr ) return;

    vol1_->addChild( region_ );

    ColorPicker* colorpicker_ = new ColorPicker( this );
    colorpicker_->setColor( QColor( red_, green_, blue_ ) );
    connect( colorpicker_, &ColorPicker::colorSelected, [=]( const QColor& color_ ){ emit setRegionColor( index_, color_ ); } );

    setItemWidget( region_, COLUMN_COLOR, colorpicker_ );
    regions.addElement( index_, region_ );

    connect( this, &ObjectTree::setVolumeVisible, [=]( std::size_t indexv_, bool status_ )
    {   if( indexv_ != 1 ) return;

        ObjectTreeItem* vol2_ = ( ObjectTreeItem* ) topLevelItem( 1 );
        if( vol2_ == nullptr ) return;

        int nchildren = vol2_->childCount();
        for( int j = 0; j < nchildren; ++j )
        {
            ObjectTreeItem* region1_ = (ObjectTreeItem* )( vol2_->child( j ) );
            if( region1_ == nullptr ) continue;
            region1_->setCheckState( COLUMN_STATUS, ( status_? Qt::Checked:Qt::Unchecked ) );
        }
    } );

}


void ObjectTree::updateRegionColor( std::size_t index_, int red_, int green_, int blue_)
{

    if( regions.findElement( index_ ) == false ) return;

    ObjectTreeItem* region_ = regions.getElement( index_ );
    ColorPicker* cp_color_ = static_cast< ColorPicker* > ( itemWidget( region_, COLUMN_COLOR ) );
    cp_color_->setColor( QColor( red_, green_, blue_ ) );
}


void ObjectTree::setRegionVisibility( std::size_t index_, bool status_ )
{
    if( regions.findElement( index_ ) == false ) return;

    ObjectTreeItem* region_ = regions.getElement( index_ );
    region_->setHidden( !status_ );
}



void ObjectTree::removeStratigraphies()
{
    std::vector< std::size_t > diff_objs;

    int nchildren_ = label_stratigraphy->childCount();
    for( int i = 0; i < nchildren_; ++i )
    {
        ObjectTreeItem* obj_ = static_cast< ObjectTreeItem* >( label_stratigraphy->child( i ) );

        ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );
        removeItemWidget( obj_, COLUMN_COLOR );
        delete colorpicker_;

        if( stratigraphies.findElement( obj_->getIndex() ) == true )
        {
            diff_objs.push_back( obj_->getIndex() );
            continue;
        }

        delete obj_;
        obj_ = nullptr;
    }

    for( auto d: diff_objs )
    {
        ObjectTreeItem* obj_ = stratigraphies.getElement( d );

        delete obj_;
        obj_ = nullptr;
    }

    stratigraphies.clear();
}


void ObjectTree::removeStructurals()
{
    std::vector< std::size_t > diff_objs;

    int nchildren_ = label_structural->childCount();
    for( int i = 0; i < nchildren_; ++i )
    {
        ObjectTreeItem* obj_ = static_cast< ObjectTreeItem* >( label_structural->child( i ) );

        ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );
        removeItemWidget( obj_, COLUMN_COLOR );
        delete colorpicker_;

        if( structurals.findElement( obj_->getIndex() ) == true )
        {
            diff_objs.push_back( obj_->getIndex() );
            continue;
        }

        delete obj_;
        obj_ = nullptr;
    }

    for( auto d: diff_objs )
    {
        ObjectTreeItem* obj_ = structurals.getElement( d );

        delete obj_;
        obj_ = nullptr;
    }


    structurals.clear();
}



void ObjectTree::createDomain1( std::size_t index_ )
{
    if( domains.empty() == true )
    {
        addTopLevelItem( label_domains );
        label_domains->setHidden( false );
    }


    ObjectTreeItem* domain_ = new ObjectTreeItem();
    domain_->setIndex( index_ );
    domain_->setType( Settings::Objects::ObjectType::DOMAINS );
    domain_->setText( COLUMN_NAME, QString( "DOMAIN %1" ).arg( index_ ) );
    domain_->setCheckState( COLUMN_STATUS, Qt::Checked );

    label_domains->addChild( domain_ );
    domains.addElement( index_, domain_ );


    domain_actions_[ index_ ] = new QAction( domain_->text( COLUMN_NAME ), this );
    mn_submenu->addAction( domain_actions_[ index_ ] );
    connect( domain_actions_[ index_ ], &QAction::triggered, [=](){ this->addToDomain1( index_ );} );

    std::vector< std::size_t > regions_;
    bool status_ = getSelectedRegionsList( regions_ );
    if( status_ == true )
        emit addRegionsToDomain( index_, regions_ );
}


bool ObjectTree::getSelectedRegionsList( std::vector< std::size_t >& regions_,
                                         std::vector< std::size_t >& parents_ )
{

    QList<QTreeWidgetItem *> items_ = selectedItems();
    if( items_.empty() == true ) return false;

    for( auto it: items_ )
    {
        ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( it );
        if( obj_->getType() != Settings::Objects::ObjectType::REGION ) continue;

        ObjectTreeItem* const& domain_ = static_cast< ObjectTreeItem* >( obj_->parent() );

        regions_.push_back( obj_->getIndex() );
        parents_.push_back( domain_->getIndex() );
    }

    return true;

}


void ObjectTree::addRegionsInDomain( std::size_t index_, const std::vector< std::size_t >& regions_ )
{

    for( auto it: regions_ )
    {
        ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( regions.getElement( it ) );
        if( obj_->getType() != Settings::Objects::ObjectType::REGION ) continue;

        ObjectTreeItem* reg_ = new ObjectTreeItem();
        reg_->setText( COLUMN_NAME, QString( obj_->text( COLUMN_NAME) ) );
        reg_->setType( Settings::Objects::ObjectType::REGION );
        reg_->setIndex( obj_->getIndex() );
        reg_->setCheckState( COLUMN_STATUS, Qt::Checked );

        ObjectTreeItem* domain_ = domains.getElement( index_ );
        domain_->addChild( reg_ );


        connect( this, &ObjectTree::itemChanged, [=]( QTreeWidgetItem* item_, int column_ )
        {
            if( item_ == reg_ )
            {
                obj_->setCheckState( COLUMN_STATUS, reg_->checkState( COLUMN_STATUS ) );
                obj_->setText( COLUMN_NAME, reg_->text( COLUMN_NAME ) );
            }
            else if( item_ == obj_ )
            {
                reg_->setCheckState( COLUMN_STATUS, obj_->checkState( COLUMN_STATUS ) );
                reg_->setText( COLUMN_NAME, obj_->text( COLUMN_NAME ) );
            }

        } );

    }
}


void ObjectTree::addToDomain1( std::size_t index_ )
{

    if( domains.findElement( index_ ) == false ) return;

    std::vector< std::size_t > regions_;
    bool status_ = getSelectedRegionsList( regions_ );
    if( status_ == true )
        emit addRegionsToDomain( index_, regions_ );
}


void ObjectTree::removeRegionsOfTheirDomains1( const std::vector< std::size_t >& regions_,
                                           const std::vector< std::size_t >& parents_ )
{

    int i = 0;
    for( auto it_: regions_ )
    {

        std::size_t idd_ = parents_[ i ];

        ObjectTreeItem* domain_ = static_cast< ObjectTreeItem* >( domains.getElement( idd_ ) );
        if( domain_ != nullptr )
        {
            int nchild_ = domain_->childCount();
            for( int j = 0; j < nchild_; ++j )
            {
                ObjectTreeItem* reg_ = static_cast< ObjectTreeItem* >( domain_->child( j ) );
                if( reg_->getIndex() != it_ ) continue;
                domain_->removeChild( reg_ );
                reg_ = nullptr;
            }

            if( domain_->childCount() == 0 )
            {
                deleteDomain1( domain_->getIndex() );
            }

         }
         ++i;
    }

}

void ObjectTree::removeFromDomain1()
{

    std::vector< std::size_t > regions_;
    std::vector< std::size_t > parents_;
    bool status_ = getSelectedRegionsList( regions_, parents_ );
    if( status_ == false ) return;


    emit removeRegionsFromTheirDomains( regions_, parents_ );

}


void ObjectTree::deleteDomains1()
{

    QList<QTreeWidgetItem *> items_ = selectedItems();
    if( items_.empty() == true ) return;

    for( auto it: items_ )
    {
        ObjectTreeItem* domain_ = static_cast< ObjectTreeItem* >( it );
        if( domain_->getType() != Settings::Objects::ObjectType::DOMAINS ) continue;

        emit removeDomain( domain_->getIndex() );
    }

}


void ObjectTree::deleteDomain1( std::size_t index_ )
{

    QList<QTreeWidgetItem *> items_ = selectedItems();
    if( items_.empty() == true ) return;

    for( auto it: items_ )
    {
        ObjectTreeItem* domain_ = static_cast< ObjectTreeItem* >( it );
        if( domain_->getType() != Settings::Objects::ObjectType::DOMAINS ) continue;

        QList<QTreeWidgetItem *> children_domains_ = domain_->takeChildren();
        while( children_domains_.isEmpty() == false )
        {
            ObjectTreeItem* reg_ = static_cast< ObjectTreeItem* >( children_domains_.first() );
            if( reg_ == nullptr ) continue;


            domain_->removeChild( reg_ );
            reg_ = nullptr;

            children_domains_.removeFirst();

        }

        label_domains->removeChild( domain_ );

        std::size_t id_ = domain_->getIndex();
        domains.removeElement( id_ );
        mn_submenu->removeAction( domain_actions_[ id_ ] );

        QAction* tmp_ = std::move( domain_actions_[ id_ ] );
        delete tmp_;
        domain_actions_[ id_ ] = nullptr;
        domain_actions_.erase( id_ );


        delete domain_;
        domain_ = nullptr;

    }

    if( label_domains->childCount() == 0 )
        label_domains->setHidden( true );

}







void ObjectTree::createDomain( std::size_t index_ )
{

    if( domains.empty() == true )
    {
        addTopLevelItem( label_domains );
        label_domains->setHidden( false );
    }
    else
    {
        std::size_t id_ = 0;
        for( auto it_: domains )
            id_ = it_.first;

        id_ += 1;
        index_ = id_;
    }

    emit createDomainOfRegions( index_ );


    ObjectTreeItem* domain_ = new ObjectTreeItem();
    domain_->setIndex( index_ );
    domain_->setType( Settings::Objects::ObjectType::DOMAINS );
    domain_->setText( COLUMN_NAME, QString( "DOMAIN %1" ).arg( index_ ) );
    domain_->setCheckState( COLUMN_STATUS, Qt::Checked );

    label_domains->addChild( domain_ );
    domains.addElement( index_, domain_ );


    addToDomain( index_ );

    domain_actions_[ index_ ] = new QAction( domain_->text( COLUMN_NAME ), this );
    mn_submenu->addAction( domain_actions_[ index_ ] );
    connect( domain_actions_[ index_ ], &QAction::triggered, [=](){ this->addToDomain( index_ );} );

}


void ObjectTree::addToDomain( std::size_t index_ )
{

    if( domains.findElement( index_ ) == false ) return;

    QList<QTreeWidgetItem *> items_ = selectedItems();
    if( items_.empty() == true ) return;


    ObjectTreeItem* domain_ = domains.getElement( index_ );
    std::vector< std::size_t > regions_;

    for( auto it: items_ )
    {
        ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( it );
        if( obj_->getType() != Settings::Objects::ObjectType::REGION ) continue;

        ObjectTreeItem* reg_ = new ObjectTreeItem();
        reg_->setText( COLUMN_NAME, QString( obj_->text( COLUMN_NAME) ) );
        reg_->setType( Settings::Objects::ObjectType::REGION );
        reg_->setIndex( obj_->getIndex() );
        reg_->setCheckState( COLUMN_STATUS, Qt::Checked );
        domain_->addChild( reg_ );

        emit addRegionToDomain( obj_->getIndex(), index_ );


        connect( this, &ObjectTree::itemChanged, [=]( QTreeWidgetItem* item_, int column_ )
        {
            if( item_ == reg_ )
            {
                obj_->setCheckState( COLUMN_STATUS, reg_->checkState( COLUMN_STATUS ) );
                obj_->setText( COLUMN_NAME, reg_->text( COLUMN_NAME ) );
            }
            else if( item_ == obj_ )
            {
                reg_->setCheckState( COLUMN_STATUS, obj_->checkState( COLUMN_STATUS ) );
                reg_->setText( COLUMN_NAME, obj_->text( COLUMN_NAME ) );
            }

        } );

    }



}


void ObjectTree::removeFromDomain()
{
    QList<QTreeWidgetItem *> items_ = selectedItems();
    if( items_.empty() == true ) return;

    for( auto it: items_ )
    {
        ObjectTreeItem* obj_ = static_cast< ObjectTreeItem* >( it );
        if( obj_->getType() != Settings::Objects::ObjectType::REGION ) continue;

        ObjectTreeItem* domain_ = static_cast< ObjectTreeItem* >( obj_->parent() );

        emit removeRegionFromDomain( obj_->getIndex(), domain_->getIndex() );


        if( domain_ != nullptr )
        {
            domain_->removeChild( obj_ );
            if( domain_->childCount() == 0 )
            {
                label_domains->removeChild( domain_ );
                delete domain_;
                domain_ = nullptr;
            }

        }

        obj_ = nullptr;
    }

}


void ObjectTree::deleteDomain( std::size_t index_ )
{

    QList<QTreeWidgetItem *> items_ = selectedItems();
    if( items_.empty() == true ) return;

    for( auto it: items_ )
    {
        ObjectTreeItem* domain_ = static_cast< ObjectTreeItem* >( it );
        if( domain_->getType() != Settings::Objects::ObjectType::DOMAINS ) continue;

        emit removeDomain( domain_->getIndex() );


        QList<QTreeWidgetItem *> children_domains_ = domain_->takeChildren();
        while( children_domains_.isEmpty() == false )
        {
            ObjectTreeItem* reg_ = static_cast< ObjectTreeItem* >( children_domains_.first() );
            if( reg_ == nullptr ) continue;

            emit removeRegionFromDomain( reg_->getIndex(), domain_->getIndex() );

            domain_->removeChild( reg_ );
            reg_ = nullptr;

            children_domains_.removeFirst();

        }

        label_domains->removeChild( domain_ );

        delete domain_;
        domain_ = nullptr;

    }

    if( label_domains->childCount() == 0 )
        label_domains->setHidden( true );

}




void ObjectTree::deleteWidgetFromObject( ObjectTreeItem* obj_, int column_ )
{
    ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, column_ ) );
    removeItemWidget( obj_, column_ );
    delete colorpicker_;
}


void ObjectTree::removeInputVolume()
{

    ObjectTreeItem* vol0_ = (ObjectTreeItem* )( topLevelItem( 0 ) );
    if( vol0_ == nullptr ) return;


    QList<QTreeWidgetItem *> children_ = label_stratigraphy->takeChildren();

    while( children_.isEmpty() == false )
    {
        ObjectTreeItem* obj_ = static_cast< ObjectTreeItem* >( children_.first() );
        if( obj_ == nullptr ) continue;

        std::size_t index_ = obj_->getIndex();

        deleteWidgetFromObject( obj_, COLUMN_COLOR );
        label_stratigraphy->removeChild( obj_ );

        if( stratigraphies.findElement( index_ ) == true )
        {
            stratigraphies.deleteElement( index_ );
        }

        obj_ = nullptr;
        children_.removeFirst();
    }

    stratigraphies.clear();
    label_stratigraphy->setHidden( true );



    QList<QTreeWidgetItem *> children1_ = label_structural->takeChildren();

    while( children1_.isEmpty() == false )
    {
        ObjectTreeItem* obj_ = static_cast< ObjectTreeItem* >( children1_.first() );
        if( obj_ == nullptr ) continue;

        std::size_t index_ = obj_->getIndex();

        deleteWidgetFromObject( obj_, COLUMN_COLOR );
        label_structural->removeChild( obj_ );

        if( structurals.findElement( index_ ) == true )
        {
            structurals.deleteElement( index_ );
        }

        obj_ = nullptr;
        children1_.removeFirst();

    }
    structurals.clear();
    label_structural->setHidden( true );


    vol0_->removeChild( label_stratigraphy );
    vol0_->removeChild( label_structural );

    delete vol0_;
    vol0_ = nullptr;
}


void ObjectTree::removeRegions()
{
    ObjectTreeItem* vol1_ = (ObjectTreeItem* )( topLevelItem( 1 ) );
    if( vol1_ == nullptr ) return;

    QList<QTreeWidgetItem *> children_ = vol1_->takeChildren();

    while( children_.isEmpty() == false )
    {
        ObjectTreeItem* reg_ = static_cast< ObjectTreeItem* >( children_.first() );
        if( reg_ == nullptr ) continue;

        std::size_t index_ = reg_->getIndex();

        deleteWidgetFromObject( reg_, COLUMN_COLOR );
        vol1_->removeChild( reg_ );

        if( regions.findElement( index_ ) == true )
        {
            regions.deleteElement( index_ );
        }

        reg_ = nullptr;
        children_.removeFirst();

    }
    regions.clear();

    delete vol1_;
    vol1_ = nullptr;

}


void ObjectTree::removeDomains()
{

    if( label_domains == nullptr ) return;

    QList<QTreeWidgetItem *> children_ = label_domains->takeChildren();

    while( children_.isEmpty() == false )
    {

        ObjectTreeItem* domain_ = static_cast< ObjectTreeItem* >( children_.first() );
        QList<QTreeWidgetItem *> children_domains_ = domain_->takeChildren();

        while( children_domains_.isEmpty() == false )
        {
            ObjectTreeItem* reg_ = static_cast< ObjectTreeItem* >( children_domains_.first() );
            if( reg_ == nullptr ) continue;

            domain_->removeChild( reg_ );
            reg_ = nullptr;

           children_domains_.removeFirst();

        }

        delete domain_;
        domain_ = nullptr;

        children_.removeFirst();

    }
    domains.clear();

    takeTopLevelItem( 2 );
    label_domains->setHidden( true );


}


void ObjectTree::clearSubMenu()
{
    while( domain_actions_.empty() == false )
    {
        QAction* tmp_ = std::move( domain_actions_.begin()->second );
        if( tmp_ == nullptr ) continue;

        mn_submenu->removeAction( tmp_ );
        delete tmp_;

        domain_actions_.erase( domain_actions_.begin()->first );
    }
    domain_actions_.clear();

}


void ObjectTree::clear()
{

    int number_levels_ = topLevelItemCount();

    if( number_levels_ > 2 )
    {
        removeDomains();
    }
    if( number_levels_ > 1 )
    {
        removeRegions();
    }

    clearSubMenu();
    removeInputVolume();


    items.clear();
    QTreeWidget::clear();

}


//void ObjectTree::dragEnterEvent( QDragEnterEvent* event )
//{

//    event->acceptProposedAction();

//}

//void ObjectTree::dragMoveEvent( QDragMoveEvent* event )
//{
//    event->accept();
//}

//void ObjectTree::dropEvent( QDropEvent *event )
//{
//    std::cout << "Testing the drop method" << std::endl << std::flush;

//}

//void ObjectTree::mouseMoveEvent( QMouseEvent *event )
//{
//    std::cout << "Testing the mouse move method" << std::endl << std::flush;
//}

//void ObjectTree::mouseReleaseEvent( QMouseEvent *event )
//{
//    std::cout << "Testing the release method" << std::endl << std::flush;

//    QModelIndex index = indexAt( event->pos() );
//    QTreeWidgetItem* item = itemFromIndex(index);
//    std::cout << "drop on item" << item->text(COLUMN_NAME).toStdString() << std::endl << std::flush;


//}
