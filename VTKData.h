#ifndef VTKDATA_H
#define VTKDATA_H


#include <iostream>
#include <istream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <float.h>
#include <ostream>


#include <QString>
#include <QRegExp>
#include <QRegularExpression>
#include <QStringList>

#include <FlowProperty.h>

using namespace std;

class VTKData
{



public:

    struct Cell
    {
        int id;
        int type;
        vector< int > vertices;
    };


    VTKData();

    bool readUnstructuredGridFile( std::string& filename );


    inline void setFileVersion( std::string version ){ file_version = version; }
    inline const std::string getFileVersion(){ return file_version; }

    inline void setFileComment( std::string text ){ file_comments = text; }
    inline const std::string getFileComment(){ return file_comments; }

    inline void setFileFormat( std::string format ){ file_format = format; }
    inline const std::string getFileFormat(){ return file_format; }

    inline void setDataSet( std::string data ){ dataset_type = data; }
    inline const std::string getDataSet(){ return dataset_type; }

    inline void setFormatPoints( std::string format ){ format_points = format; }
    inline const std::string getFormatPoints(){ return format_points; }

    inline void setNumberofPoints( int np ){ number_of_points = np; }
    inline const int getNumberofPoints(){ return number_of_points; }

    inline void setNumberofCells( int nc ){ number_of_cells = nc; }
    inline const int getNumberofCells(){ return number_of_cells; }

    inline void setSizeofCellsList( int sl ){ list_cells_size = sl; }
    inline const int getSizeofCellsList(){ return list_cells_size; }


    inline void addVectorPoint( float x, float y, float z ){  vector_points.push_back( x );vector_points.push_back( y );vector_points.push_back( z );}
    inline void setVectorPoints( vector< float > points ){  vector_points = points; }
    inline const void getVectorPoints( vector< float >& points ){  points = vector_points; }


    inline void setPoint( int id, float x, float y, float z )
    {
        int size_vector = (int) vector_points.size();
        int npoints = size_vector/3;

        if( vector_points.empty() == true || id >= npoints ) return;

        vector_points[ 3*id ] = x;
        vector_points[ 3*id + 1 ] = y;
        vector_points[ 3*id + 2 ] = z;

    }
    inline const void getPoint( int id, float& x, float& y, float& z )
    {
        int size_vector = (int) vector_points.size();
        int npoints = size_vector/3;

        if( vector_points.empty() == true || id >= npoints ) return;

        x = vector_points[ 3*id ];
        y = vector_points[ 3*id + 1 ];
        z = vector_points[ 3*id + 2 ];
    }

    inline void setVectorCells( vector< Cell > cells ){  vector_cells = cells; }
    inline const void getVectorCells( vector< Cell >& cells ){  cells = vector_cells; }


    void addCell( int id, int type, vector< int >& vertices );
    inline void setCell( int id, Cell  cell )
    {
        if( vector_cells.empty() == true || id >= vector_cells.size() ) return;

        vector_cells[ id ] = cell;
    }
    inline const void getCell( int id, Cell& cell )
    {
        if( vector_cells.empty() == true || id >= vector_cells.size() ) return;
        cell = vector_cells[ id ];
    }


    inline int getNumberofPointsProperties(){ return (int) vector_point_properties.size(); }
    inline int getNumberofCellsProperties(){ return (int) vector_cell_properties.size(); }

    inline void setPointsFlowProperties( vector< FlowProperty > properties ){  vector_point_properties = properties; }
    inline const void getPointsFlowProperties( vector< FlowProperty >& properties ){  properties = vector_point_properties; }

    inline void setCellsFlowProperties( vector< FlowProperty > properties ){  vector_cell_properties = properties; }
    inline const void getCellsFlowProperties( vector< FlowProperty >& properties ){  properties = vector_cell_properties; }


    inline void addPointFlowProperty( FlowProperty& property )
    {
        vector_point_properties.push_back( property );
    }
    inline void addCellFlowProperty( FlowProperty& property )
    {
        vector_cell_properties.push_back( property );
    }


