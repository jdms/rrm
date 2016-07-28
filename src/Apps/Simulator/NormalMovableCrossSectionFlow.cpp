#include "NormalMovableCrossSectionFlow.h"
#include "ui_NormalMovableCrossSectionFlow.h"

NormalMovableCrossSectionFlow::NormalMovableCrossSectionFlow(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    this->setLayout( hl_layout );
}

NormalMovableCrossSectionFlow::~NormalMovableCrossSectionFlow()
{
}

void NormalMovableCrossSectionFlow::on_btn_acceptnormal_accepted()
{
    float X = edt_normalCoordX->text().toFloat();
    float Y = edt_normalCoordY->text().toFloat();
    float Z = edt_normalCoordZ->text().toFloat();

    emit sendCrossSectionNormalCoordinates( X, Y, Z );

}

void NormalMovableCrossSectionFlow::on_btn_acceptnormal_rejected()
{
    edt_normalCoordX->clear();
    edt_normalCoordY->clear();
    edt_normalCoordZ->clear();

    emit canceled();

}

void NormalMovableCrossSectionFlow::clear()
{
    edt_normalCoordX->clear();
    edt_normalCoordY->clear();
    edt_normalCoordZ->clear();
}
