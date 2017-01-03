#ifndef COLORMAP_H
#define COLORMAP_H

#include <vector>

#include <QVector3D>


class ColorMap
{

    public:


        enum class COLORMAP{ JET, HOT, COOL, PARULA, SPRING, SUMMER, COPPER, POLAR, WINTER, CONSTANT, QUALITATIVE };


        ColorMap();

        QVector3D getColor( COLORMAP map, float value, float low, float high ) const;
        QVector3D getColor( const std::vector< QVector3D >& map, float value, float low, float high ) const ;
        QVector3D getConstantColor() const;
        std::vector< QVector3D > getColors( COLORMAP cm, unsigned int& nc ) const;

    protected:

        std::vector< QVector3D > jet;
        std::vector< QVector3D > hot;
        std::vector< QVector3D > cool;
        std::vector< QVector3D > parula;
        std::vector< QVector3D > spring;
        std::vector< QVector3D > summer;
        std::vector< QVector3D > copper;
        std::vector< QVector3D > polar;
        std::vector< QVector3D > winter;
		/// @see http://colorbrewer2.org/#type=qualitative&scheme=Set3&n=12
		std::vector< QVector3D > qualitative;

};

#endif // COLORMAP_H
