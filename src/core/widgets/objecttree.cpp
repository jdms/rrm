/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file objecttree.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class ObjectTree
 */


#include <iostream>
#include <memory>
#include <limits>
#include <QCheckBox>

#include "objecttree.h"
#include "core/widgets/color_picker.h"
#include "model_metadata_access.hpp"

#include "colors.hpp"


ObjectTree::ObjectTree( QWidget *parent )
{
    setHeaderHidden( true );

    // allows the user select more than one item at time
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setContextMenuPolicy( Qt::CustomContextMenu );
    setColumnCount( COLUMNS_NUMBER );
    setAcceptDrops( true );
    setMouseTracking( true );

    setColumnHidden( COLUMNS_NUMBER, true );


    createMenu();

    connect( this, &ObjectTree::itemChanged, this, &ObjectTree::filterAction );
    connect( this, &ObjectTree::itemClicked, this, &ObjectTree::clickAction );

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

    // an widget edit to the user input any information about the stratigraphy/structural
    wg_log_ = new QInputDialog();
    wg_log_->setInputMode( QInputDialog::TextInput );
    wg_log_->setOption( QInputDialog::UsePlainTextEditForTextInput, true );


    // actions regarding the domains, they will be shown through a menu
    ac_create_domain = new QAction( "Create Domain", this );
    ac_addto_domain = new QAction( "Add to Domain", this );
    ac_removefrom_domain = new QAction( "Remove from Domain", this );
    ac_remove_domain = new QAction( "Delete Domain", this );

    connect( ac_create_domain, &QAction::triggered, [=]()
    {
        emit createDomainOfRegions();

    } );


    connect( ac_removefrom_domain, &QAction::triggered, this, &ObjectTree::removeFromDomain );

    connect( ac_remove_domain, &QAction::triggered, this, &ObjectTree::deleteDomains );



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


    connect( wg_log_, &QInputDialog::accepted, [=](){ sendSurfaceLog(); } );


}


void ObjectTree::sendSurfaceLog()
{
    emit saveSurfaceLog( surface_selected_, wg_log_->textValue() );
}



void ObjectTree::showMenu( const QPoint& pos_ )
{
    QList< QTreeWidgetItem* > selected_ =  selectedItems();
   if( selected_.empty() == true ) return;

   bool region_ = false;
   bool region_in_domain_ = false;
   bool surface_ = false;
   bool domain_ = false;

   QString label_name_;

   for( auto it_: selected_ )
   {
        // checking the type of the object to know which option will be shown

        ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( it_ );
        if( obj_->getType() == Settings::Objects::ObjectType::STRATIGRAPHY ||  obj_->getType() == Settings::Objects::ObjectType::STRUCTURAL )
        {
            surface_ = true;
            label_name_ = obj_->text( COLUMN_NAME ).append( QString( ": " ) );
            surface_selected_ = static_cast< int >( obj_->getIndex() );
        }
        else if( obj_->getType() == Settings::Objects::ObjectType::REGION )
        {
            region_ = true;
            ObjectTreeItem* parent_ = static_cast<ObjectTreeItem*>( obj_->parent() );
            if (parent_->getType() == Settings::Objects::ObjectType::DOMAINS )
            {
                region_in_domain_ = true;
            }
        }
        else if( obj_->getType() == Settings::Objects::ObjectType::DOMAINS )
        {
            domain_ = true;
        }

   }



    if( surface_ == true )
    {
        clearSelection();

        if( region_ == true || domain_ == true )
        {
            return;
        }

        // region_ = false;

        // if the object is a stratigraphy or a structural, the log edit will be shown.
        // if many objects are selected we keep the last one

        ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( selected_.back() );
        obj_->setSelected( true );

        QString log_;
        wg_log_->setLabelText( label_name_ );
        emit getSurfaceLog( surface_selected_, log_ );

        wg_log_->setTextValue( log_ );
        wg_log_->show();
        return;
    }

    if( domain_ == true && region_ == true )
    {
        clearSelection();
        return;
    }


    ac_create_domain->setEnabled( region_ );
    ac_removefrom_domain->setEnabled( region_in_domain_ );
    ac_remove_domain->setEnabled( domain_ );
    mn_submenu->setEnabled( region_ );
    mn_menu->exec( mapToGlobal( pos_ ) );

}


