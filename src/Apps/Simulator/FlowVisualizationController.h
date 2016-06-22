#ifndef FLOWVISUALIZATIONCONTROLLER_H
#define FLOWVISUALIZATIONCONTROLLER_H

#include "Model/CrossSection.hpp"

#include <vector>

#include <QObject>

#include "FlowComputation/region.h"
#include "Mesh.h"
#include "ProgressCounter.h"

class FlowVisualizationController: public QObject
{
    Q_OBJECT

    public:

    typedef qreal		 	 	Real;
    typedef RRM::CrossSection<Real>  	CrossSection;


        FlowVisualizationController( QWidget* parent = 0 );



        inline void getTetgenParameter( std::string& cmd ) { region.getTetgenCommand( cmd );  }
        inline void getPropertyArea( int& np, std::vector< double >& values ){ region.getPropertyArea( np, values ); }

        inline void getBoundariesValues( int& ndir, std::vector< double > &vdir )   { region.getBoundariesSurface( ndir, vdir );  }
        inline void getWellsParameter( int& nswells, std::vector< double > &vswells )  { region.getWells( nswells, vswells );}
        inline void getTofBoundaryParameter( int& ntfbound, std::vector< int > &vtfbound )  { region.getTofBoundaries( ntfbound, vtfbound ); }
        inline void getTracerBoundaryParameter( int& ntrbound, std::vector< int > &vtrbound )  { region.getTracerBoundaries( ntrbound, vtrbound ); }


        void getVelocityValuesbyVertex( std::vector<double>& values );
        void getVelocityValuesbyCell( std::vector<double>& values );
        void getPressureValuesbyVertex(std::vector<double> &values );
        void getTOFValuesbyVertex( std::vector< double >& values );
        void getTOFValuesbyCell( std::vector< double >& values );
        void getTracerValuesbyVertex( std::vector< double >& values );
        void getTracerValuesbyCell( std::vector< double >& values );


        void getSurfaceFromCrossSection(const CrossSection& cross_section );



        std::vector< float > getMeshVerticesfromFile( const tetgenio& obj );
        std::vector< unsigned int > getMeshWireFramefromFile( const tetgenio& obj );
        std::vector< unsigned int > getMeshFacesfromFile( const tetgenio& obj );



        void updateVolumetricMesh( Mesh* mesh );
        void updateMeshFromFile( Mesh* mesh  );
        void updateMeshFromSurface( Mesh* mesh );


        std::vector< float > getVolumeVerticesfromNodeStructure( const std::vector< NODE >& nodes );
        std::vector< unsigned int > getVolumeEdgesfromTetrahedronStructure( const std::vector< TETRAHEDRON >& elements );
        std::vector< unsigned int > getVolumeCellsfromTetrahedronStructure( const std::vector< TETRAHEDRON >& elements );


        void computeFlowProperties();


        std::vector< double > getVerticesPropertyValues(std::string name_of_property, std::string method, double &min, double &max );
        std::vector< double > getFacesPropertyValues( std::string name_of_property, std::string method, double& min, double& max );
        std::vector< double > vectorToScalarProperties( const std::vector< double >& values, std::string type, double& min , double& max  );


        void getMeshVisualizationParameters( std::string& trianglecmd, int& resolutiontype, int& npartitionedge, double& lenghtedge );
        void setMeshVisualizationParameters( const std::string& trianglecmd, const int& resolutiontype, const int& npartitionedge, const double& lenghtedge );


        inline void setParent( QObject *p ){ controllerParent = p; }
        void setCounterProgressinData();

        bool isMeshOk(){ return mesh_ok; }
        bool isVolumetricOk(){ return volumetric_ok; }
        bool arePropertiesComputed() { return properties_computed; }

        void clear();

        void exportSurfacetoVTK( const std::string& filename );
        void exportVolumetoVTK( const std::string& filename );
        void exportCornerPointtoVTK( const std::string& filename );


        inline void emitSignaltoGetSurfaceCrossSection(){ emit getSurfaceCrossSection(); }
        inline void emitSignaltoReadFile(){ emit readFile(); }
        inline void emitSignaltoEditParameters(){ emit editParameters(); }
        inline void emitSignaltoApplyCrossSection(){ emit applyCrossSection(); }
        inline void emitSignaltoClearEverything(){ emit clearAll(); }


    public slots:

        inline void setToleranceValues( const float& tol1, const float& tol2 ){ region.tolerance( tol1, tol2 ); }

        void setTetgenCommand( std::string& cmd );
        inline void setPropertyArea( const int& np, const std::vector< double >& values ){ region.setPropertyArea( np, values ); }

        void setBoundariesValues( int n, std::vector< double > &values );
        void setWellsValues( int n, std::vector< double > &values );
        void setTofBoundaryValues( int n, std::vector< double > &values );
        void setTrBoundaryValues( int n, std::vector< double > &values );


        void readCornerPoint( bool read_file, const std::string& mesh_file );
        void readUnstructured( bool read_file,  const std::string& mesh_file, const std::string& type_of_file, const std::string& input_file  );
        void reloadMesh( const int& type_of_mesh, bool read_file,  const std::string& mesh_file, const std::string& type_of_file, const std::string& input_file  );

        void computeVolumetricMesh();


        void increaseMeshScale();
        void decreaseMeshScale();

        void increaseNumberofEdges();
        void decreaseNumberofEdges();

        void increaseEdgeLength();
        void decreaseEdgeLength();


    signals:

            void updateMesh();
            void updateVolumetricMesh();
            void updatePolyMesh();

            void propertybyVertexComputed( std::string, std::string );
            void propertybyFaceComputed( std::string, std::string );

            void getSurfaceCrossSection();
            void readFile();
            void editParameters();
            void applyCrossSection();

			void hideToolbar();

            void clearAll();

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
