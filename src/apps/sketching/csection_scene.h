#ifndef CSECTIONSCENE_H
#define CSECTIONSCENE_H

#include "sketch_scene.h"

class CSectionScene: public SketchScene
{
    Q_OBJECT

    public:

        CSectionScene();

        inline bool isDuplicatedObject(){ return true; }
        inline void setActiveObjects( const std::vector< std::size_t >& objs, bool status ){}
        inline void setObjectsSelected( const std::vector< std::size_t >& objs, bool status ){}
        inline void setObjectsSelectable( const std::vector< std::size_t >& objs, bool status ){}
        inline void updateObjects( const std::vector< std::size_t >& objs ){}



//         std::map< std::size_t, ObjectItemWrapper* > objects;

};

#endif // CSECTIONSCENE_H
