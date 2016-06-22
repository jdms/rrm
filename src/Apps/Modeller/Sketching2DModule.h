#ifndef SKETCHING2DMODULE_H
#define SKETCHING2DMODULE_H

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMainwindow>

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
		QMainWindow * main_widget_;

    protected:
        void createWindow();
        void createActions( QWidget* parent );

    public slots:

        void applyRemoveAbove();
        void applyRemoveBelow();
        void pointerSelection( bool flag );

        void emitColor( int R, int G, int B );
        void emitModeSelect();
        void emitModeSketching();


    signals:

        void applyremoveabove();
        void applyremovebelow();
        void setColor( int R, int G, int B );
        void selectMode();
        void sketchingMode();


    private:
				
};

#endif // SKETCHING2DMODULE_H