void ObjectTree::filterAction( QTreeWidgetItem* item_, std::size_t column_ )
{

    // method called when some object has been changed

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
        }
        else if( column_ == COLUMN_NAME )
        {
            auto index_ = obj_->getIndex();
            if (domain_actions_[ index_ ] != nullptr) {
                domain_actions_[ index_ ]->setText( obj_->text(COLUMN_NAME) );
                //delete domain_actions_[ index_ ];
            }
            // domain_actions_[ index_ ] = new QAction( obj_->text( COLUMN_NAME ), this );
            // mn_submenu->addAction( domain_actions_[ index_ ] );
            // connect( domain_actions_[ index_ ], &QAction::triggered, [=](){ this->addToDomain( index_ );} );

            emit setDomainName( obj_->getIndex(), obj_->text( COLUMN_NAME ).toStdString() );
        }
    }

}


void ObjectTree::clickAction( QTreeWidgetItem* item_, std::size_t column_ )
{

    // method called when some object has been clicked
    // this was implemented so that every time the user click on the COLUMN_DETAILS
    // the data regarding the volume will be changed from percentage to the
    // scalar and vice versa
    //

    ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( item_ );
    if( (obj_->getType() == Settings::Objects::ObjectType::REGION)
            || (obj_->getType() == Settings::Objects::ObjectType::DOMAINS)
            /* || (obj_->getType() == Settings::Objects::ObjectType::VOLUME) */
            )
    {
        if( column_ == COLUMN_DETAILS )
        {
            is_perc = !is_perc;
            // double volume_ = volume_regions[ obj_->getIndex() ];

            ObjectTreeItem* vol_ = ( ObjectTreeItem* ) topLevelItem( 0 );
            if( vol_ == nullptr ) return;

            ObjectTreeItem* vol1_ = ( ObjectTreeItem* ) topLevelItem( 1 );
            if( vol1_ == nullptr ) return;

            double total_ = 0.0;
            for( auto it: volume_regions )
            {
                total_ += it.second;
            }

            if( total_ == 0 ) return;

            int nchild_ = vol1_->childCount();

            if( is_perc == false )
            {
                vol_->setText( COLUMN_DETAILS, QString::number( total_, 'g', 3 ).append( " m3" ) );

                for( int i = 0; i < nchild_; ++i )
                {
                    ObjectTreeItem* obj1_ = ( ObjectTreeItem* ) vol1_->child( i );
                    if( obj1_ == nullptr ) continue;

                    double volume1_ = volume_regions[ obj1_->getIndex() ];
                    obj1_->setText( COLUMN_DETAILS, QString::number( volume1_, 'g', 3 ).append( " m3" ) );
                }
                if( label_domains->childCount() > 0 )
                {
                    int nchildd_ = label_domains->childCount();
                    for( int i = 0; i < nchildd_; ++i )
                    {
                        ObjectTreeItem* domain_ = ( ObjectTreeItem* ) label_domains->child( i );
                        if( domain_ == nullptr ) continue;
                        double volume1_ = volume_domains[ domain_->getIndex() ];
                        domain_->setText( COLUMN_DETAILS, QString::number( volume1_, 'g', 3 ).append( " m3" ) );
                    }
                }
            }
            else
            {
                vol_->setText( COLUMN_DETAILS, QString::number( total_, 'g', 1 ).append( " m3" ) + QString( "(100%)" )  );

                for( int i = 0; i < nchild_; ++i )
                {
                    ObjectTreeItem* obj1_ = ( ObjectTreeItem* ) vol1_->child( i );
                    if( obj1_ == nullptr ) continue;

                    double volume1_ = volume_regions[ obj1_->getIndex() ];
                    double perc1_ = 100*( volume1_/total_ );
                    if (perc1_ >= 0.1)
                        obj1_->setText( COLUMN_DETAILS, QString::number( perc1_, 'f', 1 ).append( "%" ) );
                    else
                        obj1_->setText( COLUMN_DETAILS, QString( "<0.1%" ) );
                }

                if( label_domains->childCount() > 0 )
                {
                    int nchildd_ = label_domains->childCount();
                    for( int i = 0; i < nchildd_; ++i )
                    {
                        ObjectTreeItem* domain_ = ( ObjectTreeItem* ) label_domains->child( i );
                        if( domain_ == nullptr ) continue;
                        double volume1_ = volume_domains[ domain_->getIndex() ];
                        double perc1_ = 100*( volume1_/total_ );
                        if (perc1_ >= 0.1)
                            domain_->setText( COLUMN_DETAILS, QString::number( perc1_, 'f', 1 ).append( "%" ) );
                        else
                            domain_->setText( COLUMN_DETAILS, QString( "<0.1%" ) );
                    }
                }
            }

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

    // if the object tree contains regions/and domains, so there are at least 2 top
    // level items: the input volume and the output

    if( topLevelItemCount() < 2 )
        return;

    int nchildren = topLevelItem( 1 )->childCount();
    for( int j = 0; j < nchildren; ++j )
    {
        ObjectTreeItem* obj_ = (ObjectTreeItem* )( topLevelItem( 1 )->child( j ) );

        ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );

        // first the color picker is removed from the item
        removeItemWidget( obj_, COLUMN_COLOR );
        delete colorpicker_;

        // after the object is deleted
        delete obj_;
        obj_ = nullptr;
    }

    //deleting the top level 'OUTPUT VOLUME'
    ObjectTreeItem* vol1_ = (ObjectTreeItem* )( topLevelItem( 1 ) );
    if( vol1_ != nullptr )
    {
        delete vol1_;
        vol1_ = nullptr;
   }

    regions.clear();
}


void ObjectTree::setVolumeVisibility( std::size_t index_, const Qt::CheckState& status_ )
{

    bool is_visible_ = ( status_ == Qt::Checked? true:false );
    ObjectTreeItem* vol_ = (ObjectTreeItem* )( topLevelItem( index_ ) );

    if( index_ == 0 )
    {
        // if the volume is hidden, also will be the stratigraphies and structurals
        label_stratigraphy->setCheckState( COLUMN_STATUS, status_ );
        label_structural->setCheckState( COLUMN_STATUS, status_ );
    }
    emit setVolumeVisible( index_, is_visible_ );
    update();

}


void ObjectTree::setTotalVolume( double volume_ )
{
    ObjectTreeItem* vol_ = ( ObjectTreeItem* ) topLevelItem( 0 );
    vol_->setText( COLUMN_DETAILS, QString::number( volume_, 'g', 3 ).append( " m3" ) );
    update();
}



void ObjectTree::addObject( std::size_t index_, const Settings::Objects::ObjectType& type_,
                            const std::string& name_,  const int& red_,
                            const int& green_,  const int& blue_ )
{

    ObjectTreeItem* vol_ = ( ObjectTreeItem* ) topLevelItem( 0 );
    if( vol_ == nullptr ) return;

    if( items.findElement( index_ ) == true ) return;

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
        // if it is the first stratigraphy to be added, set the label_stratigraphy as visible
        if( stratigraphies.empty() == true )
        {
            label_stratigraphy->setHidden( false );
        }

        label_stratigraphy->addChild( obj_ );
        stratigraphies.addElement( index_, obj_ );

    }

    else if( type_ == Settings::Objects::ObjectType::STRUCTURAL )
    {

        // if it is the first structural to be added, set the label_structural as visible
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


void ObjectTree::addRegion( std::size_t index_, const std::string& name_,  const int& red_, const int& green_,  const int& blue_,
                            double volume_, double perc_ )
{
    ObjectTreeItem* region_ = new ObjectTreeItem();
    region_->setIndex( index_ );
    region_->setType( Settings::Objects::ObjectType::REGION );
    region_->setText( COLUMN_NAME, QString( name_.c_str() ) );
    region_->setCheckState( COLUMN_STATUS, Qt::Checked );
    region_->setText( COLUMN_DETAILS, QString::number( volume_, 'g', 3 ).append( " m3" ) );
    region_->setFlags( region_->flags() & ~Qt::ItemIsEditable );

    // We need to pad index_ with zeros because Qt orders the tree itens using
    // the lexicographical ordering on strings
    // https://stackoverflow.com/questions/2618414/convert-an-int-to-a-qstring-with-zero-padding-leading-zeroes

    // QString().arg( index_ == number, 
    //                    10 == digits to pad, 
    //                    10 == print as decimal number (i.e. use base 10), 
    //            QChar('0') == char to use as padding );
    region_->setText( COLUMNS_NUMBER , QString( "%1" ).arg( index_, 10, 10, QChar('0') ) );


    ObjectTreeItem* vol1_ = ( ObjectTreeItem* ) topLevelItem( 1 );
    if( vol1_ == nullptr ) return;

    volume_regions[ index_ ] = volume_;

    vol1_->addChild( region_ );
    vol1_->sortChildren( COLUMNS_NUMBER, Qt::DescendingOrder );

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


void ObjectTree::updateVolumeDomain( std::size_t index_, double volume_ )
{
    ObjectTreeItem* domain_ = domains.getElement( index_ );
    if(domain_)
    {
        domain_->setText( COLUMN_DETAILS, QString::number( volume_, 'g', 3 ).append( " m3" ) );
        volume_domains[ index_ ] = volume_;
    }
}


bool ObjectTree::loadDomain( std::size_t index_ )
{
    if( domains.findElement( index_ )  == true ) return false;

    if ( MMA::domains().find(index_) == MMA::domains().end() ) return false;
    auto& domain = MMA::domains()[index_];

    if( domains.empty() == true )
    {
        addTopLevelItem( label_domains );
        label_domains->setHidden( false );
    }

    ObjectTreeItem* domain_ = new ObjectTreeItem();
    domain_->setIndex( index_ );
    domain_->setType( Settings::Objects::ObjectType::DOMAINS );
    domain_->setText( COLUMN_NAME, MMA::name(domain).empty() ? 
            QString( "DOMAIN %1" ).arg( index_ ) : QString::fromUtf8(MMA::name(domain).c_str()));
    domain_->setCheckState( COLUMN_STATUS, Qt::Checked );

    label_domains->addChild( domain_ );

    ColorPicker* colorpicker_ = new ColorPicker( this );
    /* colorpicker_->setColor( QColor(0, 0, 0, 0) ); */
    QColor color_;
    if ( !MMA::getColor(domain, color_) )
    {
        color_ = DomainsColors.color( index_ );
    }
    colorpicker_->setColor( color_ );
    connect( colorpicker_, &ColorPicker::colorSelected, [=]( const QColor& color_ ){ emit setDomainColor( index_, color_ ); } );

    setItemWidget( domain_, COLUMN_COLOR, colorpicker_ );
    domains.addElement( index_, domain_ );

    // for each domain created it will exist an associated action
    // e.g., so that the user can choose which domain he/she wants to add regions
    domain_actions_[ index_ ] = new QAction( colorpicker_->currentIcon(), domain_->text( COLUMN_NAME ), this );
    mn_submenu->addAction( domain_actions_[ index_ ] );
    connect( domain_actions_[ index_ ], &QAction::triggered, [=](){ this->addToDomain( index_ );} );

    std::vector< std::size_t > regions_;
    for (auto& r : domain.regions)
    {
        if ( r.id() > -1 )
            regions_.push_back(r.id());
    }

    if( !regions_.empty() )
        addRegionsInDomain(index_, regions_);

    sortDomains();

    return true;
}

bool ObjectTree::createDomain( std::size_t index_ )
{
    if( domains.findElement( index_ )  == true ) return false;

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

    ColorPicker* colorpicker_ = new ColorPicker( this );
    /* colorpicker_->setColor( QColor(0, 0, 0, 0) ); */
    colorpicker_->setColor( DomainsColors.color(index_) );
    connect( colorpicker_, &ColorPicker::colorSelected, [=]( const QColor& color_ ){ emit setDomainColor( index_, color_ ); } );

    setItemWidget( domain_, COLUMN_COLOR, colorpicker_ );
    domains.addElement( index_, domain_ );

    // for each domain created it will exist an associated action
    // e.g., so that the user can choose which domain he/she wants to add regions
    domain_actions_[ index_ ] = new QAction(colorpicker_->currentIcon(), domain_->text( COLUMN_NAME ), this );
    mn_submenu->addAction( domain_actions_[ index_ ] );
    connect( domain_actions_[ index_ ], &QAction::triggered, [=](){ this->addToDomain( index_ );} );

    std::vector< std::size_t > regions_;
    bool status_ = getSelectedRegionsList( regions_ );
    if( status_ == true )
        emit addRegionsToDomain( index_, regions_ );

    sortDomains();

    return true;
}


bool ObjectTree::updateDomainActions( std::size_t index_ )
{
    if ( !domains.findElement( index_ ) )
    {
        return false;
    }

    ObjectTreeItem* domain_ = static_cast< ObjectTreeItem* >( domains.getElement( index_ ) );
    if( domain_->getType() != Settings::Objects::ObjectType::DOMAINS )
    {
        return false;
    }

    auto* action_ = domain_actions_[ index_ ];
    if (action_ == nullptr)
    {
        domain_actions_[ index_ ] = new QAction( /*colorpicker_->currentIcon(), domain_->text( COLUMN_NAME ),*/ this );
        mn_submenu->addAction( domain_actions_[ index_ ] );
        connect( domain_actions_[ index_ ], &QAction::triggered, [=](){ this->addToDomain( index_ );} );
        action_ = domain_actions_[ index_ ];
    }
    action_->setText( domain_->text(COLUMN_NAME) );

    ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( domain_, COLUMN_COLOR ) );
    if (colorpicker_)
    {
        action_->setIcon( colorpicker_->currentIcon() );
    }

    return true;
}

bool ObjectTree::getSelectedRegionsList( std::vector< std::size_t >& regions_ )
{
    std::vector<std::size_t> parents;
    return getSelectedRegionsList(regions_, parents);
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

        // NOTICE:
        // notice that the parent here might be something different from a domainm
        // e.g., if region is selected in Region List, obj_->parent() is a "VOLUME".
        ObjectTreeItem* const& domain_ = static_cast< ObjectTreeItem* >( obj_->parent() );

        regions_.push_back( obj_->getIndex() );
        parents_.push_back( domain_->getIndex() );
    }

    return true;

}


// after to be added inside controller
void ObjectTree::addRegionsInDomain( std::size_t index_, const std::vector< std::size_t >& regions_ )
{

    ObjectTreeItem* domain_ = domains.getElement( index_ );
    int minor_ = domain_->text( COLUMNS_NUMBER ).toInt();

    for( auto it: regions_ )
    {
        if ( regions.findElement( it ) == false ) continue;
        ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( regions.getElement( it ) );
        if( obj_->getType() != Settings::Objects::ObjectType::REGION ) continue;

        ObjectTreeItem* reg_ = new ObjectTreeItem();
        reg_->setText( COLUMN_NAME, QString( obj_->text( COLUMN_NAME) ) );
        reg_->setType( Settings::Objects::ObjectType::REGION );
        reg_->setIndex( obj_->getIndex() );
        reg_->setCheckState( COLUMN_STATUS, Qt::Checked );


        domain_->addChild( reg_ );


        // getting the minor index of the regions to be the index of the domain in
        // the object tree, so that after the domains could be sorted
        if( obj_->getIndex() <= minor_ )
            minor_ = obj_->getIndex();


        connect( this, &ObjectTree::itemChanged, [=, &reg_]( QTreeWidgetItem* item_, int column_ )
        {

            if( column_ != COLUMN_STATUS || column_ != COLUMN_NAME ) return;
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

    int ind_ = label_domains->indexOfChild( domain_ );
    if( ind_ > 0 )
    {
        ObjectTreeItem* domain_tree_ = static_cast< ObjectTreeItem* >( label_domains->child( ind_ ) );
        domain_tree_->setText( COLUMNS_NUMBER , QString( "%1" ).arg( minor_, 10, 10, QChar('0') ) );
    }

    label_domains->sortChildren( COLUMNS_NUMBER, Qt::DescendingOrder );

    sortDomains();
}


// used for flow diagnostics
void ObjectTree::addRegionsInDomain( std::size_t index_, const std::set< std::size_t >& regions_ )
{

    // when the flow diagnostics is on, the surfaces, regions and domains are sorted
    ObjectTreeItem* domain_ = domains.getElement( index_ );
    int minor_ = domain_->text( COLUMNS_NUMBER ).toInt();

    for( auto it: regions_ )
    {
        ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( regions.getElement( it ) );
        if( obj_->getType() != Settings::Objects::ObjectType::REGION ) continue;

        ObjectTreeItem* reg_ = new ObjectTreeItem();
        reg_->setText( COLUMN_NAME, QString( obj_->text( COLUMN_NAME) ) );
        reg_->setType( Settings::Objects::ObjectType::REGION );
        reg_->setIndex( obj_->getIndex() );
        reg_->setCheckState( COLUMN_STATUS, Qt::Checked );

        domain_->addChild( reg_ );

        if( obj_->getIndex() <= minor_ )
            minor_ = obj_->getIndex();

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


    int ind_ = label_domains->indexOfChild( domain_ );
    if( ind_ > 0 )
    {
        ObjectTreeItem* domain_tree_ = static_cast< ObjectTreeItem* >( label_domains->child( ind_ ) );
        domain_tree_->setText( COLUMNS_NUMBER , QString( "%1" ).arg( minor_, 10, 10, QChar('0')) );
    }

    label_domains->sortChildren( COLUMNS_NUMBER, Qt::DescendingOrder );

    sortDomains();
}


// used by menu
void ObjectTree::addToDomain( std::size_t index_ )
{

    if( domains.findElement( index_ ) == false ) return;

    std::vector< std::size_t > regions_;
    bool status_ = getSelectedRegionsList( regions_ );
    if( status_ == true )
        emit addRegionsToDomain( index_, regions_ );

    sortDomains();
}


void ObjectTree::removeRegionsOfTheirDomains( const std::vector< std::size_t >& regions_,
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
                delete reg_;
                reg_ = nullptr;
                break;
            }


            // deleting the domains
            if( domain_->childCount() == 0 )
            {
                deleteDomain( domain_->getIndex() );
            }

         }
         ++i;
    }


    sortDomains();



}


void ObjectTree::removeRegionsOfTheirDomainsNoDelete( const std::vector< std::size_t >& regions_,
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
                delete reg_;
                reg_ = nullptr;
                break;
            }

         }
         ++i;
    }


    sortDomains();



}

