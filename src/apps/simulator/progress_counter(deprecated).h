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


#ifndef PROGRESSCOUNTER_H
#define PROGRESSCOUNTER_H

#include <QObject>

class ProgressCounter : public QObject
{
    Q_OBJECT

    public:

        explicit ProgressCounter( QObject *parent = 0 ){}

        inline void start( const unsigned int& min, const unsigned int& max ){ emit startProcess( min, max ); }
        inline void update( const unsigned int& value ){ emit updateProcess( value ); }


    signals:

        void startProcess( const unsigned int& min, const unsigned int& max );
        void updateProcess( const unsigned int& value );




};

#endif // PROGRESSCOUNTER_H
