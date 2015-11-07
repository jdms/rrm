#include "DialogInputFlowComputation.h"

DialogInputFlowComputation::DialogInputFlowComputation( QWidget* parent )
{

    createDialog();
    createActions( parent );
}


void DialogInputFlowComputation::createActions( QWidget* parent  )
{
    connect( btn_finduserfile, SIGNAL( pressed() ), this, SLOT( findUserFile() ));
    connect( btn_findsurfacefile, SIGNAL( pressed() ), this, SLOT( findSurfaceFile() ));
    connect( btns_inputdialog, SIGNAL( accepted() ), parent, SLOT( acceptInputUser() ));
    connect( btns_inputdialog, SIGNAL( rejected() ), this, SLOT( rejectInputUser() ) );
}

void DialogInputFlowComputation::createDialog()
{
    tb_inputuser = new QToolBox();

    createWidgetInputFiles();
    createWidgetInputTolerance();

    tb_inputuser->addItem( wd_inputfiles, tr( "Input Files") );
    tb_inputuser->addItem( wd_inputtolerance, tr( "Tolerance")  );

    btns_inputdialog = new QDialogButtonBox(QDialogButtonBox::Ok
                                           | QDialogButtonBox::Cancel);

    QFrame *ln_inputdialog = new QFrame();
    ln_inputdialog->setFrameStyle(QFrame::HLine | QFrame::Sunken);

    QVBoxLayout *vb_inputdialog = new QVBoxLayout();
    vb_inputdialog->addWidget( tb_inputuser );
    vb_inputdialog->addWidget( ln_inputdialog );
    vb_inputdialog->addWidget( btns_inputdialog );

    this->setLayout( vb_inputdialog );

}


void DialogInputFlowComputation::createWidgetInputFiles()
{
    wd_inputfiles = new QWidget();

    QLabel *lb_userfile = new QLabel( "User input: " );
    edt_userfile = new QLineEdit();
    edt_userfile->setText( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/inputs/userinput_example9.txt");
    btn_finduserfile = new QPushButton();
    btn_finduserfile->setIcon( QIcon( ":/images/icons/folder.png" ) );

    QLabel *lb_surfacefile = new QLabel( "Surface file: " );
    edt_surfacefile = new QLineEdit();
    edt_surfacefile->setText( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/inputs/boxexample9coarse.poly");
    btn_findsurfacefile = new QPushButton();
    btn_findsurfacefile->setIcon( QIcon( ":/images/icons/folder.png" ) );

    QHBoxLayout *hb_userfile = new QHBoxLayout();
    hb_userfile->addWidget( lb_userfile );
    hb_userfile->addWidget( edt_userfile );
    hb_userfile->addWidget( btn_finduserfile );

    QHBoxLayout *hb_surfacefile = new QHBoxLayout();
    hb_surfacefile->addWidget( lb_surfacefile );
    hb_surfacefile->addWidget( edt_surfacefile );
    hb_surfacefile->addWidget( btn_findsurfacefile );

    QVBoxLayout *vb_inputfiles = new QVBoxLayout();
    vb_inputfiles->addLayout( hb_userfile );
    vb_inputfiles->addLayout( hb_surfacefile );

    wd_inputfiles->setLayout( vb_inputfiles );

}


void DialogInputFlowComputation::createWidgetInputTolerance()
{
    wd_inputtolerance = new QWidget();

    QLabel *lb_tolerance = new QLabel( "User input: " );
    edt_tolerance1 = new QLineEdit();
    edt_tolerance1->setText( "0.000000001" );
    edt_tolerance2 = new QLineEdit();
    edt_tolerance2->setText( "0.00001" );

    QHBoxLayout *hb_toleranceinput = new QHBoxLayout();
    hb_toleranceinput->addWidget( lb_tolerance );
    hb_toleranceinput->addWidget( edt_tolerance1 );
    hb_toleranceinput->addWidget( edt_tolerance2 );

    wd_inputtolerance->setLayout( hb_toleranceinput );

}


void DialogInputFlowComputation::findUserFile()
{
    QString filename = QFileDialog::getOpenFileName( this );
    if( filename.isEmpty() == true ) return;

    edt_userfile->setText( filename );
}


void DialogInputFlowComputation::findSurfaceFile()
{
    QString filename = QFileDialog::getOpenFileName( this );
    if( filename.isEmpty() == true ) return;

    edt_surfacefile->setText( filename );


}


void DialogInputFlowComputation::getUserInputs( QString& userfile, QString& surfacefile, float& tol1, float& tol2 )
{
    userfile = edt_userfile->text();
    surfacefile = edt_surfacefile->text();

    tol1 = edt_tolerance1->text().toFloat();
    tol2 = edt_tolerance2->text().toFloat();

    this->close();

}


void DialogInputFlowComputation::rejectInputUser()
{
    edt_userfile->clear();
    edt_surfacefile->clear();
    edt_tolerance1->clear();
    edt_tolerance2->clear();

    this->close();
}
