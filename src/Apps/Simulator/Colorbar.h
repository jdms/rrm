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


#ifndef COLORBAR_H
#define COLORBAR_H

#include <QWidget>
#include <QPainter>
#include <QVector3D>
#include <iostream>

class ColorBar: public QWidget
{
	Q_OBJECT

    public:

        ColorBar();
        void setSize( const int& l, const int& w );
        void updateColorMap(const std::vector < QVector3D >& c, float _min, float _max, int label_step = 10);
        void updateColorMap( const std::vector < QVector3D >& c, const std::vector< int >& ids_, const std::vector< double >& values_ );
        void clear();

    protected:

        void paintEvent(QPaintEvent*);

        std::vector< QVector3D > colors;
        std::vector< int > ids;
        std::vector< double > values;

	private:
        int x;
        int y;
        int width;
        int length;
        float min;
        float max;
		float label_step;

        bool is_gradient;
};


#endif // COLORBAR_H
