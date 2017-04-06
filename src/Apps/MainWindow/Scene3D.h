#ifndef SCENE3D_H
#define SCENE3D_H


#include <vector>


#include <GL/glew.h>

#include "Volume.h"
#include "Object.h"
#include "Region1.h"
#include "CrossSection.h"


#include <QObject>
#include <QOpenGLContext>

class Scene3D: public QObject
{

    Q_OBJECT

    public:

        Scene3D();

        inline bool addVolume( const Volume* volume_ )
        {
            volumes.push_back( volume_ );
            return true;
        }

        inline bool addObject( Object* object_ ){ return true; }
//        inline bool addCrossSection( const CrossSection1* csection_ ){ return true; }
        inline bool addRegion( const Region1* region_ ){ return true; }

        inline void setContext( QOpenGLContext* cnt_ ){ context = cnt_; }

    protected:

        QOpenGLContext* context;

        std::vector< const Volume* > volumes;



};

#endif // SCENE3D_H
