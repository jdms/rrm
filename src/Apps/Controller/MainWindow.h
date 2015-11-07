#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QDialog>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialogButtonBox>

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QStyle>

/// CrossSection
#include "Model/CrossSection.hpp"

#include "Modeller/ExtrusionWidget/OpenGLWidget.hpp"

#include "Modeller/Canvas2D.h"

#include "Modeller/SketchBoardWidget/SketchBoard.hpp"
	#include "Modeller/SketchBoardWidget/SketchSession/SketchSession.hpp"

#include "Modeller/HorizonController.h"

// /// ZHAO -----// ZHAO
//#include "Simulator/canvasComputation.h"
// /// ZHAO -----// ZHAO

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        explicit MainWindow( QWidget *parent = 0 );
        ~MainWindow();

    protected:

        void createWindow();
        void createMenuBar();
        void createActions();
        void createToolbar();

        void create2DModule();
        void create3DModule();

        void clearCanvas2D();

    protected slots:

        void newSection();
        void applyRemoveAbove();
        void applyRemoveBelow();

        void pointerSelection();
        void changeColorLine();

        /// CrossSection
        void keyPressEvent ( QKeyEvent *e );
        void mousePressEvent(QMouseEvent* event);
        void wheelEvent(QWheelEvent* event);

    public slots:

        //void addCurve( QPolygonF& curve);

        //void updateComboBox( std::vector< std::string > ppoints, std::vector< std::string > pcells );

        void curve2DSlot ( QPolygonF polygon );
        void smoothCurveSlot ( QPolygonF raw_sketch );
        void renderSegments();
        void newSessionSlot(QPixmap pixmap);
        void newSessionSlot(qreal x , qreal y, qreal width, qreal height);
	void update3DExtrusion();

    signals:

	void newBoundarySignal();


    signals:

        void applyremoveabove();
        void applyremovebelow();
        void setColor( int R, int G, int B );

        void selectMode();
        void sketchingMode();

    	/// CrossSection

        void newBoundary();

    private:

        QDockWidget *dc_2DModule;
        QDockWidget *dc_3DModule;
        QDockWidget *dc_computation;

        QAction *ac_new;
        QAction *ac_open;
        QAction *ac_save;
        QAction *ac_export;
        QAction *ac_exit;

        QAction *ac_contents;
        QAction *ac_about;

        QAction *ac_removeabove;
        QAction *ac_removebelow;

        QAction *ac_select;
        QWidgetAction *ac_sketchcolor;

        QToolBar *tlb_section;
        QToolBar *tlb_rules;
        QToolBar *tlb_interaction;
        QToolBar *tlb_customization;

        QMenu *mn_pickercolor;
        QColorDialog *cd_pickercolor;
        QToolButton *tbt_colorsketch;

        QAction *ac_wdwsketching;
        QAction *ac_window3d;

        /// CrossSection ////////////////////////////////////////////////////////////////////
        RRM::CrossSection<qreal> cross_section_;

        QStatusBar			*status_bar_;
        	QLabel			*status_text;

        QAction *ac_newBoundary;
        /// Sketching Surface and Extrusion
        GLWidget *glWidget;

	// Sketch Views
	SketchBoard  		*sketch_board_;
		SketchSession   *sketchSession_;

	typedef RRM::CrossSection<qreal>                   CrossSection;
	typedef CrossSection::Curve2D			   Curve2D;

		// Model Testing
	QPolygonF convert( Curve2D& curve)
	{
		QPolygonF p;

		for(std::size_t i = 0; i < curve.size(); i++)
		{
			p.push_back(QPointF(curve[i].x(),curve[i].y()));
		}
		return p;
	}



};

#endif // MAINWINDOW_H
