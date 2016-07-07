#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include "ui_HelpDialog.h"

class HelpDialog : public QDialog,
        private Ui::HelpDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = 0);
    ~HelpDialog();

    void createListItems();
    void createViewContents();

private slots:
    void on_lw_contentsitems_clicked(const QModelIndex &index);
};

#endif // HELPDIALOG_H
