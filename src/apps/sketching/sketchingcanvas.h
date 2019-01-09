#ifndef SKETCHINGCANVAS_H
#define SKETCHINGCANVAS_H

#include <QGraphicsView>
#include <QKeyEvent>

#include "sketchscene.h"


/**
 *   The holder to the 2d scene.
 *   A canvas to show the scene. It inherits from QGraphicsView.
 */
class SketchingCanvas: public QGraphicsView
{
    public:

       /**
       * Constructor.
       */
        SketchingCanvas( QWidget *parent_ = nullptr );

        /**
       * Destructor.
       */
        ~SketchingCanvas() override;


       /**
       * Method to retrieves the scene.
       * @param color_ The color of the marker that was used in the depth slider ( see slider in the 3d widget ).
       * @see publicVar()
       * @return std::shared_ptr< SketchScene > A shared pointer to the scene associated to this canvas.
       */
        const std::shared_ptr< SketchScene >& getScene() const;


       /**
       * Method to apply the vertical exaggeration in the scene.
       * @param scale_ The vertical exaggeration value (double).
       * @see publicVar()
       * @return Void.
       */
        void setVerticalExaggeration( double scale_ );


       /**
       * Method to stop applying the vertical exaggeration in the axes of the scene.
       * @see publicVar()
       * @return Void.
       */
        void stopVerticalExaggeration();


       /**
       * Method to retrieves the vertical exaggeration value used in the scene.
       * @see publicVar()
       * @return double return the value of v_exag (double), which corresponds to the vertical exaggeration value used in the scene.
       */
        double getVerticalExaggeration() const;



    protected:

       /**
       * Method to create the scene and its actions.
       * @see publicVar()
       * @return Void.
       */
        void setupScene();



    private:

        std::shared_ptr< SketchScene > scene = nullptr;                 /**< Represents the scene 2d. */

        double v_exag = 1.0;                                            /**< Represents the vertical exaggeration value. */
};

#endif // SKETCHINGCANVAS_H
