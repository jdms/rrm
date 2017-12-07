#ifndef SKETCHSCENE_H
#define SKETCHSCENE_H

#include <QGraphicsScene>

#include "inputsketch.h"
#include "./item_wrappers/volume_item_wrapper.h"
#include "./item_wrappers/object_item_wrapper.h"
#include "./item_wrappers/crosssection_item_wrapper.h"
#include "./item_wrappers/trajectoryitemwrapper.h"
#include "./item_wrappers/coordinate_axes_2d.h"
#include "./item_wrappers/image_item_wrapper.h"
#include "./core/base/models/scene.h"

class SketchScene: public QGraphicsScene, public Scene
{
    Q_OBJECT

    enum class UserInteraction { SKETCHING, EDITING_BOUNDARY, EDITING_SCENE, SELECTING, MOVING_IMAGE, RESIZING_IMAGE };


    public:


        using ObjectsContainer = Container< std::size_t, ObjectItemWrapper* >;
        using TrajectoriesContainer = Container< std::size_t, TrajectoryItemWrapper* >;
        using CrossSectionsContainer = Container< std::size_t, CrossSectionItemWrapper* >;


        SketchScene();
        SketchScene( CrossSection* const& raw_ );


        void readCrossSection( CrossSection* const& raw_ );



        void addVolume( Volume* const& raw_, Settings::CrossSection::CrossSectionDirections dir_ );
        virtual void addVolume( Volume* const& raw_ ){}
        virtual void updateVolume();
        virtual void clearVolume();


        void addCrossSection( CrossSection* const& raw_ );
        void removeCrossSection( CrossSection* const& raw_ ){}
        void updateCrossSection();


        virtual void addObject( Object* const& raw_ );
        virtual void addObject( Object* const& raw_, double depth_ );
        virtual void updateObject(  const std::size_t& index_ );



        virtual void addTrajectory( Object* const& raw_ );
        virtual void updateTrajectory(  const std::size_t& index_ );
        void updateTrajectories();



        void setCurrent( bool status_ );
        bool isCurrent() const;


        virtual void setCurrentColor( int r, int g, int b );
        virtual void getCurrentColor( int& r, int& g, int& b );

        void editItem();
        void removeItem();




    signals:



        void acceptVolumeDimensions( Settings::CrossSection::CrossSectionDirections dir_, double w_, double h_ );
        void acceptCurve( const PolyCurve& curve_, double depth_ );

        void discard();
        void commit();
        void create();

        void setAsCurrent( double depth_, QGraphicsView* gview_ );
        void objectSelected( std::size_t index_ );

        void commitObject();
        void removeCurveFromObject( double depth_, std::size_t index_ );


        void isTopViewScene();
        void isCrossSectionScene();

        void setImageCrossSection( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ );
        void removeImageFromCrossSection( double depth_ );


    public slots:

        void edit( bool status_ );

        void setImageToCrossSection( const QString& file );
        void setImageToCrossSection( const QString& file, double ox_, double oy_, double x_, double y_);
        void updateImageCrossSection();

        void setModeSketching();
        void setModeEditingBoundary();
        void setModeEditingScene();
        void setModeSelecting();
        void selectObjectAsBounderingRegion();

        void setAxesVisible( bool status_ );

        void clear();



    protected:


        void initialize();


        void createCrossSectionScene( Volume* const& vol_ );
        void createTopViewScene( Volume* const& vol_ );

        void updateCrossSectionScene();
        void updateTopViewScene();


        virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
        virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event );
        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
        virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* event );
        virtual void dragEnterEvent( QGraphicsSceneDragDropEvent* event );
        virtual void dropEvent( QGraphicsSceneDragDropEvent* event );
        virtual void dragMoveEvent( QGraphicsSceneDragDropEvent* event );

        virtual void wheelEvent( QGraphicsSceneWheelEvent *event );

        virtual void keyPressEvent( QKeyEvent *event );

    protected:

        struct Color
        {
            int red = 255;
            int green = 0;
            int blue = 0;
        } current_color;


        UserInteraction current_interaction = UserInteraction::SKETCHING;
        InputSketch* user_input;

        CrossSection* csection;
        QGraphicsPixmapItem* csection_image;

        VolumeItemWrapper* volume;
        ObjectsContainer objects;
        CrossSectionsContainer cross_sections;
        TrajectoriesContainer trajectories;

        const double ZOOM_SCALE = 1.1;
        bool is_current = false;

        CoordinateAxes2d axes;

        QGraphicsEllipseItem* resize_marker;
        QGraphicsEllipseItem* move_marker;
        ImageItemWrapper* image;


};

#endif // SKETCHSCENE_H
