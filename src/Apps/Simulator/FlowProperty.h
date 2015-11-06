#ifndef FLOWPROPERTY_H
#define FLOWPROPERTY_H

#include <vector>
#include <string>

using namespace std;

class FlowProperty
{

    public:

    enum TYPE_PROPERTY { POINTS, CELLS };
    enum FORMAT_PROPERTY { SCALARS, VECTORS };



        FlowProperty();

        inline void setId( int id ){ index = id; }
        inline int getId() const { return index; }


        inline void setName( std::string n ){ name = n; }
        inline void getName( std::string &n ) const { n = name; }


        inline void setFormat( std::string f )
        {
            if( f.compare( "SCALARS" ) == 0 ) format = FORMAT_PROPERTY::SCALARS;
            else if( f.compare( "VECTORS" ) == 0 ) format = FORMAT_PROPERTY::VECTORS;

        }
        inline FlowProperty::FORMAT_PROPERTY getFormat() const { return format; }
        inline void getFormat( std::string& f ) const
        {

            if( format == FORMAT_PROPERTY::SCALARS )
                f = "SCALARS";
            else if( format == FORMAT_PROPERTY::VECTORS )
                f ="VECTORS";
        }


        inline void setType( std::string t )
        {
            if( t.compare( "POINTS" ) == 0 ) type = TYPE_PROPERTY::POINTS;
            else if( t.compare( "CELLS" ) == 0 ) type = TYPE_PROPERTY::CELLS;

        }
        inline TYPE_PROPERTY getType() const { return type; }
        inline void getType( std::string& t ) const
        {
            if( type == TYPE_PROPERTY::POINTS )
                t = "POINTS";
            else if( type == TYPE_PROPERTY::CELLS )
                t = "CELLS";
        }


        inline void setNumberofComponents( int n ) { number_of_components = n; }
        inline int getNumberofComponents() const { return number_of_components; }


        inline void setValues( vector< float > v ){ values = v; }
        inline void getValues( vector< float >& v ) const { v = values; }


        inline void addValue( float v ){ values.push_back( v ); }
        inline void getValue ( int id, float &v ) const
        {
            int nvalues = (int) values.size();
            if( id < 0 || id >= nvalues ) return;
            v = values[ id ];
        }


        inline int getNumberofValues() const { return (int) values.size(); }


        inline void setMinimum( float m ){ min = m; }
        inline float getMinimum() const { return min; }


        inline void setMaximum( float M ){ max = M; }
        inline float getMaximum() const{ return max; }


        inline void addMinimumCoordinate( float m ){ min_coordinates.push_back( m );}
        inline void addMaximumCoordinate( float M ){ max_coordinates.push_back( M );}


        inline void getMaxMinCoordinateProperty( float &xmin, float &xmax, float& ymin, float& ymax, float &zmin, float &zmax )
        {
            xmin = min_coordinates[ 0 ];
            xmax = max_coordinates[ 0 ];

            ymin = min_coordinates[ 1 ];
            ymax = max_coordinates[ 1 ];

            zmin = min_coordinates[ 2 ];
            zmax = max_coordinates[ 2 ];
        }


        void getPointProperty( int id, std::string& name, std::string& format, std::string type, int ncoords );
        void getCellProperty( int id, std::string& name, std::string& format, std::string type, int ncoords );



    protected:

        int index;
        std::string name;
        TYPE_PROPERTY type;
        FORMAT_PROPERTY format;
        int number_of_components;

        vector< float > values;
        vector< float > max_coordinates;
        vector< float > min_coordinates;
        float max;
        float min;

};

#endif // FLOWPROPERTY_H
