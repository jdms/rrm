#include "Controller/MainWindow.h"

//void MainWindow::doComputation()
//{
//    addDockWidget( Qt::RightDockWidgetArea, dc_computation );
//    dc_computation->show();
//}
//
//
//
//void MainWindow::clearComputation()
//{
//    canvas_computation->resetSetup();
//    canvas_computation->updateGL();
//
//}
//
//void MainWindow::openSurfaceFile()
//{
////    QString filename = QFileDialog::getOpenFileName( this );
////    if( filename.isEmpty() == true ) return;
//
////    statusBar()->showMessage( "Open surface file" );
//
////    QStringList list = filename.split( "\." );
////    QString name_of_file = list[ 0 ];
////    QString extension_of_file = list[ 1 ];
//
////    emit sendSurfaceFile( name_of_file.toStdString() );
//
////    canvas_computation->showSurface();
//
////    statusBar()->showMessage( "Surface file loaded" );
//}
//
//void MainWindow::openUserInputFile()
//{
//
//    dg_inputuser->show();
//
//    /*
//    statusBar()->showMessage( "Open user input file" );
//
//    QString filename = QFileDialog::getOpenFileName( this );
//    if( filename.isEmpty() == true ) return;
//
//    emit sendInputUserFile( filename.toStdString() );
//
//    statusBar()->showMessage( "User input file loaded" );
//    */
//
//
//}
//
//// ZHAO -----------------------------------------------------
//void MainWindow::createMeshVolumetric()
//{
////    emit computeVolume();
////    canvas_computation->showVolumetricGrid();
////
////    statusBar()->showMessage( "Volumetric meshing done" );
//}
//// ZHAO
//void MainWindow::selectProperty( int id )
//{
////    bool option = false;
////    emit selectFlowProperty( id, option );
////    cb_coloroption_vector->setEnabled( option );
//
//
////    if( option == true ){
////        int option_color = cb_coloroption_vector->currentIndex();
////        canvas_computation->selectProperty( id, option, option_color );
//
////    }
////    else
////        canvas_computation->selectProperty( id, option );
//
//
//}
//
//void MainWindow::computePressure()
//{
//    emit computePressureProperty();
//
//    statusBar()->showMessage( "Pressure computed" );
//}
//
//void MainWindow::computeVelocity()
//{
//
//    emit computeVelocityProperty();
//
//    statusBar()->showMessage( "Velocity computed" );
//}
//
//void MainWindow::computeTOF()
//{
//    emit computeTOFProperty();
//
//    statusBar()->showMessage( "TOF computed" );
//}


/// Widget Start

