#ifndef DIALOGINPUTFLOWCOMPUTATION_H
#define DIALOGINPUTFLOWCOMPUTATION_H

#include <QDialog>
#include <QWidget>
#include <QToolBox>
#include <QFrame>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFileDialog>


/**!
 * @brief Dialog to allow to set flow related  parameters such as mesh triangulation tolerance, etc.
 */

class DialogInputFlowComputation: public QDialog
{
    Q_OBJECT

    public:

        DialogInputFlowComputation( QWidget *parent );

        void getUserInputs( QString& userfile, QString& surfacefile, float& tol1, float& tol2 );

    protected:

        void createActions(QWidget *parent);
        void createDialog();

        void createWidgetInputFiles();
        void createWidgetInputTolerance();

    protected slots:

        void findUserFile();
        void findSurfaceFile();
        void rejectInputUser();

    private:

        QToolBox* tb_inputuser;

        QWidget *wd_inputfiles;
        QWidget *wd_inputtolerance;

        QLineEdit *edt_userfile;
        QPushButton *btn_finduserfile;
        QLineEdit *edt_surfacefile;
        QPushButton *btn_findsurfacefile;
        QLineEdit *edt_tolerance1;
        QLineEdit *edt_tolerance2;

        QDialogButtonBox* btns_inputdialog;


};

#endif // DIALOGINPUTFLOWCOMPUTATION_H
