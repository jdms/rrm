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


#ifndef FLOWDIAGNOSTICSINTERFACE_HPP
#define FLOWDIAGNOSTICSINTERFACE_HPP

#include <vector>
#include <string>

/* #include "region.h" // Undo change 1 */ 

//// It includes the Window.h  ... OpenVolumMesh defines min/max in its own 
#include "flow_computation/region.h"

class FlowDiagnosticsInterface
{


public:




    FlowDiagnosticsInterface();

    /// \brief Load default values -- refers to: ( Zhao's document ) Please include a Function to load default values for user inputs
    void loadDefaultValues(int);

    /// \brief Set regions properties as user inputs -- refers to: ( Zhao's document ) 1. Number of regions, permeability, porosity,
    ///		   viscosity and x, y, z for each region (this can be same as before)

    void setNumberofRegions(unsigned int regions_number);

  

    /// \brief Get regions properties -- refers to: ( Zhao's document ) 1. Number of regions, permeability, porosity, viscosity and
    ///		   x, y, z for each region (this can be same as before)

    unsigned int getNumberofRegions() const;

    void getRegion(unsigned int id, double& x, double& y, double& z, double& perm,
        double &poros, double& visc, double &porevolume);


    /// \brief Clean all the regions
    void clearRegions();


    /// \brief Set wells properties as user inputs -- refers to: ( Zhao's document ) 2. Number of wells, well type (1 pressure-controled
    ///		   or 2 flowrate-controled), value (unit is bar or m3/sec), sign (1 for injector, -1 for producer): please also check
    ///		   region::userinput(char*)

    void setNumberofWells(unsigned int wells_number);

    void setWell(unsigned int id, unsigned int type, double value, int sign);



    /// \brief Get wells properties -- refers to: ( Zhao's document ) 2. Number of wells, well type (1 pressure-controled or 2 flowrate-controled),
    ///		   value (unit is bar or m3/sec), sign (1 for injector, -1 for producer): please also check region::userinput(char*)

    unsigned int getNumberofWells() const;

    void getWell(unsigned int id, unsigned int& type, double& value, int& sign);


    /// \brief Clean all the wells
    void clearWells();


    void loadWellsGeometry(char*); //should be called after setting all wells


    /// \brief Read skeleton file --refers to: last code, optional
    void readSkeletonFile(const std::string& skeleton_file);


    /// \brief Set informations from RRM surfaces -- refers to: last code, required
    void setSkeletonData(unsigned int surfaces_number, const std::vector< unsigned int >& nu,
        const std::vector< unsigned int >& nv, const std::vector< double >& positions);


    void getSurfaceSkeleton(unsigned int& surfaces_number, std::vector< unsigned int >& nu,
        std::vector< unsigned int >& nv, std::vector< double >& positions) const;




    void setVolumeDimensions(double width, double height, double depth);



    /// \brief A kind of pre-processing or preparation to compute properties -- refers to: last code, optional
    void init();

    /// \brief Method to compute properties -- refers to: last code, required
    void computeProperties();




    /// \brief Method to build the volumetric mesh -- refers to: last code, required
    void buildVolumetricMesh();


    /// \brief Method to build the cpg volumetric mesh -- refers to: last code, required
    void buildCPGVolumetricMesh();

    void getPoreVolumebyRegion(std::vector< double >& values);



    /// \brief Get pressure values by vertex (only) -- refers to: last and new code (from writeresult_unstructured),
    ///        and required by Zhao's document ( output corrected_pressure )
    void getPressure(std::vector< double >& values);


    /// \brief Get velocity values by vertex -- refers to: new code (from writeresult_unstructured),
    ///        and required by Zhao's document ( output velocity )
    void getVelocitybyVertices(std::vector< double >& values); //velocity defined on elements

    /// \brief Get velocity values by cell -- refers to: new code (from writeresult_unstructured),
    ///        and required by Zhao's document ( output velocity )
    void getVelocitybyCells(std::vector< double >& values);
    void getVelocityMagnitudebyCells_log10(std::vector< double >& values);
    /// \brief Get backward tof by vertex (only) -- refers to: new code (from writeresult_unstructured),
    ///        and required by Zhao's document ( output Backward_TOF )
    void getBackwardTOF(std::vector< double >& values);
    void getBackwardTOF_log10(std::vector< double >& values);

    /// \brief Get forward tof by vertex (only) -- refers to: new code (from writeresult_unstructured),
    ///        and required by Zhao's document ( output Forward_TOF )
    void getForwardTOF(std::vector< double >& values);
    void getFowardTOF_log10(std::vector< double >& values);

    /// \brief Get total tof by vertex (only) -- refers to: new code (from writeresult_unstructured),
    ///        and required by Zhao's document ( output Total_TOF )
    void getTotalTOF(std::vector< double >& values);
    void getTotalTOF_log10(std::vector< double >& values);

