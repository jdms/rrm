#ifndef SKETCHSCENE_H
#define SKETCHSCENE_H

#include <QGraphicsScene>

#include "./models/inputsketch.h"
#include "./models/volume_item_wrapper.h"
#include "./models/object_item_wrapper.h"
#include "./models/crosssection_item_wrapper.h"
#include "./models/trajectoryitemwrapper.h"
#include "./models/coordinate_axes_2d.h"
#include "./models/image_item_wrapper.h"
#include "./core/models/scene.h"

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


        QPixmap addLabel( double depth_, QColor color_);


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

        void moveCurrentCrossSection( double depth_ );


        void setCurrent( bool status_ );
        bool isCurrent() const;


        virtual void setCurrentColor( int r, int g, int b );
        virtual void getCurrentColor( int& r, int& g, int& b );

        void editItem();
        void removeItem();


        void savetoVectorImage( const QString& filename );
        void savetoRasterImage( const QString& filename );



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

        void startSketch( const QPointF& p );
        void clearSketch();
        void finishSketch();


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
        CrossSectionItemWrapper* main_csection;
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

        QGraphicsTextItem* csection_label;
        QGraphicsEllipseItem* csection_color;

        bool allow_sketching = true;


};

#endif // SKETCHSCENE_H
