#include "OpenFlowFilesBar.h"
#include "ui_OpenFlowFilesBar.h"

OpenFlowFilesBar::OpenFlowFilesBar(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    this->setLayout( vl_mainlayout );


    QDir app_dir = QDir( qApp->applicationDirPath() );
    

#if defined(_WIN32) || defined(_WIN64) // Windows Directory Style
    QString current_dir ( app_dir.path ()+"\\" );

#elif defined(__linux__)               // Linux Directory Style
    QString current_dir ( app_dir.path ( ) + "/" );

#else
    /* Error, both can't be defined or undefined same time */
    std::cout << "Operate System not supported !"
    halt();

#endif


    edt_inputparameters->setText( current_dir + "inputs/userinput_simplechannel.txt" );
    edt_inputmesh->setText( current_dir + "inputs/simplechannel.poly" );

    edt_tolerance1->setEnabled( false );
    edt_tolerance2->setEnabled( false );

    chk_defaulttolerances->setChecked( true );


    edt_inputmesh->setEnabled( false );
    btn_inputmesh->setEnabled( false );
}


OpenFlowFilesBar::~OpenFlowFilesBar()
{
}


void OpenFlowFilesBar::on_btn_inputparameters_clicked()
{
    QString selected_format = "";
    QString filename = QFileDialog::getOpenFileName( this, tr( "Open File" ), "./inputs/",
                                                     ".txt files (*.txt)", &selected_format );
    if( filename.isEmpty() == true ) return;

    edt_inputparameters->setText( filename );
}


void OpenFlowFilesBar::on_btn_inputmesh_clicked()
{
    QString selected_format = "";
    QString filename = QFileDialog::getOpenFileName( this, tr( "Open File" ), "./inputs/",
                                                     ".poly files (*.poly);;.txt files (*.txt)", &selected_format );
    if( filename.isEmpty() == true ) return;

    edt_inputmesh->setText( filename );
}


void OpenFlowFilesBar::on_btb_inputfiles_accepted()
{

    std::string parameters_file = edt_inputparameters->text().toStdString();
    int option = -1;

    if( rd_unstructuredmesh->isChecked() == true )
        option = 1;
    else if( rd_cornerpoint->isChecked() == true )
        option = 2;


    if( chk_meshfile->isChecked() == true ){

        if( edt_inputmesh->text().isEmpty() == true || edt_inputparameters->text().isEmpty() == true  )
            return;

        QStringList listofnames = edt_inputmesh->text().split( "\." );
        QString name_of_file = listofnames[ 0 ];
        QString extension_of_file = listofnames[ 1 ];

        std::string mesh_file = name_of_file.toStdString();

        emitParametersandMeshFiles( option, true,  mesh_file, extension_of_file.toStdString(), parameters_file );

//        emitParametersandMeshFiles( parameters_file, mesh_file, extension_of_file, option );

    }

    else
    {
        if( edt_inputparameters->text().isEmpty() == true  )
            return;

        emitParametersandMeshFiles( option, false,  "", "", parameters_file );
    }


    if( chk_defaulttolerances->isChecked() == false )
    {
        float tolerance1 = edt_tolerance1->text().toFloat();
        float tolerance2 = edt_tolerance2->text().toFloat();

        emitTolerancesValues( tolerance1, tolerance2 );

    }
}


void OpenFlowFilesBar::on_btb_inputfiles_rejected()
{
//    edt_inputmesh->clear();
//    edt_inputparameters->clear();

    closeBar();
}

void OpenFlowFilesBar::on_chk_defaulttolerances_toggled( bool checked )
{
    if( checked == true )
    {
        edt_tolerance1->setEnabled( false );
        edt_tolerance2->setEnabled( false );
    }
    else
    {
        edt_tolerance1->setEnabled( true );
        edt_tolerance2->setEnabled( true );
    }

}

void OpenFlowFilesBar::on_chk_meshfile_clicked( bool checked )
{
    edt_inputmesh->setEnabled( checked );
    btn_inputmesh->setEnabled( checked );

}