    /// \brief Get max backward tracer by vertex (only) -- refers to: new code (from writeresult_unstructured),
    ///        and required by Zhao's document ( output Max_Backward_Tracer )
    void getMaxBackwardTracer(std::vector< double >& values);


    /// \brief Get max forward tracer by vertex (only) -- refers to: new code (from writeresult_unstructured),
    ///        and required by Zhao's document ( output Max_Forward_Tracer )
    void getMaxForwardTracer(std::vector< double >& values);


    /// \brief Get max forward tracer by cell (only) -- refers to: new code (from writeresult_unstructured),
    ///        and required by Zhao's document ( output Permeability ).
    void getPermeabilitybyCells(std::vector< double >& values);


    /// \brief Get max forward tracer by cell (only) -- refers to: new code (from writeresult_unstructured),
    ///        and required by Zhao's document ( output Permeability ).
    void getPermeabilitybyVertices(std::vector< double >& values); //permeability defined on cells


    void getPorosity(std::vector< double >& values);


    void getCPGPermeability(std::vector< double >& values);


    /// \brief Get region id by cell (only) -- refers to: new code (from writeresult_unstructured), optional
    ///        It's not clear if it is required and if so, how should be shown to user. What does it mean?
    void getRegionId(std::vector< int >& regions_id);


    /// Observation: below there are many functions to get properties values of cornerpoint grids, however, it was not
    /// clear in the document if they are required or not. I have supposed they are. Please, delete if I am wrong.



    /// \brief Get pressure values by cell (only), in CPG volumetric mesh -- refers to: new code (from writeresult_cpgfv)
    void getCPGPressure(std::vector< double >& values);


    void getCPGPorosity(std::vector< double >& values);


    /// \brief Get tof values by cell (only), in CPG volumetric mesh -- refers to: new code (from writeresult_cpgfv)
    void getCPGTOF(std::vector< double >& values);


    /// \brief Get tof values by cell (only), in CPG volumetric mesh -- refers to: new code (from writeresult_cpgfv)
    void getCPGBackwardTOF(std::vector< double >& values);


    /// \brief Get total tof values by cell (only), in CPG volumetric mesh -- refers to: new code (from writeresult_cpgfv)
    void getCPGTotalTOF(std::vector< double >& values);


    /// \brief Get tracer values by cell (only), in CPG volumetric mesh -- refers to: new code (from writeresult_cpgfv)
    //void getTracer(std::vector< double >& values);
    void getCPGMaxForwardTracer(std::vector< double >& values);


    /// \brief Get backward tracer values by cell (only), in CPG volumetric mesh -- refers to: new code (from writeresult_cpgfv)
    //void geBackwardTracer( std::vector< double >& values );
    void getCPGMaxBackwardTracer(std::vector< double >& values);


    void getCPGVelocity(std::vector< double >& values);


    /// \brief Get vertices and faces of the reparametrized surface -- it is required by the gui.
    //we can talk more about this if necessary
    void getSurfaceVertices(std::vector< float >& vertices) const;

    void getSurfaceFaces(std::vector< unsigned int >& faces) const;




    /// \brief Get vertices, edges and cells of the unstructured grid -- it is required by the gui.
    void getVolumeVertices(std::vector< float >& vertices);

    void getVolumeEdges(std::vector< unsigned int >& edges);

    void getVolumeCells(std::vector< unsigned int >& cells);//, std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> ptr_mesh);


    /// \brief Set regions for tetrahedral mesh
    void setTetrahedralMeshRegions( const std::vector<int> &regions );

    /// \brief Get vertices, edges and cells of the cornerpoint grid -- it is required by the gui.
    void getCPGVolumeVertices(std::vector< float >& vertices);

    void getCPGVolumeEdges(std::vector< unsigned int >& edges);

    void getCPGVolumeCells(std::vector< unsigned int >& cells);//, std::shared_ptr<OpenVolumeMesh::HexahedralMesh3d> ptr_mesh);



    /// \brief Export results -- optional
    void exportSurfacetoVTK(const std::string& filename);

    void exportCornerPointtoVTK(const std::string& filename);

    void exportCornerPointtoGRDECL(const std::string& filename);

    void exportResultstoVTK(const std::string& filename);



    /// \brief Please, clean all the data -- required by the gui
    void clear();


    /* ****************************************************************
    *
    * The following methods were added to satisfy the requirements
    * for the June 2017 release of the RRM prototype.
    *
    * ************************************************************** */

    /* The following method allows configuring a well type */

    enum WellType { INJECTOR, PRODUCER };

    bool setWell(unsigned int id, WellType t, double pressure_value,
        double qt_x, double qt_y, double qt_z, double well_depth);//qt_z not needed

    bool setVerticalWell(unsigned int id, WellType t, double pressure_value,
        double qt_x, double qt_y, double topd_, double botd_);

    /* Allow changing the linear solver for Zhao's flow diagnostics */

    enum SolverType { HYPRE, SAMG };

    /* Not for now */
    bool setSolver(SolverType solver);


