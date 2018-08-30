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



#ifndef SKETCHSCENE_H
#define SKETCHSCENE_H

#include <memory>


#include <QGraphicsScene>

#include "./models/inputsketch.h"
//#include "./models/volume_item_wrapper.h"
//#include "./models/object_item_wrapper.h"
//#include "./models/crosssection_item_wrapper.h"
//#include "./models/trajectoryitemwrapper.h"
//#include "./models/coordinate_axes_2d.h"
#include "./models/image_item_wrapper.h"
#include "./core/models/scene.h"


#include "./models/qtgeometries/curveitem.h"
#include "./models/volumeitem.h"
#include "./models/stratigraphyitem.h"
#include "./models/regionitem.h"
#include "./models/crosssectionitem.h"


class SketchScene: public QGraphicsScene/*, public Scene*/
{
    Q_OBJECT


    enum class UserInteraction1 { SKETCHING, RESIZING_BOUNDARY, RESIZING_IMAGE, CREATE_REGION, SELECTING_STRATIGRAPHY_OLD, SELECTING_STRATIGRAPHY, SELECTING_REGION, SELECTING_REGIONS, SELECTING_WELLS };

    ///================================================================================

    enum class UserInteraction { SKETCHING, EDITING_BOUNDARY, EDITING_SCENE, SELECTING, MOVING_IMAGE, RESIZING_IMAGE };


    public:


        SketchScene();
        ~SketchScene();

        void init();

        void setCrossSectionInformation( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

        void createVolume( const std::shared_ptr< Volume >& volume_ );
        void updateVolume();

        void addCrossSection( const std::shared_ptr< CrossSection >& csection_ );
        void updateCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
        void removeCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
        void updateCrossSections();


        void addStratigraphy( const std::shared_ptr< Stratigraphy >& strat_ );
        void updateStratigraphy( const std::size_t& id_ );
        void updateStratigraphies();
        void updateStratigraphiesTrajectories();

        void getSelectedStratigraphies();

        void addRegion( const std::shared_ptr< Regions >& region_ );
        void updateRegion( const std::size_t& id_ );
        void updateRegions();

//        void addWell( const std::shared_ptr< Well >& well_ );

        void enableSketch( bool status_ );
        bool isSketchEnabled() const;

        void clearScene();


    public slots:

        void cancelSketch();
        void submitSketch();
        void setSketchColor( const QColor& color_ );
        void endObject();

        void setSketchingMode();
        void setResizingBoundaryMode( bool status_ );
        void setCreateRegionMode();
        void setOldSelectingStratigraphyMode( bool status_ );
        void setSelectingStratigraphyMode( bool status_ );
        void setSelectingRegionsMode( bool status_ );
        void setResizingImageMode( bool status_ );
        void setSelectingRegionMode( bool status_ );
        //        void setSelectingWellsMode( bool status_ );

        void addImageToCrossSection( const QString& file_ );
        void setImageInCrossSection( const std::string& file_, double ox_, double oy_, double w_, double y_ );
        void removeImageInCrossSection();
        void removeImageInCrossSectionAndUpdate();




   signals:


        void resizeVolumeDimensions( const Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ );

        void ensureObjectsVisibility();

        void sketchDone( const PolyCurve& curve_,
                         const Settings::CrossSection::CrossSectionDirections& dir_ = Settings::CrossSection::CrossSectionDirections::Y, double depth_ = 0 );
        void createObject();

        void setImageToCrossSection( const std::string&, const Settings::CrossSection::CrossSectionDirections&, double, double, double, double, double );

        void removeImageFromCrossSection(  const Settings::CrossSection::CrossSectionDirections&, double );

        void objectSelected( const std::size_t id_ );

        void sendSketchOfSelection( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
        void stopSketchesOfSelection();

        void getRegionByPoint( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


    protected:

        void processSketch();
        void addToSketchesOfSelection();
        void removeSketchesOfSelection();

        void updateImageinCrossSection();


        virtual void mousePressEvent( QGraphicsSceneMouseEvent *event_ );
        virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event_ );
        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event_ );
        virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* event_ );
        virtual void dragEnterEvent( QGraphicsSceneDragDropEvent* event_ );
        virtual void dropEvent( QGraphicsSceneDragDropEvent* event_ );
        virtual void dragMoveEvent( QGraphicsSceneDragDropEvent* event_ );
        virtual void wheelEvent( QGraphicsSceneWheelEvent *event_ );
        virtual void keyPressEvent( QKeyEvent *event );


    protected:



        bool sketch_enabled = true;
        std::shared_ptr< CurveItem > sketch = nullptr;

        ImageItemWrapper* image = nullptr;

        double csection_depth = 0.0;
        Settings::CrossSection::CrossSectionDirections csection_direction = Settings::CrossSection::CrossSectionDirections::Z;

        std::shared_ptr< VolumeItem > volume1 = nullptr;
        std::map< std::size_t, std::shared_ptr< CrossSectionItem > > cross_sections1;
        std::map< std::size_t, std::shared_ptr< StratigraphyItem > > stratigraphies;
        std::map< std::size_t, std::shared_ptr< RegionItem > > regions;
        //        std::map< std::size_t, WellItem* > wells;


        std::vector< std::shared_ptr< CurveItem > > sketches_of_selection;

        UserInteraction1 current_interaction1 = UserInteraction1::SKETCHING;


        QGraphicsEllipseItem* resize_marker = nullptr;
        QGraphicsEllipseItem* move_marker = nullptr;
};

#endif // SKETCHSCENE_H