    inline void setPointFlowProperty( int id, FlowProperty property )
    {
        if( vector_point_properties.empty() == true || id >= vector_point_properties.size() ) return;
        vector_point_properties[ id ] = property;
    }
    inline const void getPointFlowProperty( int id, FlowProperty& property )
    {
        if( vector_point_properties.empty() == true || id >= vector_point_properties.size() ) return;
        property = vector_point_properties[ id ];
    }


    inline void setCellFlowProperty( int id, FlowProperty property )
    {
        if( vector_cell_properties.empty() == true || id >= vector_cell_properties.size() ) return;
        vector_cell_properties[ id ] = property;
    }
    inline const void getCellFlowProperty( int id, FlowProperty& property )
    {
        if( vector_cell_properties.empty() == true || id >= vector_cell_properties.size() ) return;
        property = vector_cell_properties[ id ];
    }

    void getFlowProperty( std::string name, std::string type, FlowProperty& property );

    void computeMaxMinProperties();
    void computeMaxMinPointProperty( int id );
    void computeMaxMinCellProperty( int id );

    void getMaxMinMagnitudePointProperty( int id, vector< float >& maxmin );
    void getMaxMinCoordinatePointProperty( int id, vector< float >& maxmin );
    void getMaxMinMagnitudeCellProperty( int id, vector< float >& maxmin );
    void getMaxMinCoordinateCellProperty( int id, vector< float >& maxmin );

    bool isEmpty();

    void writeFile( ofstream& file ) const;


protected:



    enum  class FILE_STATES  { HEADER0 /* = Begin */, HEADER1, HEADER2, DATASET, DATASET_VALUES, POINTSINF, POINTS_VALUES, CELLSINF, CELLS_VALUES, CELLSTYPES, CELLSTYPES_VALUES, ATTRIBUTEDATA, POINTS_PROPERTIES, POINTS_PROPERTIES_TYPE, LOOKUPTABLE, POINTS_PROPERTIES_VALUES, CELLS_PROPERTIES, CELLS_PROPERTIES_TYPE, LOOKUPTABLECELL, CELLS_PROPERTIES_VALUES /* = End */ };
    enum  FILE_NARGUMENTS   { N_HEADER0 = 1, N_HEADER2 = 1, N_DATASET = 1, N_POINTSINF = 2, N_POINTSVALUES = 3, N_DATAVALUES = 1, N_CELLINF = 2, N_CELLTYPES = 1, N_POINTS_PROPERTIES = 1, N_POINTS_PROPERTIES_TYPE = 4, N_LOOKUPTABLE = 1, N_CELLS_PROPERTIES = 1, N_CELLS_PROPERTIES_TYPE = 3/* = End */ };


    bool getHeader( QRegExp RegExp, QString line, vector< std::string >& data );
    bool getValues( QRegExp RegExp, QString line, vector< std::string > &data );

    VTKData::FILE_STATES readDataSetValues();
    int parseVTKData_new( std::ifstream& file, FILE_STATES& state, std::string/*QString*/& line );

    void readScalars( vector< std::string > data, std::string type );
    void readVectors( vector< std::string > data, std::string type );





private:

    std::string file_version;
    std::string file_comments;
    std::string file_format;
    std::string dataset_type;

    std::string format_points;
    int number_of_points;
    vector< float > vector_points;

    int number_of_cells;
    int list_cells_size;
    vector< Cell > vector_cells;
    int count_cells;

    vector< FlowProperty > vector_point_properties;
    vector< FlowProperty > vector_cell_properties;
    std::map< std::string, int > ppoint_name_id;
    std::map< std::string, int > pcell_name_id;
    int count_properties;

    vector< std::string > lookuptable;
    vector< std::string > lookuptablecell;

    int visitCells;
    int visitPoints;
    bool is_empty;


};

#endif // VTKDATA_H
