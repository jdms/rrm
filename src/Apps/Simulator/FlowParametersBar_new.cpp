#include "FlowParametersBar_new.h"


FlowParametersBar_new::FlowParametersBar_new(QWidget *parent) :
    QWidget(parent)
{
    setupUi( this );

    /*this->setLayout( vl_mainlayout );*/

    createDialogs();

}

void FlowParametersBar_new::createDialogs()
{

    //btn_inputparameters->setIcon(QIcon(":/images/icons/folder.png"));
    //btn_regionproperties->setIcon(QIcon(":/images/icons/pencil--plus.png"));
    //btn_wellsvalues->setIcon(QIcon(":/images/icons/pencil--plus.png"));


    //dg_regionsproperties = new FormPropertyValues( FormPropertyValues::PROPERTY_TYPE::REGIONS, this );
    //dg_wells = new FormPropertyValues( FormPropertyValues::PROPERTY_TYPE::WELL, this );


    //connect( sp_noregions, SIGNAL( valueChanged(int) ), dg_regionsproperties, SLOT( reset() ) );
    //connect( sp_nowells, SIGNAL( valueChanged(int) ), dg_wells, SLOT( reset() ) );

	//connect(toolButton_Region_Add, &QToolButton::clicked, this, [=](){ comboBox_Region->addItem(QString("Region" + QString::number(comboBox_Region->count()))); });


	//// REGION
	connect(toolButton_Region_Accept, &QToolButton::clicked, this, &FlowParametersBar_new::createRegions);
	connect(comboBox_Region, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, [=](){ updateRegionWidget(comboBox_Region->currentIndex()); });

	//@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
	connect(doubleSpinBox_Region_Permiability, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](){ perm_values[comboBox_Region->currentIndex()] = doubleSpinBox_Region_Permiability->value(); });
	connect(doubleSpinBox_Region_Porosity, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](){ poros_values[comboBox_Region->currentIndex()] = doubleSpinBox_Region_Porosity->value(); });
	connect(doubleSpinBox_Region_Viscosity, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](){ visc_values[comboBox_Region->currentIndex()] = doubleSpinBox_Region_Viscosity->value(); });
	connect(doubleSpinBox_Region_Point_X, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](){ positions_values[comboBox_Region->currentIndex()][0] = doubleSpinBox_Region_Point_X->value(); });
	connect(doubleSpinBox_Region_Point_Y, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](){ positions_values[comboBox_Region->currentIndex()][1] = doubleSpinBox_Region_Point_Y->value(); });
	connect(doubleSpinBox_Region_Point_Z, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](){ positions_values[comboBox_Region->currentIndex()][2] = doubleSpinBox_Region_Point_Z->value(); });
			
	//// WELL
	connect(toolButton_Well_Accept, &QToolButton::clicked, this, &FlowParametersBar_new::createWells);
	connect(comboBox_Well, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, [=](){ updateWellsWidget(comboBox_Well->currentIndex()); });

	//@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
	connect(doubleSpinBox_Well_Pressure, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](){ well_pressure[comboBox_Well->currentIndex()] = doubleSpinBox_Well_Pressure->value(); });
	connect(spinBox_Well_Type, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](){ well_types[comboBox_Well->currentIndex()] = spinBox_Well_Type->value(); });
	connect(spinBox_Well_Sign, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](){ well_signs[comboBox_Well->currentIndex()] = spinBox_Well_Sign->value(); });


}



void FlowParametersBar_new::setPropertyAreaParameters( const int np, const std::vector< double >& positions , const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc )
{
    //sp_noregions->setValue( np );
    //dg_regionsproperties->createRegionPropertiesForm( np, positions, perm, poros, visc );
}


void FlowParametersBar_new::getPropertyAreaParameters( int& np, std::vector< double >& values , std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc )
{
    //np = dg_regionsproperties->getNumberofRegions();
    //values = dg_regionsproperties->getRegionPositions();
    //perm = dg_regionsproperties->getRegionPermeability();
    //poros = dg_regionsproperties->getRegionPorosity();
    //visc = dg_regionsproperties->getRegionViscosity();
}





void FlowParametersBar_new::setWellParameter( const int nw,  const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign )
{
    //sp_nowells->setValue( nw );
    //dg_wells->createWellForm( nw,  type, value, sign );
}


