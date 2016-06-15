#include "DialogMeshVisualizationParameters.h"
#include "ui_DialogMeshVisualizationParameters.h"

DialogMeshVisualizationParameters::DialogMeshVisualizationParameters(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setLayout( vl_trianglelayout );
}

DialogMeshVisualizationParameters::~DialogMeshVisualizationParameters()
{
}