void ObjectTree::removeFromDomain()
{

    std::vector< std::size_t > regions_;
    std::vector< std::size_t > parents_;
    bool status_ = getSelectedRegionsList( regions_, parents_ );
    if( status_ == false ) return;


    emit removeRegionsFromTheirDomains( regions_, parents_ );

    sortDomains();
}


//TODO: methods need to be improved, since many of them has duplicated code

void ObjectTree::deleteDomains()
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

            // removing all the regions from the domains before it be deleted
            emit removeRegionFromDomain( reg_->getIndex(), domain_->getIndex() );

            domain_->removeChild( reg_ );
            reg_ = nullptr;

            children_domains_.removeFirst();

        }

        label_domains->removeChild( domain_ );

        emit removeDomain( domain_->getIndex() );


    }

    if( label_domains->childCount() == 0 )
        label_domains->setHidden( true );


    sortDomains();
}


void ObjectTree::deleteDomain( std::size_t index_ )
{

    ObjectTreeItem* domain_ = domains.getElement( index_ );
    if( domain_ == nullptr ) return;
    if( domain_->getType() != Settings::Objects::ObjectType::DOMAINS ) return;


    QList<QTreeWidgetItem *> children_domains_ = domain_->takeChildren();
    std::vector< std::size_t > regions_;
    std::vector< std::size_t > parents_;

    for( int i = 0; i < children_domains_.size(); ++i )
    {
        ObjectTreeItem* reg_ = static_cast< ObjectTreeItem* >( children_domains_.first() );
        if( reg_ == nullptr ) continue;

        regions_.push_back( reg_->getIndex() );
        parents_.push_back( domain_->getIndex() );
    }

     // removing all the regions from the domains before it be deleted
    if( regions_.empty() == false )
       emit removeRegionsFromTheirDomainsNoDelete( regions_, parents_ );

    label_domains->removeChild( domain_ );

    std::size_t id_ = domain_->getIndex();
    domains.removeElement( id_ );

    if( domain_actions_.find( id_ ) != domain_actions_.end() )
    {
        mn_submenu->removeAction( domain_actions_[ id_ ] );

        QAction* tmp_ = std::move( domain_actions_[ id_ ] );
        delete tmp_;

        domain_actions_[ id_ ] = nullptr;
        domain_actions_.erase( id_ );

    }


    delete domain_;
    domain_ = nullptr;


    sortDomains();
}


