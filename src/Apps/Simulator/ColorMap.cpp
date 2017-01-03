#include "ColorMap.h"


ColorMap::ColorMap()
{

    jet = {
            QVector3D(  0,         0,         0.6667 ),
            QVector3D(  0,         0,         1.0 ),
            QVector3D(  0,         0.333,     1.0 ),
            QVector3D(  0,         0.6667,    1.0 ),
            QVector3D(  0,         1.0,       1.0 ),
            QVector3D(  0.333,     1.0,       0.6667 ),
            QVector3D(  0.6667,    1.0,       0.3333 ),
            QVector3D(  1.0,       1.0,       0.0000 ),
            QVector3D(  1.0,       0.6667,    0.0000 ),
            QVector3D(  1.0,       0.3333,    0.0000 ),
            QVector3D(  1.0,       0.0,       0.0000 ),
            QVector3D(  0.6667,    0.0,       0.0000 )
          };


    hot = { QVector3D( 0.25, 0.0, 0.0 ), QVector3D( 0.5, 0.0, 0.0 ), QVector3D( 0.75, 0.0, 0.0 ), QVector3D( 1.0, 0.0, 0.0 ),
            QVector3D( 1.0, 0.25, 0.0 ), QVector3D( 1.0, 0.5, 0.0 ), QVector3D( 1.0, 0.75, 0.0 ), QVector3D( 1.0, 1.0, 0.0 ),
            QVector3D( 1.0, 1.0, 0.25 ), QVector3D( 1.0, 1.0, 0.5 ), QVector3D( 1.0, 1.0, 0.75 ), QVector3D( 1.0, 1.0, 1.0 ) };

    cool = { QVector3D( 0.0, 1.0, 1.0 ), QVector3D( 0.0909, 0.9091, 1.0 ), QVector3D( 0.1818, 0.8182, 1.0 ), QVector3D( 0.2727, 0.7273, 1.0 ),
             QVector3D( 0.3636, 0.6364, 1.0 ), QVector3D( 0.4545, 0.5455, 1.0 ), QVector3D( 0.5455, 0.4545, 1.0 ), QVector3D( 0.6364, 0.3636, 1.0 ),
             QVector3D( 0.7273, 0.2727, 1.0 ), QVector3D( 0.8182, 0.1818, 1.0 ), QVector3D( 0.9091, 0.0909, 1.0 ), QVector3D( 1.0, 0.0, 1.0 ) };

    parula = {QVector3D(0.2081,0.1663,0.5292), QVector3D(0.1403,0.3147,0.8168), QVector3D(0.0410,0.4502,0.8685) ,
              QVector3D(0.0734,0.5410,0.8257), QVector3D(0.1959,0.2645,0.7279), QVector3D(0.1707,0.2919,0.7792),
              QVector3D(0.1253,0.3242,0.8303), QVector3D(0.0232,0.6407,0.7925), QVector3D(0.1024,0.6984,0.6934) ,
              QVector3D(0.31877,0.7395,0.5625) , QVector3D(0.5745,0.7484,0.4479), QVector3D(0.7798,0.7361,0.3658)  ,
              QVector3D(0.0498,0.4586,0.8641), QVector3D(0.0629,0.4737,0.8554), QVector3D(0.0723,0.4887,0.8467),
              QVector3D(0.9613,0.7281,0.2774) , QVector3D(0.9763,0.8328,0.1590)  , QVector3D(0.9763,0.9831,0.0538) };


    spring = { QVector3D( 1.0, 0.0, 1.0 ), QVector3D( 1.0, 0.0909, 0.9091 ), QVector3D( 1.0, 0.1818, 0.8182 ), QVector3D( 1.0, 0.2727, 0.7273 ),
               QVector3D( 1.0, 0.3636, 0.6364 ), QVector3D( 1.0, 0.4545, 0.5455 ), QVector3D( 1.0, 0.5455, 0.4545 ), QVector3D( 1.0, 0.6364, 0.3636 ),
               QVector3D( 1.0, 0.7273, 0.2727 ), QVector3D( 1.0, 0.8182, 0.1818 ), QVector3D( 1.0, 0.9091, 0.0909 ), QVector3D( 1.0, 1.0, 0.0 ) };

    summer  = { QVector3D( 0.0, 0.5, 0.4 ), QVector3D( 0.0909, 0.5455, 0.4 ), QVector3D( 0.1818, 0.5909, 0.4 ), QVector3D( 0.2727, 0.6364, 0.4 ),
                QVector3D( 0.3636, 0.6818, 0.4 ), QVector3D( 0.4545, 0.7273, 0.4 ), QVector3D( 0.5455, 0.7727, 0.4 ), QVector3D( 0.6364, 0.8182, 0.4 ),
                QVector3D( 0.7273, 0.8636, 0.4 ), QVector3D( 0.8182, 0.9091, 0.4 ), QVector3D( 0.9091, 0.9545, 0.4 ), QVector3D( 1.0, 1.0, 0.4 ) };

    copper = { QVector3D( 0.0, 0.0, 0.0 ), QVector3D( 0.1136, 0.0710, 0.0452 ), QVector3D( 0.2273, 0.1420, 0.0905 ), QVector3D( 0.3409, 0.2131, 0.1357 ),
               QVector3D( 0.4545, 0.2841, 0.1809 ), QVector3D( 0.5682, 0.3551, 0.2261 ), QVector3D( 0.6818, 0.4261, 0.2714 ), QVector3D( 0.7955, 0.4971, 0.3166 ),
               QVector3D( 0.9091, 0.5681, 0.3618 ), QVector3D( 1.0, 0.6392, 0.4070 ), QVector3D( 1.0, 0.7102, 0.4523 ), QVector3D( 1.0, 0.7812, 0.49750 ) };

    polar = { QVector3D( 0.0, 1.0, 1.0 ), QVector3D( 0.0909, 0.9091, 1.0 ), QVector3D( 0.1818, 0.8182, 1.0 ), QVector3D( 0.2727, 0.7273, 1.0 ),
              QVector3D( 0.3636, 0.6364, 1.0 ), QVector3D( 0.4545, 0.5455, 1.0 ), QVector3D( 0.5455, 0.4545, 1.0 ), QVector3D( 0.6364, 0.3636, 1.0 ),
              QVector3D( 0.7273, 0.2727, 1.0 ), QVector3D( 0.8182, 0.1818, 1.0 ), QVector3D( 0.9091, 0.0909, 1.0 ), QVector3D( 1.0, 0.0, 1.0 ) };

    winter = { QVector3D( 0.0, 0.0, 1.0 ), QVector3D( 0.0, 0.0909, 0.9545 ), QVector3D( 0.0, 0.1818, 0.9091 ), QVector3D( 0.0, 0.2727, 0.8636 ),
               QVector3D( 0.0, 0.3636, 0.8182 ), QVector3D( 0.0, 0.4545, 0.7727 ), QVector3D( 0.0, 0.5455, 0.7273 ), QVector3D( 0.0, 0.6364, 0.6818 ),
               QVector3D( 0.0, 0.7273, 0.6364 ), QVector3D( 0.0, 0.8182, 0.5909 ), QVector3D( 0.0, 0.9091, 0.5455 ), QVector3D( 0.0, 1.0, 0.5 ) };

	/// @see http://colorbrewer2.org/#type=qualitative&scheme=Set3&n=12
	qualitative = {
		QVector3D(41.0f / 255.0f, 211.0f / 255.0f, 199.0f / 255.0f),
		QVector3D(255.0f / 255.0f, 255.0f / 255.0f, 179.0f / 255.0f),
		QVector3D(190.0f / 255.0f, 186.0f / 255.0f, 218.0f / 255.0f),
		QVector3D(251.0f / 255.0f, 128.0f / 255.0f, 114.0f / 255.0f),
		QVector3D(128.0f / 255.0f, 177.0f / 255.0f, 211.0f / 255.0f),
		QVector3D(253.0f / 255.0f, 180.0f / 255.0f, 98.0f / 255.0f),
		QVector3D(179.0f / 255.0f, 222.0f / 255.0f, 105.0f / 255.0f),
		QVector3D(252.0f / 255.0f, 205.0f / 255.0f, 229.0f / 255.0f),
		QVector3D(217.0f / 255.0f, 217.0f / 255.0f, 217.0f / 255.0f),
		QVector3D(188.0f / 255.0f, 128.0f / 255.0f, 189.0f / 255.0f),
		QVector3D(204.0f / 255.0f, 235.0f / 255.0f, 197.0f / 255.0f),
		QVector3D(255.0f / 255.0f, 237.0f / 255.0f, 111.0f / 255.0)
	};

}


