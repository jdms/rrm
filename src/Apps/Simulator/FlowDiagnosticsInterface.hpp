#ifndef FLOWDIAGNOSTICSINTERFACE_HPP
#define FLOWDIAGNOSTICSINTERFACE_HPP

#include <vector>
#include <string>

#include "FlowComputation/region.h"

class FlowDiagnosticsInterface
{


	public:
	



        FlowDiagnosticsInterface();

        /// \brief Load default values -- refers to: ( Zhao's document ) Please include a Function to load default values for user inputs
        void loadDefaultValues(int);

        /// \brief Set regions properties as user inputs -- refers to: ( Zhao's document ) 1. Number of regions, permeability, porosity,
        ///		   viscosity and x, y, z for each region (this can be same as before)

        void setNumberofRegions( unsigned int regions_number );

        void setRegion( unsigned int id, double x, double y, double z, double perm,
                        double poros, double visc );


        /// \brief Get regions properties -- refers to: ( Zhao's document ) 1. Number of regions, permeability, porosity, viscosity and
        ///		   x, y, z for each region (this can be same as before)

        unsigned int getNumberofRegions() const ;

        void getRegion( unsigned int id, double& x, double& y, double& z, double& perm,
                        double &poros, double& visc ) ;


        /// \brief Clean all the regions
        void clearRegions();








        /// \brief Set wells properties as user inputs -- refers to: ( Zhao's document ) 2. Number of wells, well type (1 pressure-controled
        ///		   or 2 flowrate-controled), value (unit is bar or m3/sec), sign (1 for injector, -1 for producer): please also check
        ///		   region::userinput(char*)

        void setNumberofWells( unsigned int wells_number );

        void setWell( unsigned int id, unsigned int type, double value, int sign );



        /// \brief Get wells properties -- refers to: ( Zhao's document ) 2. Number of wells, well type (1 pressure-controled or 2 flowrate-controled),
        ///		   value (unit is bar or m3/sec), sign (1 for injector, -1 for producer): please also check region::userinput(char*)

        unsigned int getNumberofWells() const ;

        void getWell( unsigned int id, unsigned int& type, double& value, int& sign ) ;


        /// \brief Clean all the wells
        void clearWells();


        void loadWellsGeometry( char* ); //should be called after setting all wells





        /// \brief Read user input file --refers to: last code
        void readUserInput( const std::string& input_file );


        /// \brief Read skeleton file --refers to: last code, optional
        void readSkeletonFile( const std::string& skeleton_file );


        /// \brief Set informations from RRM surfaces -- refers to: last code, required
        void setSkeletonData( unsigned int surfaces_number, const std::vector< unsigned int >& nu,
                              const std::vector< unsigned int >& nv, const std::vector< double >& positions );


        void getSurfaceSkeleton( unsigned int& surfaces_number, std::vector< unsigned int >& nu,
                                 std::vector< unsigned int >& nv, std::vector< double >& positions ) const;






        /// \brief A kind of pre-processing or preparation to compute properties -- refers to: last code, optional
        void init();

        /// \brief Method to compute properties -- refers to: last code, required
        void computeProperties();




        /// \brief Method to build the volumetric mesh -- refers to: last code, required
        void buildVolumetricMesh();


        /// \brief Method to build the cpg volumetric mesh -- refers to: last code, required
        void buildCPGVolumetricMesh();






        /// \brief Get pressure values by vertex (only) -- refers to: last and new code (from writeresult_unstructured),
        ///        and required by Zhao's document ( output corrected_pressure )
        void getPressure( std::vector< double >& values ) ;


        /// \brief Get velocity values by vertex -- refers to: new code (from writeresult_unstructured),
        ///        and required by Zhao's document ( output velocity )
        void getVelocitybyVertices( std::vector< double >& values ) ;


        /// \brief Get velocity values by cell -- refers to: new code (from writeresult_unstructured),
        ///        and required by Zhao's document ( output velocity )
        void getVelocitybyCells( std::vector< double >& values );


        /// \brief Get backward tof by vertex (only) -- refers to: new code (from writeresult_unstructured),
        ///        and required by Zhao's document ( output Backward_TOF )
        void getBackwardTOF( std::vector< double >& values ) ;


        /// \brief Get forward tof by vertex (only) -- refers to: new code (from writeresult_unstructured),
        ///        and required by Zhao's document ( output Forward_TOF )
        void getForwardTOF( std::vector< double >& values ) ;



