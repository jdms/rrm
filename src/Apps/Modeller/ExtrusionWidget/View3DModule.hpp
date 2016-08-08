#ifndef VIEW3DMODULE_H
#define VIEW3DMODULE_H

#include <QtWidgets/QApplication>
#include <QWidget>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>

#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSlider>
#include <QMainWindow>


#include "Modeller/ExtrusionWidget/OpenGLWidget.hpp"

/**!
 * @brief  brief  Embeds the sketching area into the main window.
 *
 */

class View3DModule: public QMainWindow
{
    Q_OBJECT

    public:

    GLWidget *glWidget;

    View3DModule( QWidget* parent = 0 );

    QVBoxLayout *hb_canvas3d;
    QSlider *horizontalSlider_curve;
    QSlider *horizontalSlider_surface;
    QSlider *horizontalSlider_extrusion;

    protected:

    public slots:
        void emitColor( int R, int G, int B );
        void setBlackScreenVisibility( bool );

    signals:
        void setColor( int R, int G, int B );


    private:

	    QGroupBox *groupBox;
	    QFormLayout *formLayout;
	    QLabel *label;
	    QLabel *label_2;
	    QLabel *label_3;

};

#endif // VIEW3DMODULE_H
