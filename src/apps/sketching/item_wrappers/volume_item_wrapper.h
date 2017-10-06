#ifndef VOLUMEITEMWRAPPER_H
#define VOLUMEITEMWRAPPER_H

#include <QGraphicsPathItem>
#include <QColor>
#include <QPainter>

class Volume;


class VolumeItemWrapper: public QGraphicsPathItem
{

    public:

        enum class Section{ XY, XZ };

        VolumeItemWrapper() = default;
        VolumeItemWrapper( Volume* const& vol_, const Section& sec_ = Section::XY );


        void setRawVolume( Volume* const& vol );
        Volume* getRawVolume() const ;


        void defineSectionPlane( const Section& sec );
        VolumeItemWrapper::Section getSectionPlane() const;


        double getHeight() const;
        double getWidth() const ;


        void resize( double width, double height, double ox = 0.0, double oy = 0.0 );
        void updateItem();


        bool isVisible() const ;
        void clear();

        QRectF boundingRect() const;


    protected:

        void setupPen();
        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );



    private:

        QPolygonF section_boundary;
        QPen countor_volume;
        QBrush fill_volume;

        Volume* raw;
        Section section;

};

#endif // VOLUMEITEMWRAPPER_H