QVector3D ColorMap::getColor( COLORMAP map, float value, float low, float high ) const
{
    switch ( map ) {

        case COLORMAP::JET:
            return getColor( jet, value, low, high );
            break;
        case COLORMAP::HOT:
            return getColor( hot, value, low, high );
            break;
        case COLORMAP::COOL:
            return getColor( cool, value, low, high );
            break;
        case COLORMAP::PARULA:
            return getColor( parula, value, low, high );
            break;
        case COLORMAP::SPRING:
            return getColor( spring, value, low, high );
            break;
        case COLORMAP::SUMMER:
            return getColor( summer, value, low, high );
            break;
        case COLORMAP::COPPER:
            return getColor( copper, value, low, high );
            break;
        case COLORMAP::POLAR:
            return getColor( polar, value, low, high );
            break;
        case COLORMAP::WINTER:
            return getColor( winter, value, low, high );
            break;
		case COLORMAP::QUALITATIVE:
			return getColor(qualitative, value, low, high);
			break;
        default:
            return QVector3D( 0.85f, 0.85f, 0.85f );
            break;

    }
}


QVector3D ColorMap::getColor(const std::vector< QVector3D > &map, float value, float low, float high ) const
{
    int ncolors = (int) map.size() - 1;

    if( low > high ) std::swap( low, high );


    int index = 0;

    if( high - low > FLT_EPSILON )
        index = std::fabs( ( value - low )/ ( high - low ) )*( ncolors );


    return map[ index ];

}


