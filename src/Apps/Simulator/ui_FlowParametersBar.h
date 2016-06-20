/********************************************************************************
** Form generated from reading UI file 'FlowParametersBar.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLOWPARAMETERSBAR_H
#define UI_FLOWPARAMETERSBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FlowParametersBar
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *vl_mainlayout;
    QGroupBox *gb_parameters;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpinBox *sp_nopermeabilityvalues;
    QPushButton *btn_permeabilityvalues;
    QHBoxLayout *horizontalLayout_5;
    QSpinBox *sp_nowells;
    QPushButton *btn_wellsvalues;
    QHBoxLayout *horizontalLayout_7;
    QSpinBox *sp_notfboundaryvalues;
    QPushButton *btn_tfboundaryvalues;
    QHBoxLayout *horizontalLayout_8;
    QSpinBox *sp_notrboundaryvalues;
    QPushButton *btn_trboundaryvalues;
    QLabel *lb_wells;
    QLabel *lb_boundaries;
    QLabel *lb_permeability;
    QLabel *lb_trboundary;
    QLabel *lb_tetgencmd;
    QLabel *lb_tolerance1;
    QLabel *lb_tfboundary;
    QDoubleSpinBox *dsp_tolerance2;
    QLineEdit *edt_tetgencommand;
    QDoubleSpinBox *dsp_tolerance1;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QSpinBox *sp_noboundaries;
    QPushButton *btn_boundariesvalues;
    QFrame *ln_sidebar;
    QDialogButtonBox *btb_acceptparameters;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *FlowParametersBar)
    {
        if (FlowParametersBar->objectName().isEmpty())
            FlowParametersBar->setObjectName(QStringLiteral("FlowParametersBar"));
        FlowParametersBar->resize(460, 639);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FlowParametersBar->sizePolicy().hasHeightForWidth());
        FlowParametersBar->setSizePolicy(sizePolicy);
        layoutWidget = new QWidget(FlowParametersBar);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 0, 427, 590));
        vl_mainlayout = new QVBoxLayout(layoutWidget);
        vl_mainlayout->setObjectName(QStringLiteral("vl_mainlayout"));
        vl_mainlayout->setContentsMargins(0, 0, 0, 0);
        gb_parameters = new QGroupBox(layoutWidget);
        gb_parameters->setObjectName(QStringLiteral("gb_parameters"));
        gb_parameters->setFlat(true);
        verticalLayout_2 = new QVBoxLayout(gb_parameters);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        sp_nopermeabilityvalues = new QSpinBox(gb_parameters);
        sp_nopermeabilityvalues->setObjectName(QStringLiteral("sp_nopermeabilityvalues"));

        horizontalLayout->addWidget(sp_nopermeabilityvalues);

        btn_permeabilityvalues = new QPushButton(gb_parameters);
        btn_permeabilityvalues->setObjectName(QStringLiteral("btn_permeabilityvalues"));
        btn_permeabilityvalues->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(btn_permeabilityvalues);


        gridLayout->addLayout(horizontalLayout, 3, 2, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        sp_nowells = new QSpinBox(gb_parameters);
        sp_nowells->setObjectName(QStringLiteral("sp_nowells"));

        horizontalLayout_5->addWidget(sp_nowells);

        btn_wellsvalues = new QPushButton(gb_parameters);
        btn_wellsvalues->setObjectName(QStringLiteral("btn_wellsvalues"));
        btn_wellsvalues->setMaximumSize(QSize(30, 30));

        horizontalLayout_5->addWidget(btn_wellsvalues);


        gridLayout->addLayout(horizontalLayout_5, 5, 2, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        sp_notfboundaryvalues = new QSpinBox(gb_parameters);
        sp_notfboundaryvalues->setObjectName(QStringLiteral("sp_notfboundaryvalues"));

        horizontalLayout_7->addWidget(sp_notfboundaryvalues);

        btn_tfboundaryvalues = new QPushButton(gb_parameters);
        btn_tfboundaryvalues->setObjectName(QStringLiteral("btn_tfboundaryvalues"));
        btn_tfboundaryvalues->setMaximumSize(QSize(30, 30));

        horizontalLayout_7->addWidget(btn_tfboundaryvalues);


        gridLayout->addLayout(horizontalLayout_7, 6, 2, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        sp_notrboundaryvalues = new QSpinBox(gb_parameters);
        sp_notrboundaryvalues->setObjectName(QStringLiteral("sp_notrboundaryvalues"));

        horizontalLayout_8->addWidget(sp_notrboundaryvalues);

        btn_trboundaryvalues = new QPushButton(gb_parameters);
        btn_trboundaryvalues->setObjectName(QStringLiteral("btn_trboundaryvalues"));
        btn_trboundaryvalues->setMaximumSize(QSize(30, 30));

        horizontalLayout_8->addWidget(btn_trboundaryvalues);


        gridLayout->addLayout(horizontalLayout_8, 7, 2, 1, 1);

        lb_wells = new QLabel(gb_parameters);
        lb_wells->setObjectName(QStringLiteral("lb_wells"));

        gridLayout->addWidget(lb_wells, 5, 1, 1, 1);

        lb_boundaries = new QLabel(gb_parameters);
        lb_boundaries->setObjectName(QStringLiteral("lb_boundaries"));

        gridLayout->addWidget(lb_boundaries, 4, 1, 1, 1);

        lb_permeability = new QLabel(gb_parameters);
        lb_permeability->setObjectName(QStringLiteral("lb_permeability"));

        gridLayout->addWidget(lb_permeability, 3, 1, 1, 1);

        lb_trboundary = new QLabel(gb_parameters);
        lb_trboundary->setObjectName(QStringLiteral("lb_trboundary"));

        gridLayout->addWidget(lb_trboundary, 7, 1, 1, 1);

        lb_tetgencmd = new QLabel(gb_parameters);
        lb_tetgencmd->setObjectName(QStringLiteral("lb_tetgencmd"));

        gridLayout->addWidget(lb_tetgencmd, 1, 1, 1, 1);

        lb_tolerance1 = new QLabel(gb_parameters);
        lb_tolerance1->setObjectName(QStringLiteral("lb_tolerance1"));

        gridLayout->addWidget(lb_tolerance1, 0, 1, 1, 1);

        lb_tfboundary = new QLabel(gb_parameters);
        lb_tfboundary->setObjectName(QStringLiteral("lb_tfboundary"));

        gridLayout->addWidget(lb_tfboundary, 6, 1, 1, 1);

        dsp_tolerance2 = new QDoubleSpinBox(gb_parameters);
        dsp_tolerance2->setObjectName(QStringLiteral("dsp_tolerance2"));

        gridLayout->addWidget(dsp_tolerance2, 0, 3, 1, 1);

        edt_tetgencommand = new QLineEdit(gb_parameters);
        edt_tetgencommand->setObjectName(QStringLiteral("edt_tetgencommand"));

        gridLayout->addWidget(edt_tetgencommand, 1, 2, 1, 2);

        dsp_tolerance1 = new QDoubleSpinBox(gb_parameters);
        dsp_tolerance1->setObjectName(QStringLiteral("dsp_tolerance1"));

        gridLayout->addWidget(dsp_tolerance1, 0, 2, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        sp_noboundaries = new QSpinBox(gb_parameters);
        sp_noboundaries->setObjectName(QStringLiteral("sp_noboundaries"));

        horizontalLayout_3->addWidget(sp_noboundaries);

        btn_boundariesvalues = new QPushButton(gb_parameters);
        btn_boundariesvalues->setObjectName(QStringLiteral("btn_boundariesvalues"));
        btn_boundariesvalues->setMaximumSize(QSize(30, 30));

        horizontalLayout_3->addWidget(btn_boundariesvalues);


        horizontalLayout_2->addLayout(horizontalLayout_3);


        gridLayout->addLayout(horizontalLayout_2, 4, 2, 1, 1);


        verticalLayout_2->addLayout(gridLayout);


        vl_mainlayout->addWidget(gb_parameters);

        ln_sidebar = new QFrame(layoutWidget);
        ln_sidebar->setObjectName(QStringLiteral("ln_sidebar"));
        ln_sidebar->setFrameShape(QFrame::HLine);
        ln_sidebar->setFrameShadow(QFrame::Sunken);

        vl_mainlayout->addWidget(ln_sidebar);

        btb_acceptparameters = new QDialogButtonBox(layoutWidget);
        btb_acceptparameters->setObjectName(QStringLiteral("btb_acceptparameters"));
        btb_acceptparameters->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        vl_mainlayout->addWidget(btb_acceptparameters);

        verticalSpacer_2 = new QSpacerItem(425, 218, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vl_mainlayout->addItem(verticalSpacer_2);


        retranslateUi(FlowParametersBar);

        QMetaObject::connectSlotsByName(FlowParametersBar);
    } // setupUi

    void retranslateUi(QWidget *FlowParametersBar)
    {
        FlowParametersBar->setWindowTitle(QApplication::translate("FlowParametersBar", "Form", 0));
        gb_parameters->setTitle(QApplication::translate("FlowParametersBar", "Parameters  ", 0));
        btn_permeabilityvalues->setText(QString());
        btn_wellsvalues->setText(QString());
        btn_tfboundaryvalues->setText(QString());
        btn_trboundaryvalues->setText(QString());
        lb_wells->setText(QApplication::translate("FlowParametersBar", "Wells", 0));
        lb_boundaries->setText(QApplication::translate("FlowParametersBar", "Surface Boundaries", 0));
        lb_permeability->setText(QApplication::translate("FlowParametersBar", "Properties Area", 0));
        lb_trboundary->setText(QApplication::translate("FlowParametersBar", "Tracer Boundary", 0));
        lb_tetgencmd->setText(QApplication::translate("FlowParametersBar", "Command", 0));
        lb_tolerance1->setText(QApplication::translate("FlowParametersBar", "Tolerance", 0));
        lb_tfboundary->setText(QApplication::translate("FlowParametersBar", "TOF Boundary", 0));
        btn_boundariesvalues->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FlowParametersBar: public Ui_FlowParametersBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLOWPARAMETERSBAR_H
