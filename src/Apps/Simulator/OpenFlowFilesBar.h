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


#ifndef OPENFLOWFILESBAR_H
#define OPENFLOWFILESBAR_H


#include <string>

#include <QWidget>
#include <QFileDialog>
#include <QString>

#include "ui_openflowfilesbar.h"


class OpenFlowFilesBar : public QWidget,
                         private Ui::OpenFlowFilesBar
{
    Q_OBJECT

    public:

        explicit OpenFlowFilesBar( QWidget *parent = 0 );
        ~OpenFlowFilesBar();


    private slots:

        void on_btn_inputparameters_clicked();
        void on_btn_inputmesh_clicked();

        void on_btb_inputfiles_accepted();
        void on_btb_inputfiles_rejected();


        void on_chk_defaulttolerances_toggled(bool checked);

        void on_chk_meshfile_clicked(bool checked);

signals:

        void emitParametersandMeshFiles( const int& method, bool read,  const std::string& file_mesh, const std::string& file_type, const std::string& input_file );
        void emitParametersFile( const std::string& parameters_file, int option = -1 );
        void emitTolerancesValues( float tol1, float tol2 );
        void closeBar();



};

#endif // OPENFLOWFILESBAR_H
