#ifndef VIEW3DMODULE_H
#define VIEW3DMODULE_H


#include <QMainWindow>
#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGridLayout>
#include <QLabel>



#include "3dView/View3DCanvas.hpp"

/**!
 * @brief  brief  Embeds the sketching area into the main window.
 *
 */

class View3DWindow: public QMainWindow
{
    Q_OBJECT

    public:



        View3DWindow( QWidget* parent = 0 );

        inline void setScene( Scene* scene )
        {
            canvas3d->setScene( scene );
            connect( scene, SIGNAL( updatedScene() ), canvas3d, SLOT( update() ) );
        }


    protected:

        void createWindow();


    signals:

        void initializeScene();
        void changeResolution( const int, const int );
        void exportToCPS3();


    public:

        View3DCanvas *canvas3d;
        QSlider* sl_resolution_numI;
        QSlider* sl_resolution_numJ;



};

#endif // VIEW3DMODULE_H
