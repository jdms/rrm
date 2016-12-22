#ifndef CPS3EXPORTER_H
#define CPS3EXPORTER_H

#include <iostream>
#include <istream>
#include <fstream>

#include <QString>


#include <math.h>
#include <iomanip>
#include <float.h>

#include <vector>


class CPS3Exporter
{

    public:


        CPS3Exporter();

        void setBoundingBox(float xMin, float xMax, float yMin, float yMax , float zMin, float zMax );
        void setVectorValues( const std::vector<float>& vvalues );
        void setSize( int nlinesI, int nlinesJ );
        void setSpacing( float dx, float dy );
		
        bool writeGridData( const std::string& filename ) const;
        void clearData();



    protected:

        std::string fileName;               ///< Name of the CPS3 file to be read or write.
        static const int NCOLUMNSGRID = 5;

        int             nI, nJ, nK;

        float           dX, dY, dZ;
        float           maximum[ 3 ], minimum[ 3 ];

        std::string undefValueGrid;         ///< Define the used value to represent undefined values.

        std::vector< float > values;

};

#endif // CPS3EXPORTER_H