//void MainWindow::updateComboBox( std::vector< std::string > ppoints, std::vector< std::string > pcells )
//{
//    cb_compute_property->clear();
//
//    int ndata = ppoints.size();
//    for( int i = 0; i < ndata; ++i )
//        cb_compute_property->addItem( ppoints[ i ].c_str() );
//
//    ndata = pcells.size();
//    for( int i = 0; i < ndata; ++i )
//        cb_compute_property->addItem( pcells[ i ].c_str() );
//
//
//}
//
//
//void MainWindow::createInputDialog()
//{
//    dg_inputuser = new QDialog( this );
//    tb_inputuser = new QToolBox();
//
//    createWidgetInputFiles();
//    createWidgetInputTolerance();
//
//    tb_inputuser->addItem( wd_inputfiles, tr( "Input Files") );
//    tb_inputuser->addItem( wd_inputtolerance, tr( "Tolerance")  );
//
//    btns_inputdialog = new QDialogButtonBox(QDialogButtonBox::Ok
//                                           | QDialogButtonBox::Cancel);
//
//    QFrame *ln_inputdialog = new QFrame();
//    ln_inputdialog->setFrameStyle(QFrame::HLine | QFrame::Sunken);
//
//    QVBoxLayout *vb_inputdialog = new QVBoxLayout();
//    vb_inputdialog->addWidget( tb_inputuser );
//    vb_inputdialog->addWidget( ln_inputdialog );
//    vb_inputdialog->addWidget( btns_inputdialog );
//
//    dg_inputuser->setLayout( vb_inputdialog );
//
//    connect( btn_finduserfile, SIGNAL( pressed() ), this, SLOT( findUserFile() ));
//    connect( btn_findsurfacefile, SIGNAL( pressed() ), this, SLOT( findSurfaceFile() ));
//    connect( btns_inputdialog, SIGNAL( accepted() ), this, SLOT( acceptInputUser() ));
//    connect( btns_inputdialog, SIGNAL( rejected() ), this, SLOT( rejectInputUser() ) );
//
//    connect( ac_wdwsketching, SIGNAL( toggled(bool) ), dc_2DModule, SLOT( setVisible(bool) ) );
//    connect( ac_window3d, SIGNAL( toggled(bool) ), dc_3DModule, SLOT( setVisible(bool) ) );
//    connect( ac_flowcomputation, SIGNAL( toggled(bool) ), dc_computation, SLOT( setVisible(bool) ) );
//
//
//}
//
//
//void MainWindow::createWidgetInputFiles()
//{
//    wd_inputfiles = new QWidget();
//
//    QLabel *lb_userfile = new QLabel( "User input: " );
//    edt_userfile = new QLineEdit();
//    btn_finduserfile = new QPushButton();
//
//    QLabel *lb_surfacefile = new QLabel( "Surface file: " );
//    edt_surfacefile = new QLineEdit();
//    btn_findsurfacefile = new QPushButton();
//
//    QHBoxLayout *hb_userfile = new QHBoxLayout();
//    hb_userfile->addWidget( lb_userfile );
//    hb_userfile->addWidget( edt_userfile );
//    hb_userfile->addWidget( btn_finduserfile );
//
//    QHBoxLayout *hb_surfacefile = new QHBoxLayout();
//    hb_surfacefile->addWidget( lb_surfacefile );
//    hb_surfacefile->addWidget( edt_surfacefile );
//    hb_surfacefile->addWidget( btn_findsurfacefile );
//
//    QVBoxLayout *vb_inputfiles = new QVBoxLayout();
//    vb_inputfiles->addLayout( hb_userfile );
//    vb_inputfiles->addLayout( hb_surfacefile );
//
//    wd_inputfiles->setLayout( vb_inputfiles );
//
//}
//
//void MainWindow::createWidgetInputTolerance()
//{
//    wd_inputtolerance = new QWidget();
//
//    QLabel *lb_tolerance = new QLabel( "User input: " );
//    edt_tolerance1 = new QLineEdit();
//    edt_tolerance2 = new QLineEdit();
//
//    QHBoxLayout *hb_toleranceinput = new QHBoxLayout();
//    hb_toleranceinput->addWidget( lb_tolerance );
//    hb_toleranceinput->addWidget( edt_tolerance1 );
//    hb_toleranceinput->addWidget( edt_tolerance2 );
//
//    wd_inputtolerance->setLayout( hb_toleranceinput );
//
//}
//
//void MainWindow::findUserFile()
//{
//    QString filename = QFileDialog::getOpenFileName( this );
//    if( filename.isEmpty() == true ) return;
//
//    edt_userfile->setText( filename );
//}
//
//void MainWindow::findSurfaceFile()
//{
//    QString filename = QFileDialog::getOpenFileName( this );
//    if( filename.isEmpty() == true ) return;
//
//    edt_surfacefile->setText( filename );
//}
//
//void MainWindow::acceptInputUser()
//{
//    QString filename_input = edt_userfile->text();
//    QString filename_surface = edt_surfacefile->text();
//
//    QStringList list = filename_surface.split( "\." );
//    QString name_of_file = list[ 0 ];
//    QString extension_of_file = list[ 1 ];
//
//    float tol1 = edt_tolerance1->text().toFloat();
//    float tol2 = edt_tolerance2->text().toFloat();
//
//    sendInputUser( filename_input.toStdString(), name_of_file.toStdString(), tol1, tol2  );
//    canvas_computation->showSurface();
//
//    statusBar()->showMessage( "Surface loaded" );
//
//    dg_inputuser->close();
//}
//
//void MainWindow::rejectInputUser()
//{
//    edt_userfile->clear();
//    edt_surfacefile->clear();
//    edt_tolerance1->clear();
//    edt_tolerance2->clear();
//
//    dg_inputuser->close();
//}
