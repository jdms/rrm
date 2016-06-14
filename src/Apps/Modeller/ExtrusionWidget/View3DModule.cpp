#include "View3DModule.hpp"

View3DModule::View3DModule ( QWidget* parent ) : QDockWidget(parent)
{
	this->setWindowTitle ( "3D View" );

	emit setColor ( 0 , 0 , 128 );

	QFrame *fr = new QFrame ( this );
	fr->setFrameStyle ( QFrame::Box | QFrame::Sunken );

	this->hb_canvas3d = new QVBoxLayout (this);

	fr->setLayout ( hb_canvas3d );
	this->setWidget ( fr );

        groupBox = new QGroupBox(this);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(156000, 100));
        //groupBox->setBaseSize(QSize(300, 400));
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        horizontalSlider_curve = new QSlider(groupBox);
        horizontalSlider_curve->setObjectName(QStringLiteral("horizontalSlider_curve"));
        horizontalSlider_curve->setMinimum(1);
        horizontalSlider_curve->setMaximum(10);
        horizontalSlider_curve->setOrientation(Qt::Horizontal);

        formLayout->setWidget(0, QFormLayout::FieldRole, horizontalSlider_curve);

        horizontalSlider_surface = new QSlider(groupBox);
        horizontalSlider_surface->setObjectName(QStringLiteral("horizontalSlider_surface"));
        horizontalSlider_surface->setMinimum(1);
        horizontalSlider_surface->setMaximum(10);
        horizontalSlider_surface->setValue(10);
        horizontalSlider_surface->setOrientation(Qt::Horizontal);

        formLayout->setWidget(1, QFormLayout::FieldRole, horizontalSlider_surface);

        horizontalSlider_extrusion = new QSlider(groupBox);
        horizontalSlider_extrusion->setObjectName(QStringLiteral("horizontalSlider_extrusion"));
        horizontalSlider_extrusion->setMinimum(10);
        horizontalSlider_extrusion->setMaximum(1000);
        horizontalSlider_extrusion->setSingleStep(1);
        horizontalSlider_extrusion->setValue(500);
        horizontalSlider_extrusion->setOrientation(Qt::Horizontal);

        formLayout->setWidget(2, QFormLayout::FieldRole, horizontalSlider_extrusion);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        groupBox->setTitle(QApplication::translate("ControllerView", "Resolutions", 0));
        label->setText(QApplication::translate("ControllerView", "Curve", 0));
        label_2->setText(QApplication::translate("ControllerView", "Surface", 0));
        label_3->setText(QApplication::translate("ControllerView", "Extrusion", 0));

        this->hb_canvas3d->addWidget(this->groupBox);

        groupBox->setVisible(false);

}

void View3DModule::setBlackScreenVisibility( bool _is_visible )
{
	groupBox->setVisible(_is_visible);
}

void View3DModule::emitColor ( int R , int G , int B )
{
	emit setColor ( R , G , B );
}

