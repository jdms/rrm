#ifndef SKETCHINGCANVAS_H
#define SKETCHINGCANVAS_H

#include <QtWidgets/QGraphicsView>
#include <QLabel>
#include <QWheelEvent>
#include <QFileDialog>

#include "SketchingScene.h"

class SketchingCanvas: public QGraphicsView
{
	Q_OBJECT
	
	
	public:
	
        SketchingCanvas( QWidget* parent = 0 );
		void setup();

    public slots:

        void updateCoordinates( float posx, float posy );
        void screenshot();

        inline void insertSketch(){ scene->insertLastSketch(); }
        inline void undoLastSketch(){ scene->undoLastSketch(); }
        inline void updateColor( const QColor& color ){ scene->updateColor( color ); }


    signals:

        void newBoundary();


    protected:

        void createConnections();
        void wheelEvent( QWheelEvent *event );



    public:

        QLabel* lb_coordinates;

		
	protected:
	
		SketchingScene* scene;

	
	
	
};





#endif // SKETCHINGCANVAS_H