void ObjectTree::sortDomains()
{
    for( auto& it: domains )
    {
        ObjectTreeItem* domain_ = static_cast< ObjectTreeItem* >( it.second );
        std::size_t domain_id_ = it.first;
        if( domain_ == nullptr ) continue;

        //
        // This code sorts domains based on the regions they contain
        //
        /* int nchild_ = domain_->childCount(); */
        /* int minor_ = std::numeric_limits<int>::max(); */

        /* for( int j = 0; j < nchild_; ++j ) */
        /* { */
        /*     ObjectTreeItem* reg_ = static_cast< ObjectTreeItem* >( domain_->child( j ) ); */
        /*     if( reg_ == nullptr ) continue; */

        /*     if( reg_->getIndex() > minor_ ) continue; */
        /*     minor_ = reg_->getIndex(); */

        /* } */

        /* int ind_ = label_domains->indexOfChild( domain_ ); */
        /* if( ind_ < 0 ) continue; */

        /* ObjectTreeItem* domain_tree_ = static_cast< ObjectTreeItem* >( label_domains->child( ind_ ) ); */
        /* if( domain_tree_ == nullptr ) continue; */
        /* domain_tree_->setText( COLUMNS_NUMBER , QString( "%1" ).arg( minor_, 10, 10, QChar('0') ) ); */

        // This sorts domains by the order they are created
        domain_->setText( COLUMNS_NUMBER, QString( "%1" ).arg( domain_id_, 10, 10, QChar('0') ) );
    }

    /* label_domains->sortChildren( COLUMNS_NUMBER, Qt::DescendingOrder ); */
    label_domains->sortChildren( COLUMNS_NUMBER, Qt::AscendingOrder );

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

        // removing color picker from the item
        deleteWidgetFromObject( obj_, COLUMN_COLOR );
        label_stratigraphy->removeChild( obj_ );


        // removing all stratigraphies before delete the volume
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

        // removing all structurals before delete the volume
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
        deleteDomain( domain_->getIndex() );
        children_.removeFirst();

    }
    domains.clear();

    // removing the top-level item
    takeTopLevelItem( 2 );
    label_domains->setHidden( true );


}


