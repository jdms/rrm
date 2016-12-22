#ifndef HELPDIALOG_H
#define HELPDIALOG_H


#include <iostream>

#include <QDialog>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QHBoxLayout>

//#include <QImage>
//#include <QImageReader>
//#include <QTextStream>

//#include <QtWebKitWidgets/QWebView>
//#include <QUrl>

#include "ui_HelpDialog.h"

class HelpDialog : public QDialog,
        private Ui::HelpDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = 0);
    ~HelpDialog();

    inline void setCurrentDirectory( std::string current_dir ){ current_directory.clear(); current_directory = current_dir; }

private slots:
    void on_lw_userguide_clicked(const QModelIndex &index);
    void on_lw_contentsitems_clicked(const QModelIndex &index);

private:

    std::string current_directory;
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
};

#endif // HELPDIALOG_H
