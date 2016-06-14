#include "ColorMap.h"


ColorMap::ColorMap()
{

    jet = {
            QVector3D(  0,         0,         0.5625 ),
            QVector3D(  0,         0,         0.6250 ),
            QVector3D(  0,         0,         0.6875 ),
            QVector3D(  0,         0,         0.7500 ),
            QVector3D(  0,         0,         0.8125 ),
            QVector3D(  0,         0,         0.8750 ),
            QVector3D(  0,         0,         0.9375 ),
            QVector3D(  0,         0,         1.0000 ),
            QVector3D(  0,         0.0625,    1.0000 ),
            QVector3D(  0,         0.1250,    1.0000 ),
            QVector3D(  0,         0.1875,    1.0000 ),
            QVector3D(  0,         0.2500,    1.0000 ),
            QVector3D(  0,         0.3125,    1.0000 ),
            QVector3D(  0,         0.3750,    1.0000 ),
            QVector3D(  0,         0.4375,    1.0000 ),
            QVector3D(  0,         0.5000,    1.0000 ),
            QVector3D(  0,         0.5625,    1.0000 ),
            QVector3D(  0,         0.6250,    1.0000 ),
            QVector3D(  0,         0.6875,    1.0000 ),
            QVector3D(  0,         0.7500,    1.0000 ),
            QVector3D(  0,         0.8125,    1.0000 ),
            QVector3D(  0,         0.8750,    1.0000 ),
            QVector3D(  0,         0.9375,    1.0000 ),
            QVector3D(  0,         1.0000,    1.0000 ),
            QVector3D(  0.0625,    1.0000,    0.9375 ),
            QVector3D(  0.1250,    1.0000,    0.8750 ),
            QVector3D(  0.1875,    1.0000,    0.8125 ),
            QVector3D(  0.2500,    1.0000,    0.7500 ),
            QVector3D(  0.3125,    1.0000,    0.6875 ),
            QVector3D(  0.3750,    1.0000,    0.6250 ),
            QVector3D(  0.4375,    1.0000,    0.5625 ),
            QVector3D(  0.5000,    1.0000,    0.5000 ),
            QVector3D(  0.5625,    1.0000,    0.4375 ),
            QVector3D(  0.6250,    1.0000,    0.3750 ),
            QVector3D(  0.6875,    1.0000,    0.3125 ),
            QVector3D(  0.7500,    1.0000,    0.2500 ),
            QVector3D(  0.8125,    1.0000,    0.1875 ),
            QVector3D(  0.8750,    1.0000,    0.1250 ),
            QVector3D(  0.9375,    1.0000,    0.0625 ),
            QVector3D(  1.0000,    1.0000,    0  ),
            QVector3D(  1.0000,    0.9375,    0  ),
            QVector3D(  1.0000,    0.8750,    0  ),
            QVector3D(  1.0000,    0.8125,    0  ),
            QVector3D(  1.0000,    0.7500,    0  ),
            QVector3D(  1.0000,    0.6875,    0  ),
            QVector3D(  1.0000,    0.6250,    0  ),
            QVector3D(  1.0000,    0.5625,    0  ),
            QVector3D(  1.0000,    0.5000,    0  ),
            QVector3D(  1.0000,    0.4375,    0  ),
            QVector3D(  1.0000,    0.3750,    0  ),
            QVector3D(  1.0000,    0.3125,    0  ),
            QVector3D(  1.0000,    0.2500,    0  ),
            QVector3D(  1.0000,    0.1875,    0  ),
            QVector3D(  1.0000,    0.1250,    0  ),
            QVector3D(  1.0000,    0.0625,    0  ),
            QVector3D(  1.0000,    0,         0  ),
            QVector3D(  0.9375,    0,         0  ),
            QVector3D(  0.8750,    0,         0  ),
            QVector3D(  0.8125,    0,         0  ),
            QVector3D(  0.7500,    0,         0  ),
            QVector3D(  0.6875,    0,         0  ),
            QVector3D(  0.6250,    0,         0  ),
            QVector3D(  0.5625,    0,         0  ),
            QVector3D(  0.5000,    0,         0  )
          };

}

QVector3D ColorMap::getColor( COLORMAP map, float value, float low, float high ) const
{
    switch ( map ) {

        case COLORMAP::JET:
            return getColor( jet, value, low, high );
            break;
        default:
            return QVector3D( 1.0f, 0.0f, 0.0f );
            break;

    }
}

QVector3D ColorMap::getColor(const std::vector< QVector3D > &map, float value, float low, float high ) const
{
    int ncolors = (int) map.size();

    if( low > high ) std::swap( low, high );

    int index = std::fabs( ( value - low )/ ( high - low )*ncolors );
    index = index % ncolors;

    return map[ index ];

}

QVector3D ColorMap::getConstantColor() const
{
    return QVector3D( 1.0f, 0.0f, 0.0f );
}


std::vector< QVector3D > ColorMap::getColors( COLORMAP cm, unsigned int& nc ) const
{
    if( cm == COLORMAP::CONSTANT )
    {
        nc = 1;
        std::vector< QVector3D > c;
        c.push_back( QVector3D( 1.0f, 0.0f, 0.0f ) );
        return c;
    }
    else if( cm == COLORMAP::JET )
    {
        nc = jet.size();
        return jet;
    }
}
