#ifndef SCENE3D_REFACTORED_H
#define SCENE3D_REFACTORED_H

#include <QObject>
#include <QColor>

class QString;
class QOpenGLContext;
class QSurface;


class Scene3d_refactored: public QObject
{
    Q_OBJECT

    public:

        Scene3d_refactored();

        inline void addVolume(){}

        inline void addObject(){}
        inline bool isDuplicatedObject(){ return true; }

        inline void setActiveObjects(){}

        inline void setObjectSelected(){}
        inline void setObjectsSelected(){}

        inline void setObjectSelectable(){}
        inline void setObjectsSelectable(){}

        void setCurrentColor( const QColor& color );

        inline void upadteVolume(){}
        inline void updateObject(){}
        inline void updateObjects(){}
        inline void updateScene(){}

        inline void draw(){}
        inline void clear(){}

        inline void setDefaultValues(){}
        void setCurrentDirectory( const QString& dir );
        void setOpenGLContext( QOpenGLContext* const& ctxt );


    private:


        QString shader_directory;
        QColor current_color;

        QOpenGLContext* context;
        QSurface* surface;



};

#endif // SCENE3D_REFACTORED_H
