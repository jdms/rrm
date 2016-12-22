#include "CPS3Exporter.hpp"

CPS3Exporter::CPS3Exporter()
{
	undefValueGrid = "0.1E+31";
}

void CPS3Exporter::setBoundingBox( float xMin, float xMax, float yMin, float yMax, float zMin, float zMax )
{
    maximum[ 0 ] = xMax;
    maximum[ 1 ] = yMax;
    maximum[ 2 ] = zMax;

    minimum[ 0 ] = xMin;
    minimum[ 1 ] = yMin;
    minimum[ 2 ] = zMin;
}

void CPS3Exporter::setVectorValues( const std::vector<float>& vvalues )
{
    if( vvalues.empty() == true ) return;

    values.clear();
    values.resize( vvalues.size() );
    std::copy( vvalues.begin(), vvalues.end(), values.begin() );
}

void CPS3Exporter::setSize( int nlinesI, int nlinesJ )
{
    nI = nlinesI;
    nJ = nlinesJ;
}

void CPS3Exporter::setSpacing( float dx, float dy )
{
    dX = dx;
    dY = dy;
}


void CPS3Exporter::clearData()
{

	maximum[ 0 ] = 0;
	maximum[ 1 ] = 0;

	minimum[ 0 ] = 0;
	minimum[ 1 ] = 0;
	
	
	nI = 0;
    nJ = 0;
	
	dX = 0;
    dY = 0;
	
	values.clear();
	
	
}

bool CPS3Exporter::writeGridData( const std::string& filename ) const
{

    std::ofstream file;
    file.open( filename.c_str() );
    if( !file.is_open() ) return false;
	
	
    ///
    /// writing the header.
    /// the values are distributed by nlinesJ blocks, which contains nlinesI elements distributed by NCOLUMNS columns.
    ///


    if( undefValueGrid.empty() == true )
        return false;



    file << "FSASCI " << "0 1 COMPUTED 0 " << undefValueGrid.c_str() << std::endl;

    file << "FSATTR " << "0 0" << std::endl;

    file  << std::setprecision( 15 ) << "FSLIMI " << minimum[ 0 ] << " " << maximum[ 0 ] << " "
                      << minimum[ 1 ] << " " << maximum[ 1 ] << " "
                      << minimum[ 2 ] << " " << maximum[ 2 ] << std::endl;


    file << "FSNROW " << nJ << " " << nI << std::endl;


    file  << std::setprecision( 15 ) << "FSXINC " << dY << " " << dX << std::endl;


    file << "->MSMODL: Surface of z1\r" << std::endl;



    ///
    /// writing the values.
    /// the values are distributed by nlinesJ blocks, which contains nlinesI elements distributed by NCOLUMNS columns.
    ///


    if( values.empty() == true  )
        return false;


    int int_elemens = (int) nJ / NCOLUMNSGRID;

    float indef = std::atof( undefValueGrid.c_str() );

    int elem = 0;
    for( int j = 0; j < nI; ++j )
    {

        for( int i = 0; i < int_elemens; i++, elem += NCOLUMNSGRID )
        {

            // mudar para avaliar se é valido ou nao

            if( fabs( values[ elem ] - indef ) < FLT_EPSILON )
                file << undefValueGrid.c_str() << " ";
            else
                file << std::setprecision( 15 ) << values[ elem ] << " ";


            if( fabs( values[ elem + 1 ] - indef ) < FLT_EPSILON )
                file << undefValueGrid.c_str() << " ";
            else
                file << std::setprecision( 15 ) << values[ elem + 1 ] << " ";


            if( fabs( values[ elem + 2 ] - indef ) < FLT_EPSILON )
                file << undefValueGrid.c_str() << " ";
            else
                file << std::setprecision( 15 ) << values[ elem + 2 ] << " ";


            if( fabs( values[ elem + 3 ] - indef ) < FLT_EPSILON )
                file << undefValueGrid.c_str() << " ";
            else
                file << std::setprecision( 15 ) << values[ elem + 3 ] << " ";


            if( fabs( values[ elem + 4 ] - indef ) < FLT_EPSILON )
                file << undefValueGrid.c_str();
            else
                file << std::setprecision( 15 ) << values[ elem + 4 ];

            file << std::endl;

        }


        for( int k = int_elemens*NCOLUMNSGRID; k < nJ; ++k, elem++ )
        {
            if( fabs( values[ elem ] - indef ) < FLT_EPSILON )
                file << undefValueGrid.c_str() << " ";
            else
                file << std::setprecision( 15 ) << values[ elem ] << " ";

            if( k == ( nJ - 1 ) )
                file << std::endl;
        }

    }



	file.close();
    return true;
	
	
}
