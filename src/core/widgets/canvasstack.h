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


/**
 *  A customized QDockWidget that emits a signal when is closed.
 *  This widget was created because there is not an appropriate signal to know when it is closed
 *  only signals related to its visibility.
 */

class DockWidget: public QDockWidget
{
    Q_OBJECT

    public:


        /**
        * Constructor.
        */
        DockWidget( const QString &title, QWidget* parent = nullptr ): QDockWidget( title, parent )
        {
            setWindowTitle( title );
        }


        /**
        * Method called when the DockWidget is closed.
        * @param event the event that happens when the DockWidget is closed
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        inline void closeEvent( QCloseEvent *event )
        {
            emit closeDockWidget();
        }

    signals:


        /**
        * Signal emmited to notify others widgets that the DockWidget was closed.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void closeDockWidget();
};


/**
 *  A container of DockWidget
 *  @see DockWidget
 *  @see Container
 */
using CanvasContainer = Container< double, DockWidget* >;


/**
 *  A holder and manager to the CanvasContainer
 *  It was implemented to hold the fixed cross-sections canvas
 *  @see DockWidget
 *  @see Container
 *  @see CanvasContainer
 */

class CanvasStack: public QWidget, public CanvasContainer
{
    Q_OBJECT

    public:


        /**
        * Constructor.
        */
        CanvasStack( QWidget* parent = nullptr );


        /**
        * Method to set the inital setup
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void initialize();


        /**
        * Method to add a canvas to the container
        * @param double the index of the canvas
        * @param canvas_ new canvas to be added
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addElement( double, QGraphicsView* canvas_ );


        /**
        * Method to get a canvas from the container
        * @param double the index of the canvas
        * @see testMeToo()
        * @see publicVar()
        * @return QGraphicsView* A pointer to the canvas which index is id_
        */
        QGraphicsView* getElement( double id_ );


        /**
        * Method to remove a canvas from the container
        * @param double the index of the canvas
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeElement( double id_ );


        /**
        * Method to set a canvas as the current
        * @param id_ the index of the canvas
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setCurrent( double id_ );


        /**
        * Method to return the current canvas
        * @see testMeToo()
        * @see publicVar()
        * @return QGraphicsView* A pointer to the current canvas
        */
        QGraphicsView* getCurrent();


        /**
        * Method to remove all canvas from the container
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void clear();


    signals:


        /**
        * Signal emmited to notify the main application that a canvas was closed.
        * @param id_ the index of the canvas
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void closeSubWindow( double id_ );


        //TODO: Change the name of the signal
        /**
        * Signal emmited to notify the main application that the window was closed.
        * @param id_ the index of the canvas
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void canvasClosed();


    protected:


        /**
        * Method called when the window is closed.
        * @param event the event that happens when the window is closed
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void closeEvent(QCloseEvent *event);


    protected:

        double current;                                                             /**< The index of the current canvas  */

        QSplitter* hb_mainlayout;                                                   /**< A resizable widget to hold all the canvas added to the container */

};

#endif // CANVASSTACK_H
