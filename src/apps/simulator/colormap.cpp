/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "colormap.h"


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

    hot = { QVector3D(0.25f, 0.0f, 0.0f), QVector3D(0.5f, 0.0f, 0.0f), QVector3D(0.75f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f),
            QVector3D(1.0f, 0.25f, 0.0f), QVector3D(1.0f, 0.5f, 0.0f), QVector3D(1.0f, 0.75f, 0.0f), QVector3D(1.0f, 1.0f, 0.0f),
            QVector3D(1.0f, 1.0f, 0.25f), QVector3D(1.0f, 1.0f, 0.5f), QVector3D(1.0f, 1.0f, 0.75f), QVector3D(1.0f, 1.0f, 1.0f) };

    cool = { QVector3D(0.0f, 1.0f, 1.0f), QVector3D(0.0909f, 0.9091f, 1.0f), QVector3D(0.1818f, 0.8182f, 1.0f), QVector3D(0.2727f, 0.7273f, 1.0f),
             QVector3D(0.3636f, 0.6364f, 1.0f), QVector3D(0.4545f, 0.5455f, 1.0f), QVector3D(0.5455f, 0.4545f, 1.0f), QVector3D(0.6364f, 0.3636f, 1.0f),
             QVector3D(0.7273f, 0.2727f, 1.0f), QVector3D(0.8182f, 0.1818f, 1.0f), QVector3D(0.9091f, 0.0909f, 1.0f), QVector3D(1.0f, 0.0f, 1.0f) };

    parula = { QVector3D(0.2081f, 0.1663f, 0.5292f), QVector3D(0.1403f, 0.3147f, 0.8168f), QVector3D(0.0410f, 0.4502f, 0.8685f ),
               QVector3D(0.0734f, 0.5410f, 0.8257f), QVector3D(0.1959f, 0.2645f, 0.7279f), QVector3D(0.1707f, 0.2919f, 0.7792f ),
               QVector3D(0.1253f, 0.3242f, 0.8303f), QVector3D(0.0232f, 0.6407f, 0.7925f), QVector3D(0.1024f, 0.6984f, 0.6934f ),
               QVector3D(0.31877f, 0.7395f, 0.5625f), QVector3D(0.5745f, 0.7484f, 0.4479f), QVector3D(0.7798f, 0.7361f, 0.3658f),
               QVector3D(0.0498f, 0.4586f, 0.8641f), QVector3D(0.0629f, 0.4737f, 0.8554f), QVector3D(0.0723f, 0.4887f, 0.8467f),
               QVector3D(0.9613f, 0.7281f, 0.2774f), QVector3D(0.9763f, 0.8328f, 0.1590f), QVector3D(0.9763f, 0.9831f, 0.0538f) };


    spring = { QVector3D(1.0f, 0.0f, 1.0f), QVector3D(1.0f, 0.0909f, 0.9091f), QVector3D(1.0f, 0.1818f, 0.8182f), QVector3D(1.0f, 0.2727f, 0.7273f),
               QVector3D(1.0f, 0.3636f, 0.6364f), QVector3D(1.0f, 0.4545f, 0.5455f), QVector3D(1.0f, 0.5455f, 0.4545f), QVector3D(1.0f, 0.6364f, 0.3636f),
               QVector3D(1.0f, 0.7273f, 0.2727f), QVector3D(1.0f, 0.8182f, 0.1818f), QVector3D(1.0f, 0.9091f, 0.0909f), QVector3D(1.0f, 1.0f, 0.0f) };

    summer = { QVector3D(0.0f, 0.5f, 0.4f), QVector3D(0.0909f, 0.5455f, 0.4f), QVector3D(0.1818f, 0.5909f, 0.4f), QVector3D(0.2727f, 0.6364f, 0.4f),
               QVector3D(0.3636f, 0.6818f, 0.4f), QVector3D(0.4545f, 0.7273f, 0.4f), QVector3D(0.5455f, 0.7727f, 0.4f), QVector3D(0.6364f, 0.8182f, 0.4f),
               QVector3D(0.7273f, 0.8636f, 0.4f), QVector3D(0.8182f, 0.9091f, 0.4f), QVector3D(0.9091f, 0.9545f, 0.4f), QVector3D(1.0f, 1.0f, 0.4f) };

    copper = { QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.1136f, 0.0710f, 0.0452f), QVector3D(0.2273f, 0.1420f, 0.0905f), QVector3D(0.3409f, 0.2131f, 0.1357f),
               QVector3D(0.4545f, 0.2841f, 0.1809f), QVector3D(0.5682f, 0.3551f, 0.2261f), QVector3D(0.6818f, 0.4261f, 0.2714f), QVector3D(0.7955f, 0.4971f, 0.3166f),
               QVector3D(0.9091f, 0.5681f, 0.3618f), QVector3D(1.0f, 0.6392f, 0.4070f), QVector3D(1.0f, 0.7102f, 0.4523f), QVector3D(1.0f, 0.7812f, 0.49750f) };

    polar = { QVector3D(0.0f, 1.0f, 1.0f), QVector3D(0.0909f, 0.9091f, 1.0f), QVector3D(0.1818f, 0.8182f, 1.0f), QVector3D(0.2727f, 0.7273f, 1.0f),
              QVector3D(0.3636f, 0.6364f, 1.0f), QVector3D(0.4545f, 0.5455f, 1.0f), QVector3D(0.5455f, 0.4545f, 1.0f), QVector3D(0.6364f, 0.3636f, 1.0f),
              QVector3D(0.7273f, 0.2727f, 1.0f), QVector3D(0.8182f, 0.1818f, 1.0f), QVector3D(0.9091f, 0.0909f, 1.0f), QVector3D(1.0f, 0.0f, 1.0f) };

    winter = { QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 0.0909f, 0.9545f), QVector3D(0.0f, 0.1818f, 0.9091f), QVector3D(0.0f, 0.2727f, 0.8636f),
               QVector3D(0.0f, 0.3636f, 0.8182f), QVector3D(0.0f, 0.4545f, 0.7727f), QVector3D(0.0f, 0.5455f, 0.7273f), QVector3D(0.0f, 0.6364f, 0.6818f),
               QVector3D(0.0f, 0.7273f, 0.6364f), QVector3D(0.0f, 0.8182f, 0.5909f), QVector3D(0.0f, 0.9091f, 0.5455f), QVector3D(0.0f, 1.0f, 0.5f) };

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
        QVector3D(255.0f / 255.0f, 237.0f / 255.0f, 111.0f / 255.0f)
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
    case COLORMAP::JET:
        return getColor( jet, value, low, high );
    case COLORMAP::HOT:
        return getColor( hot, value, low, high );
    case COLORMAP::COOL:
        return getColor( cool, value, low, high );
    case COLORMAP::SPRING:
        return getColor( spring, value, low, high );
    case COLORMAP::SUMMER:
        return getColor( summer, value, low, high );
    case COLORMAP::COPPER:
        return getColor( copper, value, low, high );
    case COLORMAP::POLAR:
        return getColor( polar, value, low, high );
    case COLORMAP::WINTER:
        return getColor( winter, value, low, high );
    case COLORMAP::QUALITATIVE:
        return getColor(qualitative, value, low, high);
    default:
        return QVector3D( 0.85f, 0.85f, 0.85f );

    }
}


