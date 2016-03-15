#ifndef CANVAS2D_H
#define CANVAS2D_H

#include <QGraphicsView>


#include "Modeller/SketchingScene.h"

/**!
 * @brief Qt GrapphicsView
 */

class Canvas2D: public QGraphicsView
{
    Q_OBJECT

    public:

        Canvas2D( QWidget *parent = 0 );
        ~Canvas2D();

        void clear();


    protected:

        void resizeEvent( QResizeEvent* re );


    private:

        SketchingScene *scene;

};

#endif // CANVAS2D_H
