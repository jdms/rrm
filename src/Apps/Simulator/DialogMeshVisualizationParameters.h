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

        void setVisualizationParameters( const std::string& trianglecmd, const std::string& resolutiontype, const int& npartitionedge, const int&lenghtedge );

    private slots:

    void on_buttonBox_accepted();
        void on_buttonBox_rejected();

    signals:

        void sendVisualizationParameters( const std::string&, const std::string&, const float& );
        void closeBar();

    private:
};

#endif // DIALOGMESHVISUALIZATIONPARAMETERS_H
