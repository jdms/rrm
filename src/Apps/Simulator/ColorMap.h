#ifndef COLORMAP_H
#define COLORMAP_H

#include <vector>

#include <QVector3D>


class ColorMap
{

    public:


        enum class COLORMAP{ JET, CONSTANT };


        ColorMap();

        QVector3D getColor( COLORMAP map, float value, float low, float high ) const;
        QVector3D getColor( const std::vector< QVector3D >& map, float value, float low, float high ) const ;
        QVector3D getConstantColor() const;
        std::vector< QVector3D > getColors( COLORMAP cm, unsigned int& nc ) const;

    protected:

        std::vector< QVector3D > jet;

};

#endif // COLORMAP_H
