#ifndef DIALOGMESHVISUALIZATIONPARAMETERS_H
#define DIALOGMESHVISUALIZATIONPARAMETERS_H

#include <QDialog>

#include "ui_dialogmeshvisualizationparameters.h"

class DialogMeshVisualizationParameters : public QDialog,
        private Ui::DialogMeshVisualizationParameters
{
    Q_OBJECT

public:
    explicit DialogMeshVisualizationParameters(QWidget *parent = 0);
    ~DialogMeshVisualizationParameters();

private:
};

#endif // DIALOGMESHVISUALIZATIONPARAMETERS_H
