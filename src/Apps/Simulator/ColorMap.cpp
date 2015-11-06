#include "../../Apps/Simulator/ColorMap.h"

ColorMap::ColorMap()
{

    for ( int i = 0; i < 64; ++i )
        _jet.push_back( QVector4D( jet[i][0], jet[i][1],  jet[i][2], 1.0f) );

}

QVector4D ColorMap::getColor( COLORMAP map, float value, float low, float high ) const
{
    switch ( map ) {

        case COLORMAP::JET:
            return getColor( _jet, value, low, high );
            break;
        default:
            return QVector4D( 0.0f, 0.0, 1.0f, 1.0f );
            break;

    }
}

QVector4D ColorMap::getColor(const std::vector< QVector4D > &map, float value, float low, float high ) const
{
    int ncolors = (int) map.size();

    if( low > high ) std::swap( low, high );

    int index = std::fabs( ( value - low )/ ( high - low )*ncolors );
    index = index % ncolors;

    return map[ index ];

}
