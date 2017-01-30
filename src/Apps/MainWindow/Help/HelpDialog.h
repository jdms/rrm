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


#ifndef HELPDIALOG_H
#define HELPDIALOG_H


#include <iostream>

#include <QDialog>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QHBoxLayout>

//#include <QImage>
//#include <QImageReader>
//#include <QTextStream>

//#include <QtWebKitWidgets/QWebView>
//#include <QUrl>

#include "ui_HelpDialog.h"

class HelpDialog : public QDialog,
        private Ui::HelpDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = 0);
    ~HelpDialog();

    inline void setCurrentDirectory( std::string current_dir ){ current_directory.clear(); current_directory = current_dir; }

private slots:
    void on_lw_userguide_clicked(const QModelIndex &index);
    void on_lw_contentsitems_clicked(const QModelIndex &index);

private:

    std::string current_directory;
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
};

#endif // HELPDIALOG_H
