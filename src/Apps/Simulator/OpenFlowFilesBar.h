#ifndef OPENFLOWFILESBAR_H
#define OPENFLOWFILESBAR_H


#include <string>

#include <QWidget>
#include <QFileDialog>
#include <QString>

#include "ui_OpenFlowFilesBar.h"


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