void FlowParametersBar_new::getWellParameter( int& nw,  std::vector< unsigned int >& type, std::vector< double >& value, std::vector< int >& sign )
{
    //nw = dg_wells->getNumberofWells();
    //type = dg_wells->getWellTypes();
    //value = dg_wells->getWellValues();
    //sign = dg_wells->getWellSigns();
}






void FlowParametersBar_new::on_btn_inputparameters_clicked()
{
    QString selected_format = "";
    QString filename = QFileDialog::getOpenFileName( this, tr( "Open File" ), "./inputs/",
                                                     ".ui files (*.ui)", &selected_format );
    if( filename.isEmpty() == true ) return;

    /*edt_inputparameters->setText( filename );*/
}


void FlowParametersBar_new::on_btn_loadparameterfile_clicked()
{
   /* if( edt_inputparameters->text().isEmpty() == true ) return;*/

  /*  emit readParameterFile( edt_inputparameters->text().toStdString() );*/
}





void FlowParametersBar_new::on_btn_regionproperties_clicked()
{
    //int value_sp = sp_noregions->value();
    //if( value_sp == 0 ) return;

    //int noregions = dg_regionsproperties->getNumberofRegions();
    //if( noregions == 0 )
    //{
    //    dg_regionsproperties->createRegionPropertiesForm( value_sp );
    //}

    //dg_regionsproperties->viewRegionPropertiesForm();
}



void FlowParametersBar_new::on_btn_wellsvalues_clicked()
{
    //int value_sp = sp_nowells->value();
    //if( value_sp == 0 ) return;

    //int nowellsources = dg_wells->getNumberofWells();
    //if( nowellsources == 0 )
    //{
    //    dg_wells->createWellForm( value_sp );
    //}

    //dg_wells->viewWellForm();
}





void FlowParametersBar_new::on_btb_acceptparameters_accepted()
{

    closeBarandAccept();

}


void FlowParametersBar_new::on_btb_acceptparameters_rejected()
{
    closeBar();
}






void FlowParametersBar_new::clear()
{
    //edt_inputparameters->clear();

    //sp_noregions->setValue( 0 );
    //sp_nowells->setValue( 0 );

    //dg_regionsproperties->reset();
    //dg_wells->reset();


}


void FlowParametersBar_new::createRegions( )
{
	QStringList l;
	for (auto i = 0; i < spinBox_Number_of_Regions->value(); i++)
	{
		l.push_back("Region" + QString::number(i + 1));
	}
	comboBox_Region->clear();
	comboBox_Region->insertItems(0, l);
	this->updateRegionWidget(comboBox_Region->currentIndex());
}

void FlowParametersBar_new::updateRegionWidget(const int index)
{
	doubleSpinBox_Region_Permiability->setValue(perm_values[index]);
	doubleSpinBox_Region_Porosity->setValue(poros_values[index]);
	doubleSpinBox_Region_Viscosity->setValue(visc_values[index]);
	doubleSpinBox_Region_Point_X->setValue(positions_values[index].x());
	doubleSpinBox_Region_Point_Y->setValue(positions_values[index].y());
	doubleSpinBox_Region_Point_Z->setValue(positions_values[index].z());
}


void FlowParametersBar_new::set_region_point(double x, double y, double z)
{
	positions_values[comboBox_Region->currentIndex()][0] = x;
	positions_values[comboBox_Region->currentIndex()][1] = y;
	positions_values[comboBox_Region->currentIndex()][2] = z;

	this->updateRegionWidget(comboBox_Region->currentIndex());
}

void FlowParametersBar_new::createWells()
{
	QStringList l;
	for (auto i = 0; i < spinBox_Number_of_Wells->value(); i++)
	{
		l.push_back("Well" + QString::number(i + 1));
	}
	comboBox_Well->clear();
	comboBox_Well->insertItems(0, l);
	this->updateRegionWidget(comboBox_Well->currentIndex());
}

void FlowParametersBar_new::updateWellsWidget(const int index)
{
	doubleSpinBox_Well_Pressure->setValue(well_pressure[index]);
	spinBox_Well_Sign->setValue(well_signs[index]);
	spinBox_Well_Type->setValue(well_types[index]);
	
}

void FlowParametersBar_new::mousePressEvent(QMouseEvent *event)
{
	set_region_point(event->x(), event->y(), 0.0);

}