QVector3D ColorMap::getColor(const std::vector< QVector3D > &map, double value, double low, double high) const
{
    int ncolors = static_cast< int >( map.size() ) - 1;

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


    return map[ static_cast<std::size_t>( index ) ];

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
        nc = static_cast< unsigned int > ( cool_to_warm.size() );
        return cool_to_warm;
    }
    case COLORMAP::JET:
    {
        nc = static_cast< unsigned int > ( jet.size() );
        return jet;
    }
    case COLORMAP::HOT:
    {
        nc = static_cast< unsigned int > ( hot.size() );
        return hot;
    }
    case COLORMAP::COOL:
    {
        nc = static_cast< unsigned int > ( cool.size() );
        return cool;
    }
    case COLORMAP::SPRING:
    {
        nc = static_cast< unsigned int > ( spring.size() );
        return spring;
    }
    case COLORMAP::SUMMER:
    {
        nc = static_cast< unsigned int > ( summer.size() );
        return summer;
    }
    case COLORMAP::COPPER:
    {
        nc = static_cast< unsigned int > ( copper.size() );
        return copper;
    }
    case COLORMAP::POLAR:
    {
        nc = static_cast< unsigned int > ( polar.size() );
        return polar;
    }
    case COLORMAP::WINTER:
    {
        nc = static_cast< unsigned int > ( winter.size() );
        return winter;
    }
    case COLORMAP::QUALITATIVE:
    {
        nc = static_cast< unsigned int > ( qualitative.size() );
        return qualitative;
    }
    default:
    {
        nc = 1;
        std::vector< QVector3D > c;
        c.push_back( QVector3D( 0.85f, 0.85f, 0.85f ) );
        return c;
    }

    }

}
