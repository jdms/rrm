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
#include "Modeller/SketchBoardWidget/SketchBoard.hpp"

/**!
 * @brief  brief  Embeds the sketching area into the main window.
 *
 */

class Sketching2DModule: public QDockWidget
{
    Q_OBJECT

    public:

        Sketching2DModule( QWidget* parent );

        void clearCanvas2D();
		void setCentralWidget(QWidget * _centra_widget);

		QMainWindow * getMainWidow();
		QLabel      * getStatusBarText();
		// Sketch Module CrossSection
		SketchBoard  		*sketch_board_;
		
    protected:
        void createWindow();
		void createActions( QWidget* parent );
		void createConnections();

    public slots:

        void applyRemoveAbove();
        void applyRemoveBelow();
        void pointerSelection( bool flag );

        void emitColor( int R, int G, int B );
        void emitModeSelect();
        void emitModeSketching();


    signals:
		void setColor(float, float, float);
        void applyremoveabove();
        void applyremovebelow();
        void setColor( int R, int G, int B );
        void selectMode();
        void sketchingMode();


    private:

		QToolBar *tlb_section;
			QAction *ac_newBoundary;
			QAction *ac_new;
			/// Temporary
			QAction * ac_region_point_;

		QToolBar *tlb_rules;
			QActionGroup *action_group_rules_;
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

		QLabel * label_status_bar_text_;
		QMainWindow * mainWidonw_widget_;
};

#endif // SKETCHING2DMODULE_H

