#include "FlowParametersBar_new.h"


FlowParametersBar_new::FlowParametersBar_new(QWidget *parent) :
    QWidget(parent)
{
    setupUi( this );

	/// Region Dialogs
    createDialogs();

	// Creating Fixed valeu for wells
	this->well_types[0] = 1;
	this->well_pressure[0] = 400;
	this->well_signs[0] = 1;

	this->well_types[1] = 1;
	this->well_pressure[1] = 300;
	this->well_signs[1] = -1;

	this->spinBox_Number_of_Wells->setValue(2);
	this->spinBox_Number_of_Wells->setEnabled(false);
	this->createWells();

	this->spinBox_Well_Type->setReadOnly(true);
	this->spinBox_Well_Sign->setReadOnly(true);

	this->number_of_regions_ = 0;
	
}

void FlowParametersBar_new::createDialogs()
{

	connect(button_inputparameters, &QPushButton::clicked, this, [=](){
		QString selected_format = "";
		QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "./inputs/",
			".ui files (*.ui)", &selected_format);
		if (filename.isEmpty() == true)
		{
			return;
		}
		else
		{
			edit_inputparameters->setText(filename);
			emit readParameterFile(edit_inputparameters->text().toStdString());
		}

		
	});
	/// New GUI ---->
	//// REGION
	connect(spinBox_Number_of_Regions, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &FlowParametersBar_new::createRegions);
	connect(comboBox_Region, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, [=](){ updateRegionWidget(comboBox_Region->currentIndex()); });

	//@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5

	connect(horizontalSlider_Permeability, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), this, [=]()
	{ 
		double ex = -3.0 + (0.07)*static_cast<double>(horizontalSlider_Permeability->value());
		double p = std::pow(10, ex);
		//std::cout << "Step " << horizontalSlider_Permeability->value() << " 10^" << ex << "= " << p << std::endl;
		doubleSpinBox_Region_Permeability->setValue(p);
		permeability_slider_values[comboBox_Region->currentIndex()] = horizontalSlider_Permeability->value();
	});

	connect(horizontalSlider_Porosity, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), this, [=]()
	{
		double ex = 0.01 + (0.0034)*static_cast<double>(horizontalSlider_Porosity->value());
		doubleSpinBox_Region_Porosity->setValue(ex);
		porosity_slider_values[comboBox_Region->currentIndex()] = horizontalSlider_Porosity->value();
	});

	connect(horizontalSlider_Viscosity, static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged), this, [=]()
	{
		double ex = 0.1 + (0.1)*static_cast<double>(horizontalSlider_Viscosity->value());
		doubleSpinBox_Region_Viscosity->setValue(ex);
		/// FIXME Viscosity have to be the same for all model for now, util a better
		/// interface if design.
		for (auto& v : viscosity_slider_values)
		{
			v.second = horizontalSlider_Viscosity->value();
		}
	});

	connect(doubleSpinBox_Region_Permeability, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=]()
	{
		permeability_values[comboBox_Region->currentIndex()] = doubleSpinBox_Region_Permeability->value(); 
		
	});
	connect(doubleSpinBox_Region_Porosity, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=]()
	{ 
		porosity_values[comboBox_Region->currentIndex()] = doubleSpinBox_Region_Porosity->value(); 
		
	});
	connect(doubleSpinBox_Region_Viscosity, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=]()
	{ 
		/// FIXME Viscosity have to be the same for all model for now, util a better
		/// interface if design.
		for (auto& v : viscosity_values)
		{
			v.second = doubleSpinBox_Region_Viscosity->value();
		}
		
	});
	connect(doubleSpinBox_Region_Point_X, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](){ positions_values[comboBox_Region->currentIndex()][0] = doubleSpinBox_Region_Point_X->value(); });
	connect(doubleSpinBox_Region_Point_Y, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](){ positions_values[comboBox_Region->currentIndex()][1] = doubleSpinBox_Region_Point_Y->value(); });
	connect(doubleSpinBox_Region_Point_Z, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](){ positions_values[comboBox_Region->currentIndex()][2] = doubleSpinBox_Region_Point_Z->value(); });
			
	//// WELL
	connect(spinBox_Number_of_Wells, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &FlowParametersBar_new::createWells);
	connect(comboBox_Well, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, [=](){ updateWellsWidget(comboBox_Well->currentIndex()); });

	//@see http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
	connect(doubleSpinBox_Well_Pressure, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](){ well_pressure[comboBox_Well->currentIndex()] = doubleSpinBox_Well_Pressure->value(); });
	connect(spinBox_Well_Type, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](){ well_types[comboBox_Well->currentIndex()] = spinBox_Well_Type->value(); });
	connect(spinBox_Well_Sign, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=](){ well_signs[comboBox_Well->currentIndex()] = spinBox_Well_Sign->value(); });

	// Set All Attributes to the  default value
	this->newGUIReset();

}

