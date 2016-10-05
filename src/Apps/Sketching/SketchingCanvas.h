#ifndef SKETCHINGCANVAS_H
#define SKETCHINGCANVAS_H

#include <QtWidgets/QGraphicsView>
#include <QLabel>
#include <QWheelEvent>
#include <QFileDialog>

#include "MainWindow/Scene.h"

class SketchingCanvas: public QGraphicsView
{
	Q_OBJECT
	
	
	public:
	
        SketchingCanvas( QWidget* parent = 0 );
		void setup();



    protected:

        void setBackGround();

        void wheelEvent( QWheelEvent *event );


    public slots:

        void updateCoordinates( float posx, float posy );
        void screenshot();

        inline void insertSketch(){ scene->addCurve(); }
        inline void undoLastSketch(){ scene->undoLastSketch(); }
        inline void updateColor( const QColor& color ){ scene->updateColor( color ); }
        inline void addScene( Scene* const& sc ){ scene = sc; setScene( scene ); }



    signals:

        void newBoundary();





    public:

        QLabel* lb_coordinates;

		
	protected:
	
		Scene* scene;

	
	
	
};





#endif // SKETCHINGCANVAS_H
