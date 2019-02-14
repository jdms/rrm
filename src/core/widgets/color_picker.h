/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file color_picker.h
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class ColorPicker
 */



#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QtWidgets/QToolButton>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QMenuBar>
#include <QPixmap>


/**
 *  A customized QToolButton that holds a color picker
 *  This widget was created to save space in the toolbars of the application
 */

class ColorPicker: public QToolButton
{
    Q_OBJECT

    public:


        /**
        * Constructor. parent_ is passed to QToolButton's constructor.
        * @param parent_ the parent item
        * @see QWidget
        */
        ColorPicker( QWidget* parent = nullptr );


        /**
        * Method to update the color picker with a new color
        * @param c new color
        * @see QColor
        * @return void.
        */
        void setColor( const QColor& c );


        /**
        * Method to return the current color from the color picker
        * @see QColor
        * @return QColor the current color of the color picker.
        */
        QColor currentColor() const;



    public slots:


        /**
        * This method set the current color as a random color or the current one of the color picker.
        * The chosen color depends on the current status of this widget
        * @return void.
        */

        void defineRandomColor();


    protected:


        /**
        * Method to create the interface of the widget
        * @return void.
        */
        void createWidget();


        /**
        * Method to create and define the actions and connects of this widget
        * @see QActions
        * @return void.
        */
        void createActions();


        /**
        * Method to generate a random color.
        * @see QColor
        * @return QColor a random color.
        */
        QColor randomColor();


    protected slots:


        /**
        * Method called when the current color of the color picker is changed
        * @param color_ the new color
        * @see QColor
        * @return void.
        */
        void colorChanged( const QColor& color_ );


    signals:


        /**
        * Signal emmited when a new color is selected in the color picker
        * @param QColor the new color
        * @see QColor
        * @return void.
        */
        void colorSelected( const QColor& );


    private:

        QColorDialog* cd_picker_color;                                          /**< A color picker */

        QWidgetAction* wa_picker_color;                                         /**< An action that holds the color picker */

        QMenu* mn_picker_color;                                                 /**< The menu that contains the action */

};

#endif // COLORPICKER_H
