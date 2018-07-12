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
#include "./models/volume_item_wrapper.h"
#include "./models/object_item_wrapper.h"
#include "./models/crosssection_item_wrapper.h"
#include "./models/trajectoryitemwrapper.h"
#include "./models/coordinate_axes_2d.h"
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


    enum class UserInteraction1 { SKETCHING, RESIZING_BOUNDARY, CREATE_REGION, SELECTING_STRATIGRAPHY, SELECTING_REGIONS, SELECTING_WELLS };

    ///================================================================================

    enum class UserInteraction { SKETCHING, EDITING_BOUNDARY, EDITING_SCENE, SELECTING, MOVING_IMAGE, RESIZING_IMAGE };


    public:


        SketchScene();
        ~SketchScene(){}

        void init();

        void setCrossSectionInformation( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

        void createVolume( const std::shared_ptr< Volume >& volume_ );
        void updateVolume();

        void addStratigraphy( const std::shared_ptr< Stratigraphy >& strat_ );
        void updateStratigraphy( const std::size_t& id_ );

        void addRegion( const std::shared_ptr< Regions >& region_ );
//        void addWell( const std::shared_ptr< Well >& well_ );
        void addCrossSection( const std::shared_ptr< CrossSection >& csection_ );

        void enableSketch( bool status_ );
        bool isSketchEnabled() const;


        ///=================================================================


//        using ObjectsContainer = Container< std::size_t, ObjectItemWrapper* >;
//        using TrajectoriesContainer = Container< std::size_t, TrajectoryItemWrapper* >;
//        using CrossSectionsContainer = Container< std::size_t, CrossSectionItemWrapper* >;



//        SketchScene( CrossSection* const& raw_ );

//        QPixmap addLabel( double depth_, QColor color_);


//        void readCrossSection( CrossSection* const& raw_ );


//        void addVolume( Volume* const& raw_, Settings::CrossSection::CrossSectionDirections dir_ );
//        virtual void addVolume( Volume* const& raw_ ){}
////        virtual void updateVolume();
//        virtual void clearVolume();


//        void addCrossSection( CrossSection* const& raw_ );
//        void removeCrossSection( CrossSection* const& raw_ ){}
//        void updateCrossSection();


//        virtual void addObject( Object* const& raw_ );
//        virtual void addObject( Object* const& raw_, double depth_ );
//        virtual void updateObject(  const std::size_t& index_ );



//        virtual void addTrajectory( Object* const& raw_ );
//        virtual void updateTrajectory(  const std::size_t& index_ );
//        void updateTrajectories();

//        void moveCurrentCrossSection( double depth_ );


//        void setCurrent( bool status_ );
//        bool isCurrent() const;


//        virtual void setCurrentColor( int r, int g, int b );
//        virtual void getCurrentColor( int& r, int& g, int& b );

//        void editItem();
//        void removeItem();


//        void savetoVectorImage( const QString& filename );
//        void savetoRasterImage( const QString& filename );



    public slots:

        void cancelSketch();
        void submitSketch();
        void setSketchColor();
        void endObject();

        void setSketchingMode();
        void setResizingBoundaryMode( bool status_ );
        void setCreateRegionMode();
        void setSelectingStratigraphyMode( bool status_ );
        void setSelectingRegionsMode( bool status_ );
//        void setSelectingWellsMode( bool status_ );

        ///================================================================================



//        void edit( bool status_ );

//        void setImageToCrossSection( const QString& file );
//        void setImageToCrossSection( const QString& file, double ox_, double oy_, double x_, double y_);
//        void updateImageCrossSection();

//        void setModeSketching();
//        void setModeEditingBoundary();
//        void setModeEditingScene();
//        void setModeSelecting();
//        void selectObjectAsBounderingRegion();

//        void setAxesVisible( bool status_ );

//        void clear();



   signals:


        void ensureObjectsVisibility();

        void sketchDone( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        ///=================================================================



//        void acceptVolumeDimensions( Settings::CrossSection::CrossSectionDirections dir_, double w_, double h_ );
//        void acceptCurve( const PolyCurve& curve_, double depth_ );

//        void discard();
//        void commit();
//        void create();

//        void setAsCurrent( double depth_, QGraphicsView* gview_ );
//        void objectSelected( std::size_t index_ );

//        void commitObject();
//        void removeCurveFromObject( double depth_, std::size_t index_ );


//        void isTopViewScene();
//        void isCrossSectionScene();

//        void setImageCrossSection( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ );
//        void removeImageFromCrossSection( double depth_ );





    protected:

        virtual void mousePressEvent( QGraphicsSceneMouseEvent *event_ );
        virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event_ );
        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event_ );
        virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* event_ );
        virtual void dragEnterEvent( QGraphicsSceneDragDropEvent* event_ );
        virtual void dropEvent( QGraphicsSceneDragDropEvent* event_ );
        virtual void dragMoveEvent( QGraphicsSceneDragDropEvent* event_ );
        virtual void wheelEvent( QGraphicsSceneWheelEvent *event_ );
        virtual void keyPressEvent( QKeyEvent *event );



        ///================================================================================



//        void initialize();

//        void startSketch( const QPointF& p );
//        void clearSketch();
//        void finishSketch();


//        void createCrossSectionScene( Volume* const& vol_ );
//        void createTopViewScene( Volume* const& vol_ );

//        void updateCrossSectionScene();
//        void updateTopViewScene();



    protected:


        UserInteraction1 current_interaction1 = UserInteraction1::SKETCHING;

        CurveItem* sketch = nullptr;

        Settings::CrossSection::CrossSectionDirections csection_direction = Settings::CrossSection::CrossSectionDirections::Z;

        double csection_depth = 0.0;


        VolumeItem* volume1 = nullptr;

        std::map< std::size_t, StratigraphyItem* > stratigraphies;
        std::map< std::size_t, RegionItem* > regions;
        //        std::map< std::size_t, WellItem* > wells;
        std::map< std::size_t, CrossSectionItem* > cross_sections1;

        bool sketch_enabled = true;

        // temporary
        int nregions = 0;
        int nwells = 0;
        // temporary

        ///================================================================================

//        struct Color
//        {
//            int red = 255;
//            int green = 0;
//            int blue = 0;
//        } current_color;


//        UserInteraction current_interaction = UserInteraction::SKETCHING;
//        InputSketch* user_input;

//        CrossSection* csection;
//        CrossSectionItemWrapper* main_csection;
//        QGraphicsPixmapItem* csection_image;

//        VolumeItemWrapper* volume;
//        ObjectsContainer objects;
//        CrossSectionsContainer cross_sections;
//        TrajectoriesContainer trajectories;

//        const double ZOOM_SCALE = 1.1;
//        bool is_current = false;

//        CoordinateAxes2d axes;

//        QGraphicsEllipseItem* resize_marker;
//        QGraphicsEllipseItem* move_marker;
//        ImageItemWrapper* image;

//        QGraphicsTextItem* csection_label;
//        QGraphicsEllipseItem* csection_color;

//        bool allow_sketching = true;


};

#endif // SKETCHSCENE_H