QVector3D ColorMap::getConstantColor() const
{
    return QVector3D( 0.85f, 0.85f, 0.85f );
}


std::vector< QVector3D > ColorMap::getColors( COLORMAP cm, unsigned int& nc ) const
{

    switch ( cm ) {

        case COLORMAP::JET:
        {
            nc = jet.size();
            return jet;
        }    break;
        case COLORMAP::HOT:
        {
            nc = hot.size();
            return hot;
        }    break;
        case COLORMAP::COOL:
        {
            nc = cool.size();
            return cool;
        }    break;
        case COLORMAP::PARULA:
        {
            nc = parula.size();
            return parula;
        }    break;
        case COLORMAP::SPRING:
        {
            nc = spring.size();
            return spring;
        }    break;
        case COLORMAP::SUMMER:
        {
            nc = summer.size();
            return summer;
        }    break;
        case COLORMAP::COPPER:
        {
            nc = copper.size();
            return copper;
        }    break;
        case COLORMAP::POLAR:
        {
            nc = polar.size();
            return polar;
        } break;
        case COLORMAP::WINTER:
        {
            nc = winter.size();
            return winter;
        } break;
		case COLORMAP::QUALITATIVE:
		{
			nc = qualitative.size();
			return qualitative;
		} break;
        default:
        {
            nc = 1;
            std::vector< QVector3D > c;
            c.push_back( QVector3D( 0.85f, 0.85f, 0.85f ) );
            return c;
         }break;

    }

}
