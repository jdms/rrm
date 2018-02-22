#ifndef CANVASSTACK_H
#define CANVASSTACK_H

#include <QWidget>
#include <QMainWindow>
#include <QGraphicsView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDockWidget>
#include <QCloseEvent>

#include <iostream>

#include "./core/models/container.h"


//using CanvasContainer = Container< double, QDockWidget* >;


class DockWidget: public QDockWidget
{
    Q_OBJECT

    public:

        DockWidget( QWidget* parent = nullptr ): QDockWidget( parent )
        {

        }

        inline void closeEvent( QCloseEvent *event )
        {
            std::cout << "Closing dockwidget?" << std::endl << std::flush;
            emit closeDockWidget();
        }

    signals:

        void closeDockWidget();
};

using CanvasContainer = Container< double, DockWidget* >;

class CanvasStack: public QWidget, public CanvasContainer
{
    Q_OBJECT

    public:

        CanvasStack( QWidget* parent = nullptr );

        void initialize();

        void addElement( double, QGraphicsView* canvas_ );
        QGraphicsView* getElement( double id_ );

        void removeElement( double id_ );

        void setCurrent( double id_ );
        QGraphicsView* getCurrent();

        void clear();



    signals:

        void closeSubWindow( double id_ );


    protected:

        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);

    protected:

        double current;
        QSplitter* hb_mainlayout;

};

#endif // CANVASSTACK_H