    /* Get a string with the values of the upscalled permeability to
    * display in the GUI
    * */

    bool getUpscalledPermeability(std::string &result);


    /* Export derived quantities (such as flow capacity) to a file, or
    * get this information to display it in the GUI
    * */

    bool exportDerivedQuantities(const std::string &filename);

    /* Not for now */
    bool getDerivedQuantities(std::string &derived_quantities);


    /* Set Dirchlet boundary conditions (pressure). Remember that
    * boundary conditions must be admissible, thus, initially it
    * may be wise to allow the user to impose the pressure in
    * just one of the boundaries. */

    enum Boundary { BOTTOM, TOP, LEFT, RIGHT, FRONT, BACK };

    /* Not for now */
    bool setBoundaryConditions(Boundary b, int type_, double value_, int sign_);


    /* This method clears all computed quantities (related to the flow
    * diagnostics) without clearing the meshes (either tetrhedral or
    * hexahedral).  This allows the user to change rock properties
    * (porosity, permeability) and rerun the simulation without having
    * to build a volume mesh.
    * */

    void clearComputedQuantities();

	
    /* The following definitions are the first step towards creating
    * tetrhedral meshes for the new generic models.
    *
    * The main idea is to allow the creation of a picewise linear
    * complex for the boundary of the model, to be used as input for
    * Tetgen.
    * */

    struct TriangleMesh
    {
        std::vector<double> vertex_list;
        std::vector<std::size_t> face_list;
    };

    struct CurveMesh
    {
        std::vector<double> vertex_list;
        std::vector<std::size_t> edge_list;
    };

    /* for integration Jan 2018 */
    bool setSkeleton(
        const std::vector<TriangleMesh> &triangle_meshes,
        const std::vector<CurveMesh> &left_boundary_curves,
        const std::vector<CurveMesh> &right_boundary_curves,
        const std::vector<CurveMesh> &front_boundary_curves,
        const std::vector<CurveMesh> &back_boundary_curves
        );

    //refer to writeflowdiagnostics

    //For September 2017 start*********************************************************

    void getOilInPlace(double &oil_in_place);
    void setWaterSaturation(unsigned int region_id, double Sw);

    void setNumberOfPhases(int);//1 or 2; init() updated; setviscosity is oil viscosity.
	int  getNumberOfPhases();//1 or 2; init() updated; setviscosity is oil viscosity.


    void setSaturationMethod(int); //1 means set saturation for each region; 2 means by gravity
    
    //visualise nodal capillary pressure, elemental saturation, elemental krw, kro;
    bool getCapillaryPressure(std::vector< double >& values); //nodal
    bool getWaterSaturationByCells(std::vector< double >& values);
    bool getKrwByCells(std::vector< double >& values);
    bool getKroByCells(std::vector< double >& values);
    //For September 2017 end************************************************************

    // as suggested by Matt the GUI would provide an integer (id) to be tied to a file
    // with the properties of the correspondent permeability curve
    void setPermeabilityCurve(unsigned int region_id, int pcurve_id);
    void getAvailablePermeabilityCurves(std::vector<int> &pcurves_ids);

    void setRegion(unsigned int id, double x, double y, double z,
        double min_perm, double max_perm,
        double min_poros, double max_poros);

	/*void setRegion(unsigned int id, 
		double min_perm, double max_perm,
		double min_poros, double max_poros); */

	//void setRegion(unsigned int id, double x, double y, double z, double perm, double poros);

   /* void setRegion(unsigned int id, double x, double y, double z, double perm,
        double poros, double visc)
    {
        setRegion(id, x, y, z, perm, poros);
        visc_ = visc;
    }*/

    
    /*void setRegion(unsigned int id, double x, double y, double z,
        double min_perm, double max_perm,
        double min_poros, double max_poros,
        double visc)
    {
        setRegion(id, x, y, z, min_perm, max_perm, min_poros, max_poros);
        visc_ = visc;
    }*/
	

	void setRegion_sf(unsigned int id, double Kxlow_, double Kylow_, double Kzlow_, double Kxhigh_,
		double Kyhigh_, double Kzhigh_, double porolow_, double porohigh_);

	void setRegion_mf(unsigned int id, double Kxlow_, double Kylow_, double Kzlow_, double Kxhigh_,
		double Kyhigh_, double Kzhigh_, double porolow_, double porohigh_,
		double Pct_, double Si_, double lambda_);

	//either set water saturation (setsaturationmethod=1) or oil gravity (setsaturationmethod=2)
	void setViscosity(double visc);//oil viscosity
	void setViscosity_water(double visc);
	void setdensity_water(double den_w);
	void setdensity_oil(double den_w);
	void setOilGravity(double); //same as setdensity_oil
    void setfreewaterlevel(double);
	void setBo(double Bo_);
	void setBw(double Bw_);

private:

    REGION region;

    // This code is a stub to allow changing the Flow Diagnostics GUI

    bool multi_phase_flow = false;
    double visc_ = 1;

};





#endif