        /// \brief Get total tof by vertex (only) -- refers to: new code (from writeresult_unstructured),
        ///        and required by Zhao's document ( output Total_TOF )
        void getTotalTOF( std::vector< double >& values ) ;


        /// \brief Get max backward tracer by vertex (only) -- refers to: new code (from writeresult_unstructured),
        ///        and required by Zhao's document ( output Max_Backward_Tracer )
        void getMaxBackwardTracer( std::vector< double >& values );


        /// \brief Get max forward tracer by vertex (only) -- refers to: new code (from writeresult_unstructured),
        ///        and required by Zhao's document ( output Max_Forward_Tracer )
        void getMaxForwardTracer( std::vector< double >& values );


        /// \brief Get max forward tracer by cell (only) -- refers to: new code (from writeresult_unstructured),
        ///        and required by Zhao's document ( output Permeability ).
        void getPermeabilitybyCells( std::vector< double >& values ) ;


        /// \brief Get max forward tracer by cell (only) -- refers to: new code (from writeresult_unstructured),
        ///        and required by Zhao's document ( output Permeability ).
        void getPermeabilitybyVertices( std::vector< double >& values ) ;

        void getCPGPermeability( std::vector< double >& values );


        /// \brief Get region id by cell (only) -- refers to: new code (from writeresult_unstructured), optional
        ///        It's not clear if it is required and if so, how should be shown to user. What does it mean?
        void getRegionId( std::vector< int >& regions_id ) ;


        /// \brief It's not clear how should be shown to user. What does it mean? It is required required by
        ///        Zhao's document ( Flow diagnostic information: please check
        ///        region::writeflowdiagnostics(char*, char*, char*). These information can be similarly
        ///        displayed like pore volume
        void getFlowDiagnosticInformation();



        /// Observation: below there are many functions to get properties values of cornerpoint grids, however, it was not
        /// clear in the document if they are required or not. I have supposed they are. Please, delete if I am wrong.



        /// \brief Get pressure values by cell (only), in CPG volumetric mesh -- refers to: new code (from writeresult_cpgfv)
        void getCPGPressure( std::vector< double >& values ) ;


        /// \brief Get tof values by cell (only), in CPG volumetric mesh -- refers to: new code (from writeresult_cpgfv)
        void getCPGTOF( std::vector< double >& values );


        /// \brief Get tof values by cell (only), in CPG volumetric mesh -- refers to: new code (from writeresult_cpgfv)
        void getCPGBackwardTOF( std::vector< double >& values ) ;


        /// \brief Get total tof values by cell (only), in CPG volumetric mesh -- refers to: new code (from writeresult_cpgfv)
        void getCPGTotalTOF( std::vector< double >& values ) ;


        /// \brief Get tracer values by cell (only), in CPG volumetric mesh -- refers to: new code (from writeresult_cpgfv)
        //void getTracer(std::vector< double >& values);
        void getCPGMaxForwardTracer( std::vector< double >& values );


        /// \brief Get backward tracer values by cell (only), in CPG volumetric mesh -- refers to: new code (from writeresult_cpgfv)
        //void geBackwardTracer( std::vector< double >& values );
        void getCPGMaxBackwardTracer(std::vector< double >& values);



        /// \brief Get vertices and faces of the reparametrized surface -- it is required by the gui.
        //we can talk more about this if necessary
        void getSurfaceVertices( std::vector< float >& vertices ) const;

        void getSurfaceFaces( std::vector< unsigned int >& faces ) const ;




        /// \brief Get vertices, edges and cells of the unstructured grid -- it is required by the gui.
        void getVolumeVertices( std::vector< float >& vertices )  ;

        void getVolumeEdges( std::vector< unsigned int >& edges )  ;

        void getVolumeCells( std::vector< unsigned int >& cells )  ;



        /// \brief Get vertices, edges and cells of the cornerpoint grid -- it is required by the gui.
        void getCPGVolumeVertices( std::vector< float >& vertices )  ;

        void getCPGVolumeEdges( std::vector< unsigned int >& edges )  ;

        void getCPGVolumeCells( std::vector< unsigned int >& cells )  ;



        /// \brief Export results -- optional
        void exportSurfacetoVTK( const std::string& filename );

        void exportCornerPointtoVTK( const std::string& filename );

        void exportCornerPointtoGRDECL( const std::string& filename );

        void exportResultstoVTK( const std::string& filename );



        /// \brief Please, clean all the data -- required by the gui
        void clear();


    private:

        REGION region;
		
};




#endif
