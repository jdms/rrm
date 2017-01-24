#include "ColorMap.h"


ColorMap::ColorMap()
{
	cool_to_warm =
	{
		QVector3D(59.0f / 255.0f, 76.0f / 255.0f, 192.0f / 255.0f),
		//QVector3D(68.0f / 255.0f, 90.0f / 255.0f, 204.0f / 255.0f),
		QVector3D(77.0f / 255.0f, 104.0f / 255.0f, 215.0f / 255.0f),
		//QVector3D(87.0f / 255.0f, 117.0f / 255.0f, 225.0f / 255.0f),
		QVector3D(98.0f / 255.0f, 130.0f / 255.0f, 234.0f / 255.0f),
		//QVector3D(108.0f / 255.0f, 142.0f / 255.0f, 241.0f / 255.0f),
		QVector3D(119.0f / 255.0f, 154.0f / 255.0f, 247.0f / 255.0f),
		//QVector3D(130.0f / 255.0f, 165.0f / 255.0f, 251.0f / 255.0f),
		QVector3D(141.0f / 255.0f, 176.0f / 255.0f, 254.0f / 255.0f),
		//QVector3D(152.0f / 255.0f, 185.0f / 255.0f, 255.0f / 255.0f),
		QVector3D(163.0f / 255.0f, 194.0f / 255.0f, 255.0f / 255.0f),
		//QVector3D(174.0f / 255.0f, 201.0f / 255.0f, 253.0f / 255.0f),
		QVector3D(184.0f / 255.0f, 208.0f / 255.0f, 249.0f / 255.0f),
		//QVector3D(194.0f / 255.0f, 213.0f / 255.0f, 244.0f / 255.0f),
		QVector3D(204.0f / 255.0f, 217.0f / 255.0f, 238.0f / 255.0f),
		//QVector3D(213.0f / 255.0f, 219.0f / 255.0f, 230.0f / 255.0f),
		QVector3D(221.0f / 255.0f, 221.0f / 255.0f, 221.0f / 255.0f),
		//QVector3D(229.0f / 255.0f, 216.0f / 255.0f, 209.0f / 255.0f),
		QVector3D(236.0f / 255.0f, 211.0f / 255.0f, 197.0f / 255.0f),
		//QVector3D(241.0f / 255.0f, 204.0f / 255.0f, 185.0f / 255.0f),
		QVector3D(245.0f / 255.0f, 196.0f / 255.0f, 173.0f / 255.0f),
		//QVector3D(247.0f / 255.0f, 187.0f / 255.0f, 160.0f / 255.0f),
		QVector3D(247.0f / 255.0f, 177.0f / 255.0f, 148.0f / 255.0f),
		//QVector3D(247.0f / 255.0f, 166.0f / 255.0f, 135.0f / 255.0f),
		QVector3D(244.0f / 255.0f, 154.0f / 255.0f, 123.0f / 255.0f),
		//QVector3D(241.0f / 255.0f, 141.0f / 255.0f, 111.0f / 255.0f),
		QVector3D(236.0f / 255.0f, 127.0f / 255.0f, 99.0f / 255.0f),
		//QVector3D(229.0f / 255.0f, 112.0f / 255.0f, 88.0f / 255.0f),
		QVector3D(222.0f / 255.0f, 96.0f / 255.0f, 77.0f / 255.0f),
		//QVector3D(213.0f / 255.0f, 80.0f / 255.0f, 66.0f / 255.0f),
		QVector3D(203.0f / 255.0f, 62.0f / 255.0f, 56.0f / 255.0f),
		//QVector3D(192.0f / 255.0f, 40.0f / 255.0f, 47.0f / 255.0f),
		QVector3D(180.0f / 255.0f, 4.0f / 255.0f, 38.0f / 255.0f)
	};

	hot = { QVector3D(0.25, 0.0, 0.0), QVector3D(0.5, 0.0, 0.0), QVector3D(0.75, 0.0, 0.0), QVector3D(1.0, 0.0, 0.0),
		QVector3D(1.0, 0.25, 0.0), QVector3D(1.0, 0.5, 0.0), QVector3D(1.0, 0.75, 0.0), QVector3D(1.0, 1.0, 0.0),
		QVector3D(1.0, 1.0, 0.25), QVector3D(1.0, 1.0, 0.5), QVector3D(1.0, 1.0, 0.75), QVector3D(1.0, 1.0, 1.0) };

	cool = { QVector3D(0.0, 1.0, 1.0), QVector3D(0.0909, 0.9091, 1.0), QVector3D(0.1818, 0.8182, 1.0), QVector3D(0.2727, 0.7273, 1.0),
		QVector3D(0.3636, 0.6364, 1.0), QVector3D(0.4545, 0.5455, 1.0), QVector3D(0.5455, 0.4545, 1.0), QVector3D(0.6364, 0.3636, 1.0),
		QVector3D(0.7273, 0.2727, 1.0), QVector3D(0.8182, 0.1818, 1.0), QVector3D(0.9091, 0.0909, 1.0), QVector3D(1.0, 0.0, 1.0) };

	parula = { QVector3D(0.2081, 0.1663, 0.5292), QVector3D(0.1403, 0.3147, 0.8168), QVector3D(0.0410, 0.4502, 0.8685),
		QVector3D(0.0734, 0.5410, 0.8257), QVector3D(0.1959, 0.2645, 0.7279), QVector3D(0.1707, 0.2919, 0.7792),
		QVector3D(0.1253, 0.3242, 0.8303), QVector3D(0.0232, 0.6407, 0.7925), QVector3D(0.1024, 0.6984, 0.6934),
		QVector3D(0.31877, 0.7395, 0.5625), QVector3D(0.5745, 0.7484, 0.4479), QVector3D(0.7798, 0.7361, 0.3658),
		QVector3D(0.0498, 0.4586, 0.8641), QVector3D(0.0629, 0.4737, 0.8554), QVector3D(0.0723, 0.4887, 0.8467),
		QVector3D(0.9613, 0.7281, 0.2774), QVector3D(0.9763, 0.8328, 0.1590), QVector3D(0.9763, 0.9831, 0.0538) };


	spring = { QVector3D(1.0, 0.0, 1.0), QVector3D(1.0, 0.0909, 0.9091), QVector3D(1.0, 0.1818, 0.8182), QVector3D(1.0, 0.2727, 0.7273),
		QVector3D(1.0, 0.3636, 0.6364), QVector3D(1.0, 0.4545, 0.5455), QVector3D(1.0, 0.5455, 0.4545), QVector3D(1.0, 0.6364, 0.3636),
		QVector3D(1.0, 0.7273, 0.2727), QVector3D(1.0, 0.8182, 0.1818), QVector3D(1.0, 0.9091, 0.0909), QVector3D(1.0, 1.0, 0.0) };

	summer = { QVector3D(0.0, 0.5, 0.4), QVector3D(0.0909, 0.5455, 0.4), QVector3D(0.1818, 0.5909, 0.4), QVector3D(0.2727, 0.6364, 0.4),
		QVector3D(0.3636, 0.6818, 0.4), QVector3D(0.4545, 0.7273, 0.4), QVector3D(0.5455, 0.7727, 0.4), QVector3D(0.6364, 0.8182, 0.4),
		QVector3D(0.7273, 0.8636, 0.4), QVector3D(0.8182, 0.9091, 0.4), QVector3D(0.9091, 0.9545, 0.4), QVector3D(1.0, 1.0, 0.4) };

	copper = { QVector3D(0.0, 0.0, 0.0), QVector3D(0.1136, 0.0710, 0.0452), QVector3D(0.2273, 0.1420, 0.0905), QVector3D(0.3409, 0.2131, 0.1357),
		QVector3D(0.4545, 0.2841, 0.1809), QVector3D(0.5682, 0.3551, 0.2261), QVector3D(0.6818, 0.4261, 0.2714), QVector3D(0.7955, 0.4971, 0.3166),
		QVector3D(0.9091, 0.5681, 0.3618), QVector3D(1.0, 0.6392, 0.4070), QVector3D(1.0, 0.7102, 0.4523), QVector3D(1.0, 0.7812, 0.49750) };

	polar = { QVector3D(0.0, 1.0, 1.0), QVector3D(0.0909, 0.9091, 1.0), QVector3D(0.1818, 0.8182, 1.0), QVector3D(0.2727, 0.7273, 1.0),
		QVector3D(0.3636, 0.6364, 1.0), QVector3D(0.4545, 0.5455, 1.0), QVector3D(0.5455, 0.4545, 1.0), QVector3D(0.6364, 0.3636, 1.0),
		QVector3D(0.7273, 0.2727, 1.0), QVector3D(0.8182, 0.1818, 1.0), QVector3D(0.9091, 0.0909, 1.0), QVector3D(1.0, 0.0, 1.0) };

	winter = { QVector3D(0.0, 0.0, 1.0), QVector3D(0.0, 0.0909, 0.9545), QVector3D(0.0, 0.1818, 0.9091), QVector3D(0.0, 0.2727, 0.8636),
		QVector3D(0.0, 0.3636, 0.8182), QVector3D(0.0, 0.4545, 0.7727), QVector3D(0.0, 0.5455, 0.7273), QVector3D(0.0, 0.6364, 0.6818),
		QVector3D(0.0, 0.7273, 0.6364), QVector3D(0.0, 0.8182, 0.5909), QVector3D(0.0, 0.9091, 0.5455), QVector3D(0.0, 1.0, 0.5) };

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



	jet = {
		QVector3D(0.0f, 0.0f, 0.6667f),
		QVector3D(0.0f, 0.0f, 1.0f),
		QVector3D(0.0f, 0.333f, 1.0f),
		QVector3D(0.0f, 0.6667f, 1.0f),
		QVector3D(0.0f, 1.0f, 1.0f),
		QVector3D(0.333f, 1.0f, 0.6667f),
		QVector3D(0.6667f, 1.0f, 0.3333f),
		QVector3D(1.0f, 1.0f, 0.0000f),
		QVector3D(1.0f, 0.6667f, 0.0000f),
		QVector3D(1.0f, 0.3333f, 0.0000f),
		QVector3D(1.0, 0.0f, 0.0000f),
		QVector3D(0.6667f, 0.0f, 0.0000f)
	};

}


QVector3D ColorMap::getColor(COLORMAP map, double value, double low, double high) const
{
    switch ( map ) {

		case COLORMAP::COOL_TO_WARM:
			return getColor(cool_to_warm, value, low, high);
		break;
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


QVector3D ColorMap::getColor(const std::vector< QVector3D > &map, double value, double low, double high) const
{
    int ncolors = (int) map.size() - 1;

	if (low > high)
	{
		std::swap(low, high);
	}


    int index = 0;
	/// Preventing division by Zero
	if (high - low > std::numeric_limits<double>::epsilon() )
	{
		index = static_cast<int>(std::fabs((value - low) / (high - low))*( static_cast<double>(ncolors) ));
	}


    return map[ index ];

}


QVector3D ColorMap::getConstantColor() const
{
    return QVector3D( 0.85f, 0.85f, 0.85f );
}


std::vector< QVector3D > ColorMap::getColors( COLORMAP cm, unsigned int& nc ) const
{

    switch ( cm ) {

		case COLORMAP::COOL_TO_WARM:
		{
			nc = cool_to_warm.size();
			return cool_to_warm;
		}    break;
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
