#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <memory>

#include <QMainWindow>
#include <QToolBar>

#include "./core/models/controller.h"
#include "rrmapplication.h"
#include "sketchinterface.h"
#include "view3dinterface.h"




class MainWindow1: public QMainWindow
{
  Q_OBJECT


  public:

    MainWindow1( QWidget* parent_ = 0 );

  protected:

    void setWindowProperties();

    void createWindow();
    void createActions();
    void createMenuBar();
    void createToolbar();

    void createController();

    void plugSketchInterface();
    void plug3dInterface();

  protected:

    friend class SketchInterface;
    friend class View3dInterface;
    friend class RRMApplication;

    std::shared_ptr< SketchInterface > sketchapp;
    std::shared_ptr< View3dInterface > view3dapp;

    Controller* controller;
    RRMApplication* app;

    QAction* ac_save = nullptr;
    QAction* ac_load = nullptr;
    QAction* ac_clear = nullptr;
    QAction *ac_export = nullptr;
    QAction* ac_exit = nullptr;
    QAction* ac_manual = nullptr;
    QAction* ac_about = nullptr;

    QAction* ac_undo = nullptr;
    QAction* ac_redo = nullptr;
    QAction* ac_sketch_above = nullptr;
    QAction* ac_sketch_below = nullptr;
    QAction* ac_remove_above = nullptr;
    QAction* ac_remove_above_int = nullptr;
    QAction* ac_remove_below = nullptr;
    QAction* ac_remove_below_int = nullptr;
    QAction* ac_screenshot = nullptr;

    QMenu* mn_file = nullptr;
    QMenu* mn_windows = nullptr;
    QMenu* mn_help = nullptr;

    QToolBar* tb_mainwindow;



};

#endif // MAINWINDOW1_H
