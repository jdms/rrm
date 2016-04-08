#include "MainWindow.h"

// Skecthing
void MainWindow::create2DModule ( )
{
	dc_2DModule = new Sketching2DModule( this );
	dc_2DModule->setWindowTitle ( "Sketching" );
	addDockWidget( Qt::LeftDockWidgetArea, dc_2DModule );

	// XXX Just the Canvas
	this->sketch_board_ = new SketchBoard ( );

	this->sketch_controller_ = new SketchController(this->cross_section__);

	//this->setCentralWidget ( sketch_board_ );

	dc_2DModule->setWidget ( sketch_board_ );

	addDockWidget ( Qt::LeftDockWidgetArea , dc_2DModule );

	this->status_bar_ = new QStatusBar ( this );
	this->status_text = new QLabel ( "Sketch" , this );
	this->status_bar_->addWidget ( status_text );
	this->setStatusBar ( this->status_bar_ );

//	// Sketching
//	connect ( this->sketchSession_ , SIGNAL( curve2DSignal(QPolygonF) ) , this , SLOT( curve2DSlot(QPolygonF) ) );
//
//	connect ( this->sketchSession_ , SIGNAL( smoothSketchSignal(QPolygonF) ) , this , SLOT( smoothCurveSlot(QPolygonF) ) );
//
//	connect ( this->sketchSession_ , SIGNAL( newSessionSignal(QPixmap) ) , this , SLOT( newSessionSlot(QPixmap) ) );
//
//	connect ( this->sketchSession_ , SIGNAL( newSessionSignal(qreal,qreal,qreal,qreal) ) , this , SLOT( newSessionSlot(qreal,qreal,qreal,qreal) ) );
//

}

void MainWindow::create3DModule ( )
{
	dc_3DModule = new QDockWidget ( this );
	dc_3DModule->setAllowedAreas ( Qt::RightDockWidgetArea );
	dc_3DModule->setWindowTitle ( "3D View" );

	glWidget = new GLWidget ( this );

	QFrame *fr = new QFrame ( this );
	fr->setFrameStyle ( QFrame::Box | QFrame::Sunken );

	QVBoxLayout *hb_canvas3d = new QVBoxLayout (  );

	fr->setLayout ( hb_canvas3d );
	dc_3DModule->setWidget ( fr );
	addDockWidget ( Qt::RightDockWidgetArea , dc_3DModule );


        groupBox = new QGroupBox(dc_3DModule);
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

        hb_canvas3d->addWidget(groupBox);

        hb_canvas3d->addWidget ( glWidget );

}

//void MainWindow::newSessionSlot ( qreal x , qreal y , qreal width , qreal height )
//{
//	std::cout << "New Boundary " << std::endl;
//
//	this->sketchSession_->initialization ( x , y , width , height );// THE VIEW
//	this->cross_section_.initialization ( x , y , width , height ); // THE MODEL
//}
//
//void MainWindow::newSessionSlot ( QPixmap pixmap )
//{
//	this->sketchSession_->initialization_with_image ( pixmap );  // THE VIEW
//	this->cross_section_.initialization ( pixmap.rect ( ).x ( ) ,   // THE MODEL
//                                              pixmap.rect ( ).y ( ) ,
//					      pixmap.rect ( ).width ( ) ,
//					      pixmap.rect ( ).height ( ) );
//}


void MainWindow::keyPressEvent ( QKeyEvent *event )
{
	event->ignore();
}

void MainWindow::mousePressEvent ( QMouseEvent* event )
{
	event->ignore();
}

void MainWindow::on_horizontalSlider_curve_valueChanged()
{
	//update3DExtrusion();
}

void MainWindow::on_horizontalSlider_surface_valueChanged()
{
	//update3DExtrusion();
}

void MainWindow::on_horizontalSlider_extrusion_valueChanged()
{
	//update3DExtrusion();
}
