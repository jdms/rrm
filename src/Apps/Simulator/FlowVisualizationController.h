#ifndef FLOWVISUALIZATIONCONTROLLER_H
#define FLOWVISUALIZATIONCONTROLLER_H

#include <vector>

#include <QObject>

#include "FlowComputation/region.h"
#include "Mesh.h"
#include "ProgressCounter.h"

class FlowVisualizationController: public QObject
{
    Q_OBJECT

    public:

        FlowVisualizationController( QWidget* parent = 0 );



        inline void getTetgenParameter( std::string& cmd ) { region.getTetgenCommand( cmd );  }
        inline void getViscosityParameter( float& vis ) { vis = region.getViscosityValue(); }
        inline void getPermeabilityParameter( int& nperm, std::vector< double > &vperm ) { region.getPermeabilityValue( nperm, vperm );  }
        inline void getBoundariesValues( int& ndir, std::vector< double > &vdir )   { region.getBoundariesSurface( ndir, vdir );  }
        inline void getWellsParameter( int& nswells, std::vector< double > &vswells )  { region.getWells( nswells, vswells );}
        inline void getTofBoundaryParameter( int& ntfbound, std::vector< int > &vtfbound )  { region.getTofBoundaries( ntfbound, vtfbound ); }
        inline void getTracerBoundaryParameter( int& ntrbound, std::vector< int > &vtrbound )  { region.getTracerBoundaries( ntrbound, vtrbound ); }


        void getVelocityValuesbyVertex( std::vector<double>& values );
        void getVelocityValuesbyCell( std::vector<double>& values );
        void getPressureValuesbyVertex(std::vector<double> &values );
        void getTOFValuesbyVertex( std::vector< double >& values );
        void getTOFValuesbyCell( std::vector< double >& values );


        void getSurfaceFromCrossSection( /*RRM::CrossSection<double>& _cross_section*/ );



        std::vector< float > getMeshVerticesfromFile( const tetgenio& obj );
        std::vector< unsigned int > getMeshWireFramefromFile( const tetgenio& obj );
        std::vector< unsigned int > getMeshFacesfromFile( const tetgenio& obj );



        void updateVolumetricMesh( Mesh* mesh );
        void updateMeshFromFile( Mesh* mesh  );


        std::vector< float > getVolumeVerticesfromNodeStructure( const std::vector< NODE >& nodes );
        std::vector< unsigned int > getVolumeEdgesfromTetrahedronStructure( const std::vector< TETRAHEDRON >& elements );
        std::vector< unsigned int > getVolumeCellsfromTetrahedronStructure( const std::vector< TETRAHEDRON >& elements );


        void computeFlowProperties();
        void computePressure();
        void computeVelocity();
        void computeTOF();

        std::vector< double > getVerticesPropertyValues(std::string name_of_property, std::string method, double &min, double &max );
        std::vector< double > getFacesPropertyValues( std::string name_of_property, std::string method, double& min, double& max );
        std::vector< double > vectorToScalarProperties( const std::vector< double >& values, std::string type, double& min , double& max  );

        inline void setParent( QObject *p ){ controllerParent = p; }
        void setCounterProgressinData();

        bool isMeshOk(){ return mesh_ok; }
        bool isVolumetricOk(){ return volumetric_ok; }
        bool arePropertiesComputed() { return properties_computed; }

        void clear();

        void exportSurfacetoVTK( const std::string& filename );
        void exportVolumetoVTK( const std::string& filename );
        void exportCornerPointtoVTK( const std::string& filename );


    public slots:

        inline void setToleranceValues( const float& tol1, const float& tol2 ){ region.tolerance( tol1, tol2 ); }

        void setTetgenCommand( std::string& cmd );
        void setViscosityValue( double value );
        void setPermeabilityValues( int n, std::vector< double > &values );
        void setBoundariesValues( int n, std::vector< double > &values );
        void setWellsValues( int n, std::vector< double > &values );
        void setTofBoundaryValues( int n, std::vector< double > &values );
        void setTrBoundaryValues( int n, std::vector< double > &values );



//        void readfromFiles( const std::string& input_file, const std::string& mesh_file, std::string type, int option = -1 );
//        void readMeshFile(const std::string &mesh_file, std::string type, int option = -1 );
//        void readParametersFile( const std::string& input_file, int option = -1 );

        void readCornerPoint( bool read_file, const std::string& mesh_file );
        void readUnstructured( bool read_file,  const std::string& mesh_file, const std::string& type_of_file, const std::string& input_file  );
        void reloadMesh( const int& type_of_mesh, bool read_file,  const std::string& mesh_file, const std::string& type_of_file, const std::string& input_file  );

        void computeVolumetricMesh();




    signals:

            void updateMesh();
            void updateVolumetricMesh();

            void propertybyVertexComputed( std::string, std::string );
            void propertybyFaceComputed( std::string, std::string );


    private:

        QObject* controllerParent;

        REGION region;
        tetgenio obj;

        ProgressCounter counter;

        bool mesh_ok;
        bool volumetric_ok;
        bool properties_computed;

};

#endif // FLOWVISUALIZATIONCONTROLLER_H
