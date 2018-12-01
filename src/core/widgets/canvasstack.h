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


//using CanvasContainer = Container< double, QDockWidget* >;


class DockWidget: public QDockWidget
{
    Q_OBJECT

    public:

        DockWidget( const QString &title, QWidget* parent = nullptr ): QDockWidget( title, parent )
        {
            setWindowTitle( title );
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
