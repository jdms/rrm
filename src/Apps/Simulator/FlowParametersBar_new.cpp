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


	/// New GUI ---->
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
	loadRegions(np, positions, perm, poros, visc);

}


void FlowParametersBar_new::getPropertyAreaParameters( int& np, std::vector< double >& values , std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc )
{
    //np = dg_regionsproperties->getNumberofRegions();
    //values = dg_regionsproperties->getRegionPositions();
    //perm = dg_regionsproperties->getRegionPermeability();
    //poros = dg_regionsproperties->getRegionPorosity();
    //visc = dg_regionsproperties->getRegionViscosity();
	getPropertyAreaParameters_new(np, values, perm, poros, visc);

}





void FlowParametersBar_new::setWellParameter( const int nw,  const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign )
{
    //sp_nowells->setValue( nw );
    //dg_wells->createWellForm( nw,  type, value, sign );
	loadWells(nw, type, value, sign);
}


void FlowParametersBar_new::getWellParameter( int& nw,  std::vector< unsigned int >& type, std::vector< double >& value, std::vector< int >& sign )
{
    //nw = dg_wells->getNumberofWells();
    //type = dg_wells->getWellTypes();
    //value = dg_wells->getWellValues();
    //sign = dg_wells->getWellSigns();
	getWellParameter_new(nw, type, value, sign);
}






void FlowParametersBar_new::on_btn_inputparameters_clicked()
{
    QString selected_format = "";
    QString filename = QFileDialog::getOpenFileName( this, tr( "Open File" ), "./inputs/",
                                                     ".ui files (*.ui)", &selected_format );
    if( filename.isEmpty() == true ) return;

    edt_inputparameters->setText( filename );

	emit readParameterFile(edt_inputparameters->text().toStdString());
}


void FlowParametersBar_new::on_btn_loadparameterfile_clicked()
{
   if( edt_inputparameters->text().isEmpty() == true ) return;

   emit readParameterFile( edt_inputparameters->text().toStdString() );
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
	edt_inputparameters->clear();

    //sp_noregions->setValue( 0 );
    //sp_nowells->setValue( 0 );

    //dg_regionsproperties->reset();
    //dg_wells->reset();

	/// New GUI ---->
	new_gui_clear();
}


/// New GUI ---->


void FlowParametersBar_new::mousePressEvent(QMouseEvent *event)
{
	//set_region_point(event->x(), event->y(), 0.0);

}

void FlowParametersBar_new::new_gui_clear()
{
	positions_values.clear();
	perm_values.clear();
	poros_values.clear();
	visc_values.clear();

	well_types.clear();
	well_pressure.clear();
	well_signs.clear();

	comboBox_Region->clear();
	comboBox_Well->clear();
}

/// Region

void FlowParametersBar_new::getPropertyAreaParameters_new(int& np, std::vector< double >& positions, std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc)
{

	perm.resize(this->number_of_regions_);
	poros.resize(this->number_of_regions_);
	visc.resize(this->number_of_regions_);
	positions.resize(3 * this->number_of_regions_);

	np = this->number_of_regions_;

	for (auto it = 0; it < this->number_of_regions_; it++)
	{
		perm[it] = perm_values[it];
		poros[it] = poros_values[it];
		visc[it] = visc_values[it];
		positions[it * 3 + 0] = positions_values[it].x();
		positions[it * 3 + 1] = positions_values[it].y();
		positions[it * 3 + 2] = positions_values[it].z();
	}
}

void FlowParametersBar_new::loadRegions(const int np, const std::vector< double >& positions, const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc)
{
	spinBox_Number_of_Regions->setValue(np);
	this->number_of_regions_ = np;

	for (auto it = 0; it < this->number_of_regions_ ; it++)
	{
		perm_values[it]      = perm[it];
		poros_values[it]	 = poros[it];
		visc_values[it]		 = visc[it];
		positions_values[it].x() = positions[it*3+0];
		positions_values[it].y() = positions[it*3+1];
		positions_values[it].z() = positions[it*3+2];
	}

	this->createRegions();
}

void FlowParametersBar_new::createRegions( )
{
	QStringList l;

	for (auto i = 0; i < spinBox_Number_of_Regions->value(); i++)
	{
		l.push_back("Region " + QString::number(i + 1));
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

/// Well
void FlowParametersBar_new::set_region_point(double x, double y, double z)
{
	positions_values[comboBox_Region->currentIndex()][0] = x;
	positions_values[comboBox_Region->currentIndex()][1] = y;
	positions_values[comboBox_Region->currentIndex()][2] = z;

	this->updateRegionWidget(comboBox_Region->currentIndex());
}

void FlowParametersBar_new::loadWells(const int nw, const std::vector< unsigned int >& type, const std::vector< double >& pressure, const std::vector< int >& sign)
{
	this->number_of_wells_ = nw;

	for (auto it = 0; it < nw; it++)
	{
		this->well_pressure[it] = pressure[it];
		this->well_types[it] = type[it];
		this->well_signs[it] = sign[it];
	}

	spinBox_Number_of_Wells->setValue(this->number_of_wells_);

	this->createWells();
}

void FlowParametersBar_new::getWellParameter_new(int& nw, std::vector< unsigned int >& type, std::vector< double >& pressure, std::vector< int >& sign)
{
	nw = this->number_of_wells_;

	for (auto it = 0; it < nw; it++)
	{
		pressure[it] = this->well_pressure[it];
		type[it] = this->well_types[it];
		sign[it] = this->well_signs[it];
	}
}

void FlowParametersBar_new::createWells()
{
	QStringList l;
	for (auto i = 0; i < spinBox_Number_of_Wells->value(); i++)
	{
		l.push_back("Well " + QString::number(i + 1));
	}
	comboBox_Well->clear();
	comboBox_Well->insertItems(0, l);
	this->updateWellsWidget(comboBox_Well->currentIndex());
}

void FlowParametersBar_new::updateWellsWidget(const int index)
{
	doubleSpinBox_Well_Pressure->setValue(well_pressure[index]);
	spinBox_Well_Sign->setValue(well_signs[index]);
	spinBox_Well_Type->setValue(well_types[index]);
	
}