void FlowParametersBar_new::setPropertyAreaParameters( const int np, const std::vector< double >& positions , const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc )
{
	loadRegions(np, positions, perm, poros, visc);
}

void FlowParametersBar_new::getPropertyAreaParameters( int& np, std::vector< double >& values , std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc )
{
	getPropertyAreaParameters_new(np, values, perm, poros, visc);
}

void FlowParametersBar_new::setWellParameter( const int nw,  const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign )
{
	loadWells(nw, type, value, sign);
}

void FlowParametersBar_new::getWellParameter( int& nw,  std::vector< unsigned int >& type, std::vector< double >& value, std::vector< int >& sign )
{
	getWellParameter_new(nw, type, value, sign);
}

int FlowParametersBar_new::getNumberOfRegions()
{
	return this->number_of_regions_;
}

void FlowParametersBar_new::clear()
{
	edit_inputparameters->clear();

	/// New GUI ---->
	newGUIClear();
}

/// New GUI ---->
void FlowParametersBar_new::newGUIReset()
{
	//horizontalSlider_Permeability->setValue(75);
	//	emit horizontalSlider_Permeability->valueChanged(75);
	//horizontalSlider_Porosity->setValue(88);
	//	emit horizontalSlider_Porosity->valueChanged(88);
	//horizontalSlider_Viscosity->setValue(9);
	//	emit horizontalSlider_Viscosity->valueChanged(9);

	//doubleSpinBox_Region_Permeability->setValue(200.0);
	//doubleSpinBox_Region_Porosity->setValue(0.2);
	//doubleSpinBox_Region_Viscosity->setValue(1.0);
	//doubleSpinBox_Region_Point_X->setValue(0.0);
	//doubleSpinBox_Region_Point_Y->setValue(0.0);
	//doubleSpinBox_Region_Point_Z->setValue(0.0);

	//doubleSpinBox_Well_Pressure->setValue(0);
	//spinBox_Well_Sign->setValue(1);
	//spinBox_Well_Type->setValue(1);

	//spinBox_Number_of_Regions->setValue(0);
	//spinBox_Number_of_Wells->setValue(0);
}

void FlowParametersBar_new::newGUIClear()
{
	/// Reset all dictionaries
	//positions_values.clear();
	//permeability_values.clear();
	//porosity_values.clear();
	//viscosity_values.clear();

	//well_types.clear();
	//well_pressure.clear();
	//well_signs.clear();

	/// Reset all Gui Values
	//comboBox_Region->clear();
	//comboBox_Well->clear();

	this->newGUIReset();
}

/// Slot used to grab Region Point
void FlowParametersBar_new::setRegionPoints(const std::map<int, Eigen::Vector3f>& region_points)
{
	/// Coordinate System for FlowDiagnostic is  <x, z, y>
	/// @FIXEME Be Careful about the coordinate sytems
	for (auto index : region_points)
	{
		// 
		positions_values[index.first][0] = index.second.x();
		// Z
		positions_values[index.first][2] = index.second.y();
		// Y
		positions_values[index.first][1] = this->depth_;

	}

	this->updateRegionWidget(comboBox_Region->currentIndex());
}


void FlowParametersBar_new::setRegionDepth(const float _depth)
{
	this->depth_ = _depth;

	/// Coordinate System for FlowDiagnostic is  <x, z, y>
	/// @FIXEME Be Careful about the coordinate sytems
	for (auto index : positions_values)
	{
		// 
		positions_values[index.first][0] = positions_values[index.first][0];
		// Z
		positions_values[index.first][2] = positions_values[index.first][2];
		// Y
		positions_values[index.first][1] = this->depth_;

	}
	this->updateRegionWidget(comboBox_Region->currentIndex());

}


