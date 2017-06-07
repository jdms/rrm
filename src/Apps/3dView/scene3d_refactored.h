#ifndef SCENE3D_REFACTORED_H
#define SCENE3D_REFACTORED_H

#include <QObject>
#include <QColor>

#include "Eigen/Dense"

class QString;
class QOpenGLContext;
class QSurface;

class Volume;
class VolumeOpenGLWrapper_Refactored;
class CrossSectionOpenGLWrapper_Refactored;
class Object_Refactored;
class ObjectOpenGLWrapper_Refactored;


class Scene3d_refactored: public QObject
{
    Q_OBJECT

    public:

        Scene3d_refactored();

        void addVolume( Volume* const& vol );

        void addCrossSection();

        bool addObject( Object_Refactored* const& obj );
        bool isAddedObject( std::size_t id );

        inline void setActiveObjects(){}

        inline void setObjectSelected(){}
        inline void setObjectsSelected(){}

        inline void setObjectSelectable(){}
        inline void setObjectsSelectable(){}

        void setCurrentColor( const QColor& color );

        void updateVolume();
        void updateObject( std::size_t );
        inline void updateObjects(){}
        inline void updateScene(){}

        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w, const int& h );

        void clear();
        void clearData();

        void setCurrentDirectory( const QString& dir );
        void setOpenGLContext( QOpenGLContext* ctxt );


    signals:

        void updateCanvas();


    private:


        QString shader_directory;
        QColor current_color;

        QOpenGLContext* context;
        QSurface* surface;

        VolumeOpenGLWrapper_Refactored* volume;
        CrossSectionOpenGLWrapper_Refactored* csection;

        std::map< std::size_t, ObjectOpenGLWrapper_Refactored* > objects;

};

#endif // SCENE3D_REFACTORED_H
