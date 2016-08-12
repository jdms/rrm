#ifndef SKETCHING2DMODULE_H
#define SKETCHING2DMODULE_H

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMainwindow>
#include <QtWidgets/QAction>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QHBoxLayout>
#include "Modeller/SketchBoardWidget/SketchBoard.hpp"

/**!
* @brief  brief  Embeds the sketching area into the main window.
*
*/

class Sketching2DModule : public QMainWindow
{
	Q_OBJECT

    public:

        SketchBoard* sketching_canvas;

        Sketching2DModule(QWidget* parent);


    protected:
        void createWindow();
        void createActions();
        void createConnections();


    signals:

        void setColor(float, float, float);



    private:

        /// View
        QAction * ac_zoom_in;
        QAction * ac_zoom_out;

        QToolBar *tlb_section;
        QAction *ac_new_boundary;
        QAction *ac_screenshot;
        QAction *ac_new;
        QAction *ac_add_sketch;
        QAction *ac_deny_curve;
        QAction *ac_undo;
        QAction *ac_redo;


        /// Temporary
        QAction * ac_region_point;

        QToolBar *tlb_rules;
        QActionGroup *action_group_rules;
        QAction *ac_removeabove;
        QAction *ac_removebelow;
        QAction *ac_sketch;

        QToolBar *tlb_interaction;
        QAction *ac_select;

        QToolBar *tlb_customization;
        QWidgetAction *ac_sketchcolor;
        QMenu *mn_pickercolor;
        QColorDialog *cd_pickercolor;
        QToolButton *tbt_colorsketch;


        QLabel* lb_statusbar_status;

};

#endif // SKETCHING2DMODULE_H

