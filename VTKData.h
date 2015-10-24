#ifndef VTKDATA_H
#define VTKDATA_H


#include <iostream>
#include <istream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <float.h>


#include <QString>
#include <QRegExp>
#include <QRegularExpression>
#include <QStringList>



using namespace std;

class VTKData
{
    public:

        enum CODESHAPE { TETRAHEDRON = 10 };


        VTKData();

        bool readUnstructuredGridFile( std::string& filename );

        void getPoints( vector< float >& points ) const;
        void getCells( vector< int >& shape, vector< int >& index ) const;
        void getAttributesPoints( vector< std::string > format, vector< std::string > name, vector< float > values ) const;
        void getAttributesCells( vector< std::string > format, vector< std::string > name, vector< float > values ) const;



    protected:

        enum  class UNSTRUCTUREDGRID_STATES  { HEADER0 /* = Begin */, HEADER1, HEADER2, DATASET, DATATYPE, DATAVALUES, CELLINF, CELLINFVALUES, CELLTYPES, CELLTYPEVALUES, ATTRIBUTEDATA, ATTRIBUTETYPE, LOOKUPTABLE, ATTRIBUTEVALUES, CELLDATAINF, ATTRIBUTECELL, LOOKUPTABLECELL, ATTRIBUTECELVALUES /* = End */ };
        enum  UNSTRUCTUREDGRID_NARGUMENTS   { NHEADER0 = 1, NHEADER2 = 1, NDATASET = 1, NDATATYPE = 2, NDATAVALUES = 1, NCELLINF = 2, NCELLTYPES = 1, NATTRIBUTEDATA = 1, NATTRIBUTETYPE = 3, NLOOKUPTABLE = 1, NCELLDATAINF = 1, NATTRIBUTECELL = 2/* = End */ };



        bool getHeader( QRegExp RegExp, QString line, vector< std::string >& data );
        bool getValues( QRegExp RegExp, QString line, vector< std::string > &data );

        bool parseVTKData( UNSTRUCTUREDGRID_STATES& state, QString line );
        UNSTRUCTUREDGRID_STATES nextState( std::string type );


    private:

        std::string file_version;
        std::string file_comments;
        std::string file_format;
        std::string dataset_type;
        std::string data_type;
        std::string data_nvalues;
        std::string data_format;
        std::string cells_number;
        std::string listcells_size;
        std::string cell_types;
        std::string attributes_number;
        vector< std::string > attribute_type;
        vector< std::string > attribute_name;
        vector< std::string > attribute_format;
        vector< std::string > lookuptable;
        std::string cells_number_data;
        vector< std::string > attributecell_type;
        vector< std::string > attributecell_name;
        vector< std::string > attributecell_format;
        vector< std::string > lookuptablecell;

        vector< float > data_values;
        vector< int > cells_values;
        vector< int > celltype_values;
        vector< float > attribute_values;
        vector< float > attributecell_values;



        int total_datavalues;



};

#endif // VTKDATA_H
