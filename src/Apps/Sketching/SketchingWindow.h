#ifndef SKETCHINGWINDOW_H
#define SKETCHING2DMODULE_H

#include <QtWidgets/QAction>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidgetAction>


#include "Sketching/SketchingCanvas.h"

/**!
* @brief  brief  Embeds the sketching area into the main window.
*
*/


class SketchingWindow : public QMainWindow
{
	Q_OBJECT

    public:



        SketchingWindow( QWidget* parent = 0 );



    protected:


        void init();

        void createWindow();

        void createActions();
        void createDefineSketchRegionsActions();
        void createRulesActions();
        void createNewActions();
        void createEditActions();

        void createColorActions();
        void createMiscActions();


        void createConnections();
        void resetActions();




    public slots:


        inline void setScene( Scene* const& scene ){ sketching_canvas->addScene( scene ); }


        inline void enableUndo( bool option ){ ac_undo->setEnabled( option ); }
        inline void enableRedo( bool option ){ ac_redo->setEnabled( option ); }
        inline void enableSketching( bool option ){ ac_sketch->setChecked( option ); }

        void changeDefineRegionStatus( const bool above, const bool below );
        void changeStratigraphyRulesStatus( const std::string& rule );




    signals:

        void undo();
        void redo();
        void clear();
        void updateStratigraphicRule( const std::string& );
        void defineSketchingAbove( bool );
        void defineSketchingBelow( bool );

        void addStratigraphy();
        void undoLastSketch();
        void setCurrentMode( const Scene::InteractionMode& );
        void updateColor( const QColor& c );

        void setRandomColor ( bool );



    private:


        SketchingCanvas* sketching_canvas;


        QToolBar *tlb_section;
        QAction *ac_new_boundary;
        QAction *ac_new;
        QAction *ac_add_sketch;
        QAction *ac_deny_curve;
        QAction *ac_undo;
        QAction *ac_redo;


        QToolBar *tlb_rules;
        QActionGroup *ag_sketching_rules;
        QActionGroup *ag_remove_rules;
        QAction *ac_removeabove;
        QAction *ac_removebelow;
        QAction *ac_sketch;
        QAction *ac_removeaboveintersection;
        QAction *ac_removebelowintersection;

        QToolBar *tlb_sketchregions;
        QAction *ac_sketchabove;
        QAction *ac_sketchbelow;

        QToolBar *tlb_interaction;
        QToolBar *tlb_edit;

        QToolBar *tlb_customization;
        QAction *ac_screenshot;
        QWidgetAction *ac_sketchcolor;
        QMenu *mn_pickercolor;
        QColorDialog *cd_pickercolor;
        QToolButton *tbt_colorsketch;

        QLabel* lb_statusbar_status;


};

#endif // SKETCHING2DMODULE_H

