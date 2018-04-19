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



#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QtWidgets/QToolButton>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QMenuBar>
#include <QPixmap>


class ColorPicker: public QToolButton
{
    Q_OBJECT

    public:

        ColorPicker( QWidget* parent = 0 );

        void setColor( const QColor& c );
        QColor currentColor() const;


    public slots:

        void defineRandomColor();


    protected:

        void createWidget();
        void createActions();

        QColor randomColor();


    protected slots:

        void colorChanged( const QColor& color_ );



    signals:

        void colorSelected( const QColor& );
        void setFixedColor( bool status, const QColor& );

    private:

        QColorDialog* cd_picker_color;
        QWidgetAction* wa_picker_color;
        QMenu* mn_picker_color;

};

#endif // COLORPICKER_H
