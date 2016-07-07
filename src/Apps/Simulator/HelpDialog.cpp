#include "HelpDialog.h"
#include "ui_HelpDialog.h"

#include <iostream>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setWindowTitle("Contents");
    createListItems();
    createViewContents();

}

void HelpDialog::createListItems()
{
    lw_contentsitems->addItems( QStringList()
            << "Number of regions of different permeability or porosity (edit by user)."
            << "Permeability, porosity, viscosity values and region node coordinates x y z (edit by user)"
            << "Total number of wells."
            << "Well number, corrected pressure, well locations (u, v)"
            << "Total number of TOF boundaries"
            << "TOF sign"
            << "TOF boundary well"
            << "Total number of Tracer boundaries"
            << "Tracer sign"
            << "Tracer boundary well"
      );
}

void HelpDialog::createViewContents()
{
}

HelpDialog::~HelpDialog()
{
//    delete ui;
}

void HelpDialog::on_lw_contentsitems_clicked(const QModelIndex &index)
{
    te_contentsview->clear();

    if( index.row() == 0 )
        te_contentsview->setText("1st entry is the total number of regions of different permeability or porosity or viscosity.");
    else if( index.row() == 1 )
        te_contentsview->setText("Values of each region are in one row. 1st entry is permeability value in unit mD (1mD=10^-15m^2); 2nd entry is porosity value and 3rd entry is viscosity value in cP (1cp = 0.001pa*s). The region node coordinates are of any node inside this region to help identify this region when generating volume mesh. The x, z coordinates can be read from the sketch window. Because this is simple extraction, the y coordinate can be any positive number smaller than the extraction length.");
    else if( index.row() == 2 )
        te_contentsview->setText("At least 2 wells");
    else if( index.row() == 3 )
        te_contentsview->setText("1st entry is the numbering of the wells (start from 0). 2nd entry is the corrected pressure of the well. 3rd and 4th entries are parametric locations of the wells. 0, 0 and 1, 1 are at the two corners of the model (0, 1 or 1, 0 can also be used). Currently wells can only be defined at the 4 corners.");
    else if( index.row() == 4 )
        te_contentsview->setText("If 0, time-of-flight is not calculated.");
    else if( index.row() == 5 )
        te_contentsview->setText("1 means forward TOF, -1 means backward TOF");
    else if( index.row() == 6 )
        te_contentsview->setText("1st entry is the TOF boundary ordering (start from 0), 2nd entry is the well number of the TOF boundary");
    else if( index.row() == 7 )
        te_contentsview->setText("If 0, tracer is not calculated.");
    else if( index.row() == 8 )
        te_contentsview->setText("1 means forward Tracer, -1 means backward TOF");
    else if( index.row() == 9 )
        te_contentsview->setText("1st entry is the Tracer boundary ordering (start from 0), 2nd entry is the well number of the Tracer boundary");

}
