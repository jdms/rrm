#ifndef CSECTIONSCENE_H
#define CSECTIONSCENE_H

#include "sketchscene_refactored.h"

class CSectionScene: public SketchScene_Refactored
{
    Q_OBJECT

    public:

        CSectionScene();

        inline bool isDuplicatedObject(){ return true; }
        inline void setActiveObjects( const std::vector< std::size_t >& objs ){}
        inline void setObjectsSelected( const std::vector< std::size_t >& objs ){}
        inline void setObjectsSelectable( const std::vector< std::size_t >& objs ){}
        inline void updateObjects( const std::vector< std::size_t >& objs ){}


    private:

         std::map< std::size_t, ObjectItemWrapper_Refactored* > objects;

};

#endif // CSECTIONSCENE_H
