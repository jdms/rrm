#ifndef VOLUMEITEMWRAPPER_H
#define VOLUMEITEMWRAPPER_H


#include <QPainter>
#include <QGraphicsPathItem>

#include "Core/Geology/Models/Volume.h"

class VolumeItemWrapper: public QGraphicsPathItem
{

    public:

        enum class Section{ XY, XZ };

        VolumeItemWrapper( const Section& sec_ = VolumeItemWrapper::Section::XY );

        inline void setSection( const Section& sec_ ){ section = sec_; }
        inline VolumeItemWrapper::Section getSection() const { return section; }

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );
        QRectF boundingRect() const;

        void setVolumeRaw( Volume* const &vol_ );
        void editGeometry( int ox_, int oy_, int w_, int h_ );

        inline double getWidth(){ return volume->getWidth(); }
        inline double getHeight()
        {
            if( section == Section::XZ )
                return volume->getDepth();
            return volume->getHeight();
        }


        inline void update()
        {
            boundary.clear();
            boundary = QPolygonF( QRectF( boundary.boundingRect().bottomLeft(),
                                          QSizeF( volume->getWidth(), volume->getHeight() ) ) );
        }

        inline void clear()
        {
            boundary.clear();
            if( volume != nullptr )
                volume->clear();
            volume = nullptr;
        }

    protected:

        QPolygonF boundary;
        Volume* volume;

        Section section = Section::XY;


};

#endif // VOLUMEITEMWRAPPER_H
