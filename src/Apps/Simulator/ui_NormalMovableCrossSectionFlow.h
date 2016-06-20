/********************************************************************************
** Form generated from reading UI file 'NormalMovableCrossSectionFlow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NORMALMOVABLECROSSSECTIONFLOW_H
#define UI_NORMALMOVABLECROSSSECTIONFLOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NormalMovableCrossSectionFlow
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hl_layout;
    QVBoxLayout *vl_layoutedts;
    QHBoxLayout *horizontalLayout;
    QLabel *lb_normalCoordX;
    QLineEdit *edt_normalCoordX;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lb_normalCoordY;
    QLineEdit *edt_normalCoordY;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lb_normalCoordZ;
    QLineEdit *edt_normalCoordZ;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *btn_acceptnormal;

    void setupUi(QWidget *NormalMovableCrossSectionFlow)
    {
        if (NormalMovableCrossSectionFlow->objectName().isEmpty())
            NormalMovableCrossSectionFlow->setObjectName(QStringLiteral("NormalMovableCrossSectionFlow"));
        NormalMovableCrossSectionFlow->resize(307, 117);
        layoutWidget = new QWidget(NormalMovableCrossSectionFlow);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 232, 97));
        hl_layout = new QHBoxLayout(layoutWidget);
        hl_layout->setObjectName(QStringLiteral("hl_layout"));
        hl_layout->setContentsMargins(0, 0, 0, 0);
        vl_layoutedts = new QVBoxLayout();
        vl_layoutedts->setObjectName(QStringLiteral("vl_layoutedts"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lb_normalCoordX = new QLabel(layoutWidget);
        lb_normalCoordX->setObjectName(QStringLiteral("lb_normalCoordX"));

        horizontalLayout->addWidget(lb_normalCoordX);

        edt_normalCoordX = new QLineEdit(layoutWidget);
        edt_normalCoordX->setObjectName(QStringLiteral("edt_normalCoordX"));

        horizontalLayout->addWidget(edt_normalCoordX);


        vl_layoutedts->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lb_normalCoordY = new QLabel(layoutWidget);
        lb_normalCoordY->setObjectName(QStringLiteral("lb_normalCoordY"));

        horizontalLayout_2->addWidget(lb_normalCoordY);

        edt_normalCoordY = new QLineEdit(layoutWidget);
        edt_normalCoordY->setObjectName(QStringLiteral("edt_normalCoordY"));

        horizontalLayout_2->addWidget(edt_normalCoordY);


        vl_layoutedts->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lb_normalCoordZ = new QLabel(layoutWidget);
        lb_normalCoordZ->setObjectName(QStringLiteral("lb_normalCoordZ"));

        horizontalLayout_3->addWidget(lb_normalCoordZ);

        edt_normalCoordZ = new QLineEdit(layoutWidget);
        edt_normalCoordZ->setObjectName(QStringLiteral("edt_normalCoordZ"));

        horizontalLayout_3->addWidget(edt_normalCoordZ);


        vl_layoutedts->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vl_layoutedts->addItem(verticalSpacer);


        hl_layout->addLayout(vl_layoutedts);

        btn_acceptnormal = new QDialogButtonBox(layoutWidget);
        btn_acceptnormal->setObjectName(QStringLiteral("btn_acceptnormal"));
        btn_acceptnormal->setOrientation(Qt::Vertical);
        btn_acceptnormal->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        hl_layout->addWidget(btn_acceptnormal);


        retranslateUi(NormalMovableCrossSectionFlow);

        QMetaObject::connectSlotsByName(NormalMovableCrossSectionFlow);
    } // setupUi

    void retranslateUi(QWidget *NormalMovableCrossSectionFlow)
    {
        NormalMovableCrossSectionFlow->setWindowTitle(QApplication::translate("NormalMovableCrossSectionFlow", "Form", 0));
        lb_normalCoordX->setText(QApplication::translate("NormalMovableCrossSectionFlow", "X", 0));
        lb_normalCoordY->setText(QApplication::translate("NormalMovableCrossSectionFlow", "Y", 0));
        lb_normalCoordZ->setText(QApplication::translate("NormalMovableCrossSectionFlow", "Z", 0));
    } // retranslateUi

};

namespace Ui {
    class NormalMovableCrossSectionFlow: public Ui_NormalMovableCrossSectionFlow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NORMALMOVABLECROSSSECTIONFLOW_H