/// Grab current Region Information
void FlowParametersBar_new::getPropertyAreaParameters_new(int& np, std::vector< double >& positions, std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc)
{
	/// Ensure number of regions
	np = this->number_of_regions_;

	perm.resize(this->number_of_regions_);
	poros.resize(this->number_of_regions_);
	visc.resize(this->number_of_regions_);
	positions.resize(3 * this->number_of_regions_);

	for (int it = 0; it < this->number_of_regions_; it++)
	{
		perm[it] = permeability_values[it];
		poros[it] = porosity_values[it];
		visc[it] = viscosity_values[it];
		positions[it * 3 + 0] = positions_values[it].x();
		positions[it * 3 + 1] = positions_values[it].y();
		positions[it * 3 + 2] = positions_values[it].z();
	}
}

void FlowParametersBar_new::loadRegions(const int np, const std::vector< double >& positions, const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc)
{
	/// Update the GUI
	spinBox_Number_of_Regions->setValue(np);

	/// Update the Model
	this->number_of_regions_ = np;

	for ( int it = 0; it < this->number_of_regions_ ; it++)
	{
		permeability_values[it]      = perm[it];
		porosity_values[it]			 = poros[it];
		viscosity_values[it]		 = visc[it];
		positions_values[it].x() = positions[it*3+0];
		positions_values[it].y() = positions[it*3+1];
		positions_values[it].z() = positions[it*3+2];
	}

	/// Update the GUI based on the model
	this->createRegions();
}

/// Populate the combo box
void FlowParametersBar_new::createRegions( )
{
	QStringList l;

	for (auto i = 0; i < spinBox_Number_of_Regions->value(); i++)
	{
		l.push_back("Region " + QString::number(i + 1));

		if (permeability_values.count(i) == 0)
		{
			horizontalSlider_Permeability->setValue(75);
			emit horizontalSlider_Permeability->valueChanged(75);
			horizontalSlider_Porosity->setValue(88);
			emit horizontalSlider_Porosity->valueChanged(88);
			horizontalSlider_Viscosity->setValue(horizontalSlider_Viscosity->value());
			emit horizontalSlider_Viscosity->valueChanged(horizontalSlider_Viscosity->value());

			permeability_values[i] = doubleSpinBox_Region_Permeability->value();
			permeability_slider_values[i] = 75;
			porosity_values[i] = doubleSpinBox_Region_Porosity->value();
			porosity_slider_values[i] = 88;
 			viscosity_values[i] = doubleSpinBox_Region_Viscosity->value();
			viscosity_slider_values[i] = 9;
			positions_values[i].x() = 0.0;
			positions_values[i].y() = 0.0;
			positions_values[i].z() = 0.0;
		}

	}
	
	this->number_of_regions_ = spinBox_Number_of_Regions->value();

	comboBox_Region->clear();
	comboBox_Region->insertItems(0, l);
	/// Update the GUI using the current region information
	this->updateRegionWidget(comboBox_Region->currentIndex());

	emit numberRegions(spinBox_Number_of_Regions->value());
}

/// Update the GUI using the current region information
void FlowParametersBar_new::updateRegionWidget(const int index)
{
	doubleSpinBox_Region_Permeability->setValue(permeability_values[index]);
	horizontalSlider_Permeability->setValue(permeability_slider_values[index]);
	doubleSpinBox_Region_Porosity->setValue(porosity_values[index]);
	horizontalSlider_Porosity->setValue(porosity_slider_values[index]);
	doubleSpinBox_Region_Viscosity->setValue(viscosity_values[index]);
	horizontalSlider_Viscosity->setValue(viscosity_slider_values[index]);
	doubleSpinBox_Region_Point_X->setValue(positions_values[index].x());
	doubleSpinBox_Region_Point_Y->setValue(positions_values[index].y());
	doubleSpinBox_Region_Point_Z->setValue(positions_values[index].z());
}

/// --- Well Information
/// Load Well Information from a File
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


/// Grab current Well information
void FlowParametersBar_new::getWellParameter_new(int& nw, std::vector< unsigned int >& type, std::vector< double >& pressure, std::vector< int >& sign)
{

	/// Ensure correct number of Well
	nw = this->number_of_wells_;

	type.resize(this->number_of_wells_);
	pressure.resize(this->number_of_wells_);
	sign.resize(this->number_of_wells_);


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

	this->number_of_wells_ = spinBox_Number_of_Wells->value();

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