void ObjectTree::sortStratigraphies( std::vector< std::size_t > indexes_ )
{

    int nstratigraphies_ = label_stratigraphy->childCount();
    int nstructurals_ = label_structural->childCount();

    int ntotal_ = nstratigraphies_ + nstructurals_ ;
    if( ntotal_ != indexes_.size() ) return;

    for( int i = 0; i < ntotal_; ++i )
    {
        std::size_t index_ = indexes_[ i ];
        ObjectTreeItem* obj_ = nullptr;
        ObjectTreeItem* obj_tree_ = nullptr;
        int ind_ = 0;

        if( stratigraphies.findElement( index_ ) == true )
        {
            obj_ = stratigraphies.getElement( index_ );
            ind_ = label_stratigraphy->indexOfChild( obj_ );
            obj_tree_ = static_cast< ObjectTreeItem* >( label_stratigraphy->child( ind_ ) );
        }
        else if( structurals.findElement( index_ ) == true )
        {
            obj_ = structurals.getElement( index_ );
            ind_ = label_structural->indexOfChild( obj_ );
            obj_tree_ = static_cast< ObjectTreeItem* >( label_structural->child( ind_ ) );
        }

        if( ( obj_ == nullptr ) || ( ind_ < 0 ) ) continue;

        obj_tree_->setText( COLUMNS_NUMBER, QString( "%1" ).arg( i, 10, 10, QChar('0') ) );

    }

    // ordering in descending order -- note that COLUMNS_NUMBER - 1 should be
    // hidden, since it serves only to help sorting
    label_stratigraphy->sortChildren( COLUMNS_NUMBER, Qt::DescendingOrder );
    label_structural->sortChildren( COLUMNS_NUMBER, Qt::DescendingOrder );


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


    volume_regions.clear();
    volume_domains.clear();
    is_perc = false;
    items.clear();
    QTreeWidget::clear();

}

