/********************************************************************************
** Form generated from reading UI file 'OpenFlowFilesBar.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENFLOWFILESBAR_H
#define UI_OPENFLOWFILESBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OpenFlowFilesBar
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *vl_mainlayout;
    QGridLayout *gl_inputfiles;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lb_inputmesh;
    QCheckBox *chk_meshfile;
    QLabel *lb_tolerance;
    QLineEdit *edt_inputparameters;
    QPushButton *btn_inputmesh;
    QFrame *ln_inputfiles_2;
    QLabel *lb_inputparameters;
    QLineEdit *edt_inputmesh;
    QPushButton *btn_inputparameters;
    QFrame *ln_inputfiles;
    QHBoxLayout *horizontalLayout;
    QLineEdit *edt_tolerance1;
    QLineEdit *edt_tolerance2;
    QCheckBox *chk_defaulttolerances;
    QFrame *line;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *rd_cornerpoint;
    QRadioButton *rd_unstructuredmesh;
    QHBoxLayout *horizontalLayout_4;
    QDialogButtonBox *btb_inputfiles;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *OpenFlowFilesBar)
    {
        if (OpenFlowFilesBar->objectName().isEmpty())
            OpenFlowFilesBar->setObjectName(QStringLiteral("OpenFlowFilesBar"));
        OpenFlowFilesBar->resize(698, 621);
        layoutWidget = new QWidget(OpenFlowFilesBar);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 609, 261));
        vl_mainlayout = new QVBoxLayout(layoutWidget);
        vl_mainlayout->setObjectName(QStringLiteral("vl_mainlayout"));
        vl_mainlayout->setContentsMargins(0, 0, 0, 0);
        gl_inputfiles = new QGridLayout();
        gl_inputfiles->setObjectName(QStringLiteral("gl_inputfiles"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lb_inputmesh = new QLabel(layoutWidget);
        lb_inputmesh->setObjectName(QStringLiteral("lb_inputmesh"));

        horizontalLayout_3->addWidget(lb_inputmesh);

        chk_meshfile = new QCheckBox(layoutWidget);
        chk_meshfile->setObjectName(QStringLiteral("chk_meshfile"));
        chk_meshfile->setChecked(false);

        horizontalLayout_3->addWidget(chk_meshfile);


        gl_inputfiles->addLayout(horizontalLayout_3, 1, 1, 1, 1);

        lb_tolerance = new QLabel(layoutWidget);
        lb_tolerance->setObjectName(QStringLiteral("lb_tolerance"));

        gl_inputfiles->addWidget(lb_tolerance, 3, 1, 1, 1);

        edt_inputparameters = new QLineEdit(layoutWidget);
        edt_inputparameters->setObjectName(QStringLiteral("edt_inputparameters"));

        gl_inputfiles->addWidget(edt_inputparameters, 0, 2, 1, 1);

        btn_inputmesh = new QPushButton(layoutWidget);
        btn_inputmesh->setObjectName(QStringLiteral("btn_inputmesh"));

        gl_inputfiles->addWidget(btn_inputmesh, 1, 3, 1, 1);

        ln_inputfiles_2 = new QFrame(layoutWidget);
        ln_inputfiles_2->setObjectName(QStringLiteral("ln_inputfiles_2"));
        ln_inputfiles_2->setFrameShape(QFrame::HLine);
        ln_inputfiles_2->setFrameShadow(QFrame::Sunken);

        gl_inputfiles->addWidget(ln_inputfiles_2, 2, 1, 1, 3);

        lb_inputparameters = new QLabel(layoutWidget);
        lb_inputparameters->setObjectName(QStringLiteral("lb_inputparameters"));

        gl_inputfiles->addWidget(lb_inputparameters, 0, 1, 1, 1);

        edt_inputmesh = new QLineEdit(layoutWidget);
        edt_inputmesh->setObjectName(QStringLiteral("edt_inputmesh"));

        gl_inputfiles->addWidget(edt_inputmesh, 1, 2, 1, 1);

        btn_inputparameters = new QPushButton(layoutWidget);
        btn_inputparameters->setObjectName(QStringLiteral("btn_inputparameters"));

        gl_inputfiles->addWidget(btn_inputparameters, 0, 3, 1, 1);

        ln_inputfiles = new QFrame(layoutWidget);
        ln_inputfiles->setObjectName(QStringLiteral("ln_inputfiles"));
        ln_inputfiles->setFrameShape(QFrame::HLine);
        ln_inputfiles->setFrameShadow(QFrame::Sunken);

        gl_inputfiles->addWidget(ln_inputfiles, 4, 1, 1, 4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        edt_tolerance1 = new QLineEdit(layoutWidget);
        edt_tolerance1->setObjectName(QStringLiteral("edt_tolerance1"));

        horizontalLayout->addWidget(edt_tolerance1);

        edt_tolerance2 = new QLineEdit(layoutWidget);
        edt_tolerance2->setObjectName(QStringLiteral("edt_tolerance2"));

        horizontalLayout->addWidget(edt_tolerance2);

        chk_defaulttolerances = new QCheckBox(layoutWidget);
        chk_defaulttolerances->setObjectName(QStringLiteral("chk_defaulttolerances"));
        chk_defaulttolerances->setChecked(true);

        horizontalLayout->addWidget(chk_defaulttolerances);

        line = new QFrame(layoutWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        rd_cornerpoint = new QRadioButton(layoutWidget);
        rd_cornerpoint->setObjectName(QStringLiteral("rd_cornerpoint"));

        horizontalLayout_2->addWidget(rd_cornerpoint);

        rd_unstructuredmesh = new QRadioButton(layoutWidget);
        rd_unstructuredmesh->setObjectName(QStringLiteral("rd_unstructuredmesh"));
        rd_unstructuredmesh->setChecked(true);

        horizontalLayout_2->addWidget(rd_unstructuredmesh);


        horizontalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));

        horizontalLayout->addLayout(horizontalLayout_4);


        gl_inputfiles->addLayout(horizontalLayout, 3, 2, 1, 2);


        vl_mainlayout->addLayout(gl_inputfiles);

        btb_inputfiles = new QDialogButtonBox(layoutWidget);
        btb_inputfiles->setObjectName(QStringLiteral("btb_inputfiles"));
        btb_inputfiles->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Open);

        vl_mainlayout->addWidget(btb_inputfiles);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vl_mainlayout->addItem(verticalSpacer);


        retranslateUi(OpenFlowFilesBar);

        QMetaObject::connectSlotsByName(OpenFlowFilesBar);
    } // setupUi

    void retranslateUi(QWidget *OpenFlowFilesBar)
    {
        OpenFlowFilesBar->setWindowTitle(QApplication::translate("OpenFlowFilesBar", "Form", 0));
        lb_inputmesh->setText(QApplication::translate("OpenFlowFilesBar", "Mesh File", 0));
        chk_meshfile->setText(QString());
        lb_tolerance->setText(QApplication::translate("OpenFlowFilesBar", "Tolerance", 0));
        btn_inputmesh->setText(QString());
        lb_inputparameters->setText(QApplication::translate("OpenFlowFilesBar", "Parameters File", 0));
        btn_inputparameters->setText(QString());
        chk_defaulttolerances->setText(QApplication::translate("OpenFlowFilesBar", "Default", 0));
        rd_cornerpoint->setText(QApplication::translate("OpenFlowFilesBar", "Corner Point ", 0));
        rd_unstructuredmesh->setText(QApplication::translate("OpenFlowFilesBar", "Unstructured Mesh", 0));
    } // retranslateUi

};

namespace Ui {
    class OpenFlowFilesBar: public Ui_OpenFlowFilesBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENFLOWFILESBAR_H
