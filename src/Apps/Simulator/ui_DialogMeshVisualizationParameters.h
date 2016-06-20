/********************************************************************************
** Form generated from reading UI file 'DialogMeshVisualizationParameters.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGMESHVISUALIZATIONPARAMETERS_H
#define UI_DIALOGMESHVISUALIZATIONPARAMETERS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogMeshVisualizationParameters
{
public:
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *vl_trianglelayout;
    QHBoxLayout *horizontalLayout;
    QLabel *lb_trianglecmd;
    QLineEdit *edt_cmdtriangle;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QRadioButton *rd_numberpartition;
    QRadioButton *rd_lenghtedge;
    QLineEdit *edt_value;
    QFrame *line;
    QDialogButtonBox *buttonBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *DialogMeshVisualizationParameters)
    {
        if (DialogMeshVisualizationParameters->objectName().isEmpty())
            DialogMeshVisualizationParameters->setObjectName(QStringLiteral("DialogMeshVisualizationParameters"));
        DialogMeshVisualizationParameters->resize(507, 441);
        verticalLayoutWidget_2 = new QWidget(DialogMeshVisualizationParameters);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(150, 30, 241, 139));
        vl_trianglelayout = new QVBoxLayout(verticalLayoutWidget_2);
        vl_trianglelayout->setObjectName(QStringLiteral("vl_trianglelayout"));
        vl_trianglelayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lb_trianglecmd = new QLabel(verticalLayoutWidget_2);
        lb_trianglecmd->setObjectName(QStringLiteral("lb_trianglecmd"));

        horizontalLayout->addWidget(lb_trianglecmd);

        edt_cmdtriangle = new QLineEdit(verticalLayoutWidget_2);
        edt_cmdtriangle->setObjectName(QStringLiteral("edt_cmdtriangle"));

        horizontalLayout->addWidget(edt_cmdtriangle);


        vl_trianglelayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        rd_numberpartition = new QRadioButton(verticalLayoutWidget_2);
        rd_numberpartition->setObjectName(QStringLiteral("rd_numberpartition"));

        verticalLayout->addWidget(rd_numberpartition);

        rd_lenghtedge = new QRadioButton(verticalLayoutWidget_2);
        rd_lenghtedge->setObjectName(QStringLiteral("rd_lenghtedge"));

        verticalLayout->addWidget(rd_lenghtedge);


        horizontalLayout_2->addLayout(verticalLayout);

        edt_value = new QLineEdit(verticalLayoutWidget_2);
        edt_value->setObjectName(QStringLiteral("edt_value"));

        horizontalLayout_2->addWidget(edt_value);


        vl_trianglelayout->addLayout(horizontalLayout_2);

        line = new QFrame(verticalLayoutWidget_2);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        vl_trianglelayout->addWidget(line);

        buttonBox = new QDialogButtonBox(verticalLayoutWidget_2);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        vl_trianglelayout->addWidget(buttonBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vl_trianglelayout->addItem(verticalSpacer);


        retranslateUi(DialogMeshVisualizationParameters);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogMeshVisualizationParameters, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogMeshVisualizationParameters, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogMeshVisualizationParameters);
    } // setupUi

    void retranslateUi(QDialog *DialogMeshVisualizationParameters)
    {
        DialogMeshVisualizationParameters->setWindowTitle(QApplication::translate("DialogMeshVisualizationParameters", "Dialog", 0));
        lb_trianglecmd->setText(QApplication::translate("DialogMeshVisualizationParameters", "Triangle Command", 0));
        rd_numberpartition->setText(QApplication::translate("DialogMeshVisualizationParameters", "Number of Partition", 0));
        rd_lenghtedge->setText(QApplication::translate("DialogMeshVisualizationParameters", "Lenght Edge", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogMeshVisualizationParameters: public Ui_DialogMeshVisualizationParameters {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGMESHVISUALIZATIONPARAMETERS_H
