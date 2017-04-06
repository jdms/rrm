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

#include "HelpDialog.h"
#include "ui_HelpDialog.h"

#include <iostream>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setWindowTitle("Contents");

    tw_guide->setMaximumWidth( 300 );

    QHBoxLayout *hb_generalcontents  = new QHBoxLayout();
    hb_generalcontents->addWidget( lw_contentsitems );
    tb_generalguide->setLayout( hb_generalcontents );


    QHBoxLayout *hb_specificcontents  = new QHBoxLayout();
    hb_specificcontents->addWidget( lw_userguide );
    tb_specificguide->setLayout( hb_specificcontents );

    QHBoxLayout *hb_contents = new QHBoxLayout();
    hb_contents->addWidget( tw_guide );
    hb_contents->addWidget( gv_contents );


    setLayout( hb_contents );


}


HelpDialog::~HelpDialog()
{
//    delete ui;
}

void HelpDialog::on_lw_userguide_clicked(const QModelIndex &index)
{


    scene.clear();

    if( index.row() == 0 )
    {

        QImage image( QString(  QString(current_directory.c_str()) + "manual/ch1.How to sketch and get the surfaces from sketch.png" )  );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
    if( index.row() == 1 )
    {
        QImage image(  QString(  QString(current_directory.c_str()) + "manual/ch2.How to get the surfaces from file_DPR.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
    if( index.row() == 2 )
    {
        QImage image(  QString(  QString(current_directory.c_str()) + "manual/ch3.How to edit the user input parameters_DPR_Page_1_Page_1.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
    if( index.row() == 3 )
    {
        QImage image(  QString(  QString(current_directory.c_str()) + "manual/ch3.How to edit the user input parameters_DPR_Page_1_Page_2.png"  ));

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
    if( index.row() == 4 )
    {
        QImage image(  QString(  QString(current_directory.c_str()) + "manual/ch4.User input form_DPR_Page_1_Page_1.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
    if( index.row() == 5 )
    {
        QImage image(  QString(  QString(current_directory.c_str()) + "manual/ch4.User input form_DPR_Page_1_Page_2.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
    if( index.row() == 6 )
    {
        QImage image(  QString(  QString(current_directory.c_str()) + "manual/ch4.User input form_DPR_Page_1_Page_3.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
    if( index.row() == 7 )
    {
        QImage image(  QString(  QString(current_directory.c_str()) + "manual/ch4.User input form_DPR_Page_1_Page_4.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }


    if( index.row() == 8 )
    {
        QImage image(  QString(  QString(current_directory.c_str()) + "manual/ch5.Mesh Generation_DPR.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }


    if( index.row() == 9 )
    {
        QImage image(  QString(  QString(current_directory.c_str()) + "manual/ch6.Flow Diagnostics_DPR.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }


    if( index.row() == 10 )
    {
        QImage image(  QString(  QString(current_directory.c_str()) + "manual/ch7.Pore Volume.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }


    if( index.row() == 11 )
    {
        QImage image( QString(  QString(current_directory.c_str()) + "manual/ch8.CrossSection feature_DPR.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }


    if( index.row() == 12 )
    {
        QImage image( QString(  QString(current_directory.c_str()) + "manual/ch9.Export  options_DPR.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }


    gv_contents->setScene( &scene );
    gv_contents->show();

}



void HelpDialog::on_lw_contentsitems_clicked(const QModelIndex &index)
{


    scene.clear();

    if( index.row() == 0 )
    {///*Quick Manual to Sketching View Interface_DPR_Page_1.png*/
        QImage image( QString(  QString(current_directory.c_str()) + "manual/Sketch_ReadMe-NoSVG-1 (dragged).png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
    if( index.row() == 1 )
    {
        QImage image( QString(  QString(current_directory.c_str()) + "manual/Sketch_ReadMe-NoSVG-2 (dragged).png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
    if( index.row() == 2 )
    {
        QImage image( QString(  QString(current_directory.c_str()) + "manual/Sketch_ReadMe-NoSVG-3 (dragged).png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
      if( index.row() == 3 )
    {
        QImage image( QString(  QString(current_directory.c_str()) + "manual/Quick Manual to Flow Diagnostic View Interface_DPR_Page_1.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
    if( index.row() == 4 )
    {
        QImage image( QString(  QString(current_directory.c_str()) + "manual/Quick Manual to Flow Diagnostic View Interface_DPR_Page_2.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
    if( index.row() == 5 )
    {
        QImage image( QString(  QString(current_directory.c_str()) + "manual/Quick Manual to Flow Diagnostic View Interface_DPR_Page_3.png" ) );

        if(image.isNull())
        {
            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
            return;
        }

        scene.addPixmap( QPixmap::fromImage( image ) );
    }
//    if( index.row() == 6 )
//    {
//        QImage image( QString(  QString(current_directory.c_str()) + "manual/Quick Manual to Flow Diagnostic View Interface_DPR_Page_4.png" ) );

//        if(image.isNull())
//        {
//            std::cout << "Image Viewer, Error Displaying image" << std::endl;;
//            return;
//        }

//        scene.addPixmap( QPixmap::fromImage( image ) );
//    }


    gv_contents->setScene( &scene );
    gv_contents->show();

}
