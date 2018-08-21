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
#include <QCheckBox>

#include "objecttree.h"
#include "core/widgets/color_picker.h"


ObjectTree::ObjectTree( QWidget *parent )
{
    setHeaderHidden( true );
    setColumnCount( COLUMNS_NUMBER );
    connect( this, &ObjectTree::itemChanged, this, &ObjectTree::filterAction );

    label_stratigraphy = new QTreeWidgetItem();
    label_stratigraphy->setText( COLUMN_NAME, "STRATIGRAPHY" );
    label_stratigraphy->setCheckState( COLUMN_STATUS, Qt::Checked );


    label_structural = new QTreeWidgetItem();
    label_structural->setText( COLUMN_NAME, "STRUCTURAL" );
    label_structural->setCheckState( COLUMN_STATUS, Qt::Checked );

}



void ObjectTree::filterAction( QTreeWidgetItem* item_, std::size_t column_ )
{
    ObjectTreeItem* const& obj_ = static_cast< ObjectTreeItem* >( item_ );

    if( item_ == label_stratigraphy && column_ == COLUMN_STATUS )
        setStratigraphiesVisible( item_->checkState( COLUMN_STATUS ) );


    else if( item_ == label_structural && column_ == COLUMN_STATUS )
        setStructuralsVisible( item_->checkState( COLUMN_STATUS ) );


    else if( obj_->getType() == Settings::Objects::ObjectType::VOLUME )
    {
        if( column_ == COLUMN_STATUS )
        {
//            bool status_ = false;
//            if( obj_->checkState( COLUMN_STATUS ) == Qt::Checked )
//                status_ = true;

            setVolumeVisibility( obj_->checkState( COLUMN_STATUS ) );
//            emit setVolumeVisible( obj_->getIndex(), status_ );
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


void ObjectTree::setVolumeVisibility( const Qt::CheckState& status_ )
{

    bool is_visible_ = ( status_ == Qt::Checked? true:false );
    ObjectTreeItem* vol_ = (ObjectTreeItem* )( topLevelItem( 0 ) );

    setVolumeVisible( 0, is_visible_ );
    label_stratigraphy->setCheckState( COLUMN_STATUS, status_ );
    label_structural->setCheckState( COLUMN_STATUS, status_ );

    emit setVolumeVisible( vol_->getIndex(), is_visible_ );
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


void ObjectTree::clear()
{

    std::vector< std::size_t > diff_regs;


    if( topLevelItemCount() != 0 )
    {

        removeStratigraphies();
        removeStructurals();

        label_stratigraphy->setHidden( true );
        label_structural->setHidden( true );

        if( topLevelItemCount() < 2 )
        {
            ObjectTreeItem* vol0_ = (ObjectTreeItem* )( topLevelItem( 0 ) );
            if( vol0_ != nullptr )
            {
                vol0_->removeChild( label_stratigraphy );
                vol0_->removeChild( label_structural );
                delete vol0_;
                vol0_ = nullptr;
            }

            QTreeWidget::clear();
            items.clear();
            regions.clear();
            stratigraphies.clear();
            structurals.clear();
            return;
        }



        int nchildren = topLevelItem( 1 )->childCount();
        for( int j = 0; j < nchildren; ++j )
        {
            ObjectTreeItem* obj_ = (ObjectTreeItem* )( topLevelItem( 1 )->child( j ) );

            ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );
            removeItemWidget( obj_, COLUMN_COLOR );
            delete colorpicker_;

            if( regions.findElement( obj_->getIndex() ) == true )
            {
                diff_regs.push_back( obj_->getIndex() );
                continue;
            }

            delete obj_;
            obj_ = nullptr;
        }

        for( auto d: diff_regs )
        {
            ObjectTreeItem* obj_ = regions.getElement( d );

            delete obj_;
            obj_ = nullptr;
        }


        ObjectTreeItem* vol0_ = (ObjectTreeItem* )( topLevelItem( 0 ) );
        if( vol0_ != nullptr )
        {
            vol0_->removeChild( label_stratigraphy );
            vol0_->removeChild( label_structural );
            delete vol0_;
            vol0_ = nullptr;
        }

         ObjectTreeItem* vol1_ = (ObjectTreeItem* )( topLevelItem( 1 ) );
         if( vol1_ != nullptr )
         {
             delete vol1_;
             vol1_ = nullptr;
        }

    }




    QTreeWidget::clear();
    items.clear();
    regions.clear();
    stratigraphies.clear();
    structurals.clear();


/*

//    std::vector< std::size_t > diff_objs;
//    std::vector< std::size_t > diff_regs;


//    if( topLevelItemCount() != 0 )
//    {

//        int nchildren = topLevelItem( 0 )->childCount();
//        for( int j = 0; j < nchildren; ++j )
//        {
//            ObjectTreeItem* obj_ = (ObjectTreeItem* )( topLevelItem( 0 )->child( j ) );
//            if( obj_ == nullptr ) continue;

//            ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );
//            removeItemWidget( obj_, COLUMN_COLOR );
//            delete colorpicker_;

//            if( items.findElement( obj_->getIndex() ) == true )
//            {
//                diff_objs.push_back( obj_->getIndex() );
//                continue;
//            }

//            delete obj_;
//            obj_ = nullptr;
//        }

//        for( auto d: diff_objs )
//        {
//            ObjectTreeItem* obj_ = items.getElement( d );

//            delete obj_;
//            obj_ = nullptr;
//        }



//        if( topLevelItemCount() < 2 )
//        {
//            ObjectTreeItem* vol0_ = (ObjectTreeItem* )( topLevelItem( 0 ) );
//            if( vol0_ != nullptr )
//            {
//                delete vol0_;
//                vol0_ = nullptr;
//            }
//            return;
//        }

//        nchildren = topLevelItem( 1 )->childCount();
//        for( int j = 0; j < nchildren; ++j )
//        {
//            ObjectTreeItem* obj_ = (ObjectTreeItem* )( topLevelItem( 1 )->child( j ) );

//            ColorPicker* colorpicker_ = (ColorPicker*)( itemWidget( obj_, COLUMN_COLOR ) );
//            removeItemWidget( obj_, COLUMN_COLOR );
//            delete colorpicker_;

//            if( regions.findElement( obj_->getIndex() ) == true )
//            {
//                diff_regs.push_back( obj_->getIndex() );
//                continue;
//            }

//            delete obj_;
//            obj_ = nullptr;
//        }

//        for( auto d: diff_regs )
//        {
//            ObjectTreeItem* obj_ = regions.getElement( d );

//            delete obj_;
//            obj_ = nullptr;
//        }


//        ObjectTreeItem* vol0_ = (ObjectTreeItem* )( topLevelItem( 0 ) );
//        if( vol0_ != nullptr )
//        {
//            delete vol0_;
//            vol0_ = nullptr;
//        }

//         ObjectTreeItem* vol1_ = (ObjectTreeItem* )( topLevelItem( 1 ) );
//         if( vol1_ != nullptr )
//         {
//             delete vol1_;
//             vol1_ = nullptr;
//        }

//    }


//    QTreeWidget::clear();
//    items.clear();
//    regions.clear();

*/

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
