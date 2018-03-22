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

#include "flow_diagnostics_interface.hpp"


FlowDiagnosticsInterface::FlowDiagnosticsInterface(){}


void FlowDiagnosticsInterface::loadDefaultValues(int i)// 1 for unstructured 2 for cpg
{
    if (i == 1){
        region.RRMdefaultvalues_unstructured();
    }
    else if (i == 2){
        region.RRMdefaultvalues_cpg();
    }
}


void FlowDiagnosticsInterface::setNumberofRegions(unsigned int regions_number){
    region.setnumberofpropertyareas(regions_number);
}


unsigned int FlowDiagnosticsInterface::getNumberofRegions() const {
    return region.numberofpropertyareas();
}


void FlowDiagnosticsInterface::setViscosity(double visc){
    region.oilviscosity(visc*0.001); //cp to si
}

void FlowDiagnosticsInterface::setViscosity_water(double visc){
	region.waterviscosity(visc*0.001); //cp to si
}

void FlowDiagnosticsInterface::setdensity_water(double den_w){
	region.waterdensity(den_w); //cp to si
}

void FlowDiagnosticsInterface::setdensity_oil(double den_o){
	region.oildensity(den_o*0.001); //cp to si
}

void FlowDiagnosticsInterface::setBo(double Bo_){
	region.setBo(Bo_);
}

void FlowDiagnosticsInterface::setBw(double Bw_){
	region.setBw(Bw_);
}

void FlowDiagnosticsInterface::clearRegions(){
    region.clearpropertyareas();
}

void FlowDiagnosticsInterface::setNumberofWells(unsigned int wells_number){
    region.setnumberofwells(wells_number);
}


unsigned int FlowDiagnosticsInterface::getNumberofWells() const { return region.numberofwells(); }


void FlowDiagnosticsInterface::clearWells(){
    region.clearwells();
}


void FlowDiagnosticsInterface::init(){
    if (region.meshinfo_type() == 1 && region.numberofphases() == 1){ //unstructured
		region.clear_computedproperties();
		//region.writevolumemesh("volumetetmesh.vtk");
        region.tetrahedralmesh_postprocessCVFE();
        region.properties_cvfe_sf(); //kvratio_=0.1
        region.computemobility_cvfe_sf();
        region.boundarycondition();
        region.markwellnodes_verticaltopbot();
        region.wellcondition();
        region.oilinplace();

    }
    else if (region.meshinfo_type() == 1 && region.numberofphases() == 2){ //unstructured
		region.clear_computedproperties();
        region.tetrahedralmesh_postprocessCVFE();
        region.properties_cvfe_mf();
        if (region.saturationflag() == 1){
            region.setelesaturationbyregion();
        }
        if (region.saturationflag() == 2){
            region.computecapillarypressure();
            region.computesaturation();
        }
        region.computerelativepermeabilities();
        region.computemobility_cvfe_mf();
        region.boundarycondition();
		region.markwellnodes_verticaltopbot();
        region.wellcondition(); //well boundary condition overwrite surface boundary condition
        region.oilinplace();
    }
    else if (region.meshinfo_type() == 2){ //CPG
        region.properties_cpgfv();
        region.wellcondition_cpgfv();
        region.transmatrix_cpgfv();
    }
}


void FlowDiagnosticsInterface::setVolumeDimensions(double width, double height, double depth)
{
    region.setvolumedimension(width, height, depth);
}



void FlowDiagnosticsInterface::buildVolumetricMesh(){
	if (region.sketchflag() == 0){//linear extrusion
		region.RRMdefaultvalues_unstructured();
		region.paramodel();
		region.surfacemeshmax();
		//region.inputstraightwells();
		region.inputverticalwells();
		region.unstructuredsurfacemesh();
		region.calltetgen_in2mid(); //must: already read in poly or created in
		region.wellgeonodes2addin(); //wellgeonodes -> wellnodes->addin
		region.calltetgenrefine_mid2out();
		region.tetgen2region();
		region.meshdimension_max();
	}
	else if (region.sketchflag() == 1){
		region.RRMdefaultvalues_unstructured();
		region.surfacemeshmax();
		region.inputverticalwells();
		region.buildsurfacemesh();
		region.calltetgen_in2mid(); //must: already read in poly or created in
		region.wellgeonodes2addin(); //wellgeonodes -> wellnodes->addin
		region.calltetgenrefine_mid2out();
		//region.calltetgen_in2out();
		region.tetgen2region();
		region.meshdimension_max();
	}
}


void FlowDiagnosticsInterface::buildCPGVolumetricMesh(){
	if (region.sketchflag() == 0){//linear extrusion only
		region.RRMdefaultvalues_cpg();
		region.paramodel();
		region.buildcpg_ordering();
		region.inputverticalwells();
	}
}

void FlowDiagnosticsInterface::getPressure(std::vector< double >& values) {
    values = region.getpressure();
}

void FlowDiagnosticsInterface::getVelocitybyCells(std::vector< double >& values) {
    values = region.getvelocity_elements();
}

void FlowDiagnosticsInterface::getBackwardTOF(std::vector< double >& values) {
	values = region.getbackwardtof();
}
void FlowDiagnosticsInterface::getVelocityMagnitudebyCells_log10(std::vector< double >& values) {
    values = region.getvelocitymagnitude_elements_log10();
}



void FlowDiagnosticsInterface::getBackwardTOF_log10(std::vector< double >& values) {
    values = region.getbackwardtof_log10();
}


void FlowDiagnosticsInterface::getForwardTOF(std::vector< double >& values) {
    values = region.getforwardtof();
}

void FlowDiagnosticsInterface::getFowardTOF_log10(std::vector< double >& values) {
    values = region.getforwardtof_log10();
}

void FlowDiagnosticsInterface::getTotalTOF(std::vector< double >& values) {
    values = region.gettotaltof();
}
void FlowDiagnosticsInterface::getTotalTOF_log10(std::vector< double >& values) {
    values = region.gettotaltof_log10();
}

void FlowDiagnosticsInterface::getMaxBackwardTracer(std::vector< double >& values) {
    values = region.getmaxbackwardtracer();
}


void FlowDiagnosticsInterface::getMaxForwardTracer(std::vector< double >& values) {
    values = region.getmaxforwardtracer();
}


void FlowDiagnosticsInterface::getPermeabilitybyCells(std::vector< double >& values) {
    values = region.getpermx_elements();
}

void FlowDiagnosticsInterface::getPermeabilityXbyCells(std::vector< double >& values) {
	values = region.getpermx_elements();
}

void FlowDiagnosticsInterface::getPermeabilityYbyCells(std::vector< double >& values) {
	values = region.getpermy_elements();
}

void FlowDiagnosticsInterface::getPermeabilityZbyCells(std::vector< double >& values) {
	values = region.getpermz_elements();
}

void FlowDiagnosticsInterface::getVelocitybyVertices(std::vector< double >& values)
{
    (void)(values);
    return;
}

void FlowDiagnosticsInterface::getPermeabilitybyVertices(std::vector< double >& values)
{
    (void)(values);
    /* values = region.getpermeability_nodes(); */
    return;
}

void FlowDiagnosticsInterface::getPorosity(std::vector< double >& values)
{
    values = region.getporosity_elements();
}


void FlowDiagnosticsInterface::getPoreVolumebyRegion(std::vector< double >& values)
{
    values = region.getporevolume();
}



void FlowDiagnosticsInterface::getRegionId(std::vector< int >& regions_id) {
    regions_id = region.getregionid();
}



void FlowDiagnosticsInterface::getCPGPressure(std::vector< double >& values) {
    values = region.getpressure_cpg();
}


void FlowDiagnosticsInterface::getCPGTOF(std::vector< double >& values) {
    values = region.getforwardtof_cpg();
}


void FlowDiagnosticsInterface::getCPGBackwardTOF(std::vector< double >& values) {
    values = region.getbackwardtof_cpg();
}


void FlowDiagnosticsInterface::getCPGTotalTOF(std::vector< double >& values) {
    values = region.gettotaltof_cpg();
}


void FlowDiagnosticsInterface::getCPGMaxForwardTracer(std::vector< double >& values) {
    values = region.getmaxforwardtracer_cpg();
}

void FlowDiagnosticsInterface::getCPGMaxBackwardTracer(std::vector< double >& values) {
    values = region.getmaxbackwardtracer_cpg();
}


void FlowDiagnosticsInterface::getCPGPermeability(std::vector< double >& values) {
    values = region.getpermeability_elements_cpg();
}


void FlowDiagnosticsInterface::getCPGPorosity(std::vector< double >& values)
{
    values = region.getporosity_elements_cpg();
}

void FlowDiagnosticsInterface::getCPGVelocity(std::vector< double >& values)
{

}


void FlowDiagnosticsInterface::getVolumeVertices(std::vector< float >& vertices)
{
    std::vector<NODE> nodelist;
    nodelist = region.getnodelist();
    int number_of_points = (int)nodelist.size();

    for (int i = 0; i < number_of_points; ++i)
    {
        vertices.push_back(nodelist[i].x());
        vertices.push_back(nodelist[i].y());
        vertices.push_back(nodelist[i].z());
    }

}

void FlowDiagnosticsInterface::getVolumeEdges(std::vector< unsigned int >& edges)
{
    std::vector<TETRAHEDRON> elementlist = region.getelementlist();
    int number_of_cells = (int)elementlist.size();

    for (int i = 0; i < number_of_cells; ++i)
    {
        TETRAHEDRON tet = elementlist[i];

        edges.push_back((unsigned int)tet.node(0));
        edges.push_back((unsigned int)tet.node(1));

        edges.push_back((unsigned int)tet.node(1));
        edges.push_back((unsigned int)tet.node(3));

        edges.push_back((unsigned int)tet.node(3));
        edges.push_back((unsigned int)tet.node(0));

        edges.push_back((unsigned int)tet.node(1));
        edges.push_back((unsigned int)tet.node(2));

        edges.push_back((unsigned int)tet.node(2));
        edges.push_back((unsigned int)tet.node(3));

        edges.push_back((unsigned int)tet.node(2));
        edges.push_back((unsigned int)tet.node(0));

        edges.push_back((unsigned int)tet.node(3));
        edges.push_back((unsigned int)tet.node(2));
    }


}


void FlowDiagnosticsInterface::getVolumeCells(std::vector< unsigned int >& cells)//, std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> ptr_mesh)
{
    std::vector<TETRAHEDRON> elementlist = region.getelementlist();
    int number_of_cells = (int)elementlist.size();


    for (int i = 0; i < number_of_cells; ++i)
    {
        TETRAHEDRON tet = elementlist[i];

        cells.push_back((unsigned int)tet.node(0));
        cells.push_back((unsigned int)tet.node(1));
        cells.push_back((unsigned int)tet.node(2));
        cells.push_back((unsigned int)tet.node(3));
    }
}

void FlowDiagnosticsInterface::setTetrahedralMeshRegions( const std::vector<int> &regions )
{
	region.setregionids_tet(regions);
}

void FlowDiagnosticsInterface::getCPGVolumeVertices(std::vector< float >& vertices)
{

    std::vector<NODE> nodelist = region.getcpgnodelist();
    for (int i = 0; i < nodelist.size(); i++){
        vertices.push_back((float)nodelist[i].x());
        vertices.push_back((float)nodelist[i].y());
        vertices.push_back((float)nodelist[i].z());
        //std::cout << " x " << (float)nodelist[i].x() << std::endl;
    }


}



void FlowDiagnosticsInterface::getCPGVolumeEdges(std::vector< unsigned int >& edges)
{
    std::vector<CPGELEMENT> cpgelementlist = region.getcpgelementlist();
    for (int i = 0; i < cpgelementlist.size(); i++){
        unsigned int id0 = (unsigned int)cpgelementlist[i].node(0);
        unsigned int id1 = (unsigned int)cpgelementlist[i].node(1);
        unsigned int id2 = (unsigned int)cpgelementlist[i].node(2);
        unsigned int id3 = (unsigned int)cpgelementlist[i].node(3);
        unsigned int id4 = (unsigned int)cpgelementlist[i].node(4);
        unsigned int id5 = (unsigned int)cpgelementlist[i].node(5);
        unsigned int id6 = (unsigned int)cpgelementlist[i].node(6);
        unsigned int id7 = (unsigned int)cpgelementlist[i].node(7);


        edges.push_back(id0);
        edges.push_back(id1);

        edges.push_back(id1);
        edges.push_back(id3);

        edges.push_back(id3);
        edges.push_back(id2);

        edges.push_back(id2);
        edges.push_back(id0);


        edges.push_back(id4);
        edges.push_back(id5);

        edges.push_back(id5);
        edges.push_back(id7);

        edges.push_back(id7);
        edges.push_back(id6);

        edges.push_back(id6);
        edges.push_back(id4);


        edges.push_back(id0);
        edges.push_back(id4);

        edges.push_back(id1);
        edges.push_back(id5);


        edges.push_back(id3);
        edges.push_back(id7);

        edges.push_back(id2);
        edges.push_back(id6);


    }
}



void FlowDiagnosticsInterface::getCPGVolumeCells(std::vector< unsigned int >& cells)
{

    std::vector<CPGELEMENT> cpgelementlist = region.getcpgelementlist();

    for (int i = 0; i < cpgelementlist.size(); i++){

        unsigned int id0 = (unsigned int)cpgelementlist[i].node(0);
        unsigned int id1 = (unsigned int)cpgelementlist[i].node(1);
        unsigned int id2 = (unsigned int)cpgelementlist[i].node(2);
        unsigned int id3 = (unsigned int)cpgelementlist[i].node(3);
        unsigned int id4 = (unsigned int)cpgelementlist[i].node(4);
        unsigned int id5 = (unsigned int)cpgelementlist[i].node(5);
        unsigned int id6 = (unsigned int)cpgelementlist[i].node(6);
        unsigned int id7 = (unsigned int)cpgelementlist[i].node(7);

        cells.push_back(id0);
        cells.push_back(id1);
        cells.push_back(id2);
        cells.push_back(id3);
        cells.push_back(id4);
        cells.push_back(id5);
        cells.push_back(id6);
        cells.push_back(id7);

        //cells.push_back( id0 );
        //cells.push_back( id1 );
        //cells.push_back( id5 );
        //cells.push_back( id4 );


        //cells.push_back( id1 );
        //cells.push_back( id3 );
        //cells.push_back( id7 );
        //cells.push_back( id5 );

        //cells.push_back( id2 );
        //cells.push_back( id3 );
        //cells.push_back( id7 );
        //cells.push_back( id6 );

        //cells.push_back( id2 );
        //cells.push_back( id0 );
        //cells.push_back( id4 );
        //cells.push_back( id6 );


        //cells.push_back( id2 );
        //cells.push_back( id3 );
        //cells.push_back( id1 );
        //cells.push_back( id0 );

        //cells.push_back( id4 );
        //cells.push_back( id5 );
        //cells.push_back( id7 );
        //cells.push_back( id6 );

    }

}



void FlowDiagnosticsInterface::exportSurfacetoVTK(const std::string& filename){
    region.writesurfacemeshVTK(const_cast<char *>(filename.c_str()));
}

void FlowDiagnosticsInterface::exportCornerPointtoVTK(const std::string& filename){
    region.writecornerpointgridVTK(const_cast<char *>(filename.c_str()));
}

void FlowDiagnosticsInterface::exportCornerPointtoGRDECL(const std::string& filename){
    region.writecornerpointgridGRDECL(const_cast<char *>(filename.c_str()));
}

void FlowDiagnosticsInterface::exportResultstoVTK(const std::string& filename){
    region.writeresult(const_cast<char *>(filename.c_str()));
}

void FlowDiagnosticsInterface::exportMeshtoMSH(const std::string& filename){
	region.writevolumemeshMSH(const_cast<char *>(filename.c_str()));
}


void FlowDiagnosticsInterface::clear(){
    region.clearregion();
}



//bool FlowDiagnosticsInterface::getUpscalledPermeability(std::string &result)
//{
//	region.clear_computedproperties();
//	region.modelpreparation_upscale();
//	region.upscalebsurface(1);
//	region.boundarycondition();
//	region.steadystateflowsolver();
//	region.upscaling(1, result);
//	region.clearnodesbc();
//	region.upscalebsurface(2);
//	region.boundarycondition();
//	region.steadystateflowsolver();
//	region.upscaling(2, result);
//	region.clearnodesbc();
//	region.upscalebsurface(3);
//	region.boundarycondition();
//	region.steadystateflowsolver();
//	region.upscaling(3, result);
//
//	return true;
//}

bool FlowDiagnosticsInterface::getUpscalledPermeability(std::string &result)
{
	region.clear_computedproperties();
	if (region.numberofphases() == 1){
		std::vector<double> kk;
		kk = region.flowbasedupscaling_sf();
		result.append("Kx= ");
		result.append(std::to_string(kk[0]));
		result.append(" mD ");
		result.append("\n");

		result.append("Ky= ");
		result.append(std::to_string(kk[1]));
		result.append(" mD ");
		result.append("\n");

		result.append("Kz= ");
		result.append(std::to_string(kk[2]));
		result.append(" mD ");
		result.append("\n");
	}
	else if (region.numberofphases() == 2){
		region.flowbasedupscaling_mf();
		result.append("upscaled absolute and relative permeability curves exported to upscaledrelativeperm.txt in bin folder");
	}


	return true;
}


/* Export derived quantities (such as flow capacity) to a file, or
* get this information to display it in the GUI
* */

bool FlowDiagnosticsInterface::exportDerivedQuantities(const std::string &filename)
{
    region.writeflowdiagnostics(filename);
    return true;
}

bool FlowDiagnosticsInterface::getDerivedQuantities(std::string &derived_quantities)
{
    return false;
}



/* Set Dirchlet boundary conditions (pressure). Remember that
* boundary conditions must be admissible, thus, initially it
* may be wise to allow the user to impose the pressure in
* just one of the boundaries. */

bool FlowDiagnosticsInterface::setBoundaryConditions(Boundary b, int type_, double value, int sign_)
{// type: 1 for pressure 2 for velocity; sign_: 1 is outflow -1 is inflow
    return false;
}



/* This method clears all computed quantities (related to the flow
* diagnostics) without clearing the meshes (either tetrhedral or
* hexahedral).  This allows the user to change rock properties
* (porosity, permeability) and rerun the simulation without having
* to build a volume mesh.
* */

void FlowDiagnosticsInterface::clearComputedQuantities()
{
    region.clear_computedproperties();
    std::cout << " clearComputedQuantities " << std::endl;
    return;
}



/* The following definitions are the first step towards creating
* tetrhedral meshes for the new generic models.
*
* The main idea is to allow the creation of a picewise linear
* complex for the boundary of the model, to be used as input for
* Tetgen.
* */



bool FlowDiagnosticsInterface::setSkeleton(
	const std::vector<TriangleMesh> &triangle_meshes,
	const std::vector<CurveMesh> &left_boundary_curves,
	const std::vector<CurveMesh> &right_boundary_curves,
	const std::vector<CurveMesh> &front_boundary_curves,
	const std::vector<CurveMesh> &back_boundary_curves
	)
{
	region.sketchflag(1);//mark sketch 3D
	std::vector<NODE> surfacenodelist;
	std::vector<FACET> facetlist;
	NODE node_;
	FACET facet_;
	int cc = 0;
	int minsid = 0;
	int maxsid = 0;
	double minz = 100000000;
	double maxz = -100000000;
	for (int isur = 0; isur < triangle_meshes.size(); isur++){
		double zz = triangle_meshes[isur].vertex_list[2];
		if (zz > maxz){
			maxz = zz;
			maxsid = isur;
		}
		if (zz < minz){
			minz = zz;
			minsid = isur;
		}
		for (int iv = 0; iv < triangle_meshes[isur].vertex_list.size() / 3; iv++){
			node_.x(triangle_meshes[isur].vertex_list[iv * 3]);
			node_.y(triangle_meshes[isur].vertex_list[iv * 3 + 1]);
			node_.z(triangle_meshes[isur].vertex_list[iv * 3 + 2]);
			surfacenodelist.push_back(node_);
		}

		for (int iv = 0; iv < triangle_meshes[isur].face_list.size() / 3; iv++){
			facet_.node(0, triangle_meshes[isur].face_list[iv * 3] + cc);
			facet_.node(1, triangle_meshes[isur].face_list[iv * 3 + 1] + cc);
			facet_.node(2, triangle_meshes[isur].face_list[iv * 3 + 2] + cc);
			facet_.numberofvertex(3);
			facet_.markbsurface(isur);//start from 0
			facetlist.push_back(facet_);
		}
		cc = cc + triangle_meshes[isur].vertex_list.size() / 3;
	}
	region.readinsurfacenodes(surfacenodelist);
	region.readinfacets(facetlist);
	region.numberofsurfaces(triangle_meshes.size());
	//for all surfaces (sketched and vertical), remember boundary and internal for surface bc
	region.setoutbsurfaceid(minsid, maxsid, triangle_meshes.size(), triangle_meshes.size() + 1, triangle_meshes.size() + 2, triangle_meshes.size() + 3);//order same in region.buildsurfacemesh()
	region.createbsurfacelist(triangle_meshes.size() + 4);
	region.setnoflowbsurface(minsid);
	region.setnoflowbsurface(maxsid);
	region.setnoflowbsurface(triangle_meshes.size());
	region.setnoflowbsurface(triangle_meshes.size() + 1);
	region.setnoflowbsurface(triangle_meshes.size() + 2);
	region.setnoflowbsurface(triangle_meshes.size() + 3);

	std::vector<NODE> curvenodelist, curvenodelist2;
	std::vector<SEGMENT> segmentlist, segmentlist2;
	SEGMENT segment_;
	std::vector<int> markreplace, markreplacenumber;


	//for front boundary 
	segmentlist.clear();
	segmentlist2.clear();
	curvenodelist.clear();
	curvenodelist2.clear();
	markreplace.clear();
	markreplacenumber.clear();
	
	cc = 0;
	for (int ic = 0; ic < front_boundary_curves.size(); ic++){
		for (int iv = 0; iv < front_boundary_curves[ic].vertex_list.size() / 3; iv++){
			node_.x(front_boundary_curves[ic].vertex_list[iv * 3]);
			node_.y(front_boundary_curves[ic].vertex_list[iv * 3 + 1]);
			node_.z(front_boundary_curves[ic].vertex_list[iv * 3 + 2]);
			curvenodelist.push_back(node_);
		}
		for (int iv = 0; iv < front_boundary_curves[ic].edge_list.size() / 2; iv++){
			segment_.node(0, front_boundary_curves[ic].edge_list[iv * 2] + cc);
			segment_.node(1, front_boundary_curves[ic].edge_list[iv * 2 + 1] + cc);
			segmentlist.push_back(segment_);
		}
		cc = cc + front_boundary_curves[ic].vertex_list.size() / 3;
	}

	markreplace.resize(curvenodelist.size());
	markreplacenumber.resize(curvenodelist.size());
	for (int i = 0; i < markreplace.size(); i++){
		markreplace[i] = -1;
		markreplacenumber[i] = 0;
	}
	for (int inode = 0; inode < curvenodelist.size(); inode++){
		if (markreplace[inode] < 0){
			double x = curvenodelist[inode].x();
			double y = curvenodelist[inode].y();
			double z = curvenodelist[inode].z();
			for (int i = 0; i < curvenodelist.size(); i++){
				double xx = curvenodelist[i].x();
				double yy = curvenodelist[i].y();
				double zz = curvenodelist[i].z();
				if (std::abs(x - xx) < 0.0001 && std::abs(y - yy) < 0.0001 && std::abs(z - zz) < 0.0001){
					if (inode != i && markreplace[inode] <0){
						markreplace[i] = inode;//remove i
						//markreplace[inode] = -2;
						for (int j = i + 1; j < markreplacenumber.size(); j++){
							markreplacenumber[j]++;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < curvenodelist.size(); i++){
		if (markreplace[i] < 0){
			NODE node_ = curvenodelist[i];
			curvenodelist2.push_back(node_);
		}
	}
	for (int i = 0; i < segmentlist.size(); i++){
		int i1 = segmentlist[i].node(0);
		int i2 = segmentlist[i].node(1);
		if (markreplace[i1] >= 0){
			i1 = markreplace[i1];
		}
		if (markreplace[i2] >= 0){
			i2 = markreplace[i2];
		}
		i1 = i1 - markreplacenumber[i1];
		i2 = i2 - markreplacenumber[i2];
		segment_.node(0, i1);
		segment_.node(1, i2);
		segmentlist2.push_back(segment_);
	}

	region.readincurves_front(curvenodelist2, segmentlist2); //f-b-l-r

	//for back boundary 
	segmentlist.clear();
	segmentlist2.clear();
	curvenodelist.clear();
	curvenodelist2.clear();
	markreplace.clear();
	markreplacenumber.clear();
	cc = 0;
	for (int ic = 0; ic < back_boundary_curves.size(); ic++){
		for (int iv = 0; iv < back_boundary_curves[ic].vertex_list.size() / 3; iv++){
			node_.x(back_boundary_curves[ic].vertex_list[iv * 3]);
			node_.y(back_boundary_curves[ic].vertex_list[iv * 3 + 1]);
			node_.z(back_boundary_curves[ic].vertex_list[iv * 3 + 2]);
			curvenodelist.push_back(node_);
		}
		for (int iv = 0; iv < back_boundary_curves[ic].edge_list.size() / 2; iv++){
			segment_.node(0, back_boundary_curves[ic].edge_list[iv * 2] + cc);
			segment_.node(1, back_boundary_curves[ic].edge_list[iv * 2 + 1] + cc);
			segmentlist.push_back(segment_);
		}
		cc = cc + back_boundary_curves[ic].vertex_list.size() / 3;
	}

	markreplace.resize(curvenodelist.size());
	markreplacenumber.resize(curvenodelist.size());
	for (int i = 0; i < markreplace.size(); i++){
		markreplace[i] = -1;
		markreplacenumber[i] = 0;
	}
	for (int inode = 0; inode < curvenodelist.size(); inode++){
		if (markreplace[inode] < 0){
			double x = curvenodelist[inode].x();
			double y = curvenodelist[inode].y();
			double z = curvenodelist[inode].z();
			for (int i = 0; i < curvenodelist.size(); i++){
				double xx = curvenodelist[i].x();
				double yy = curvenodelist[i].y();
				double zz = curvenodelist[i].z();
				if (std::abs(x - xx) < 0.0001 && std::abs(y - yy) < 0.0001 && std::abs(z - zz) < 0.0001){
					if (inode != i && markreplace[inode] <0){
						markreplace[i] = inode;//remove i
						//markreplace[inode] = -2;
						for (int j = i + 1; j < markreplacenumber.size(); j++){
							markreplacenumber[j]++;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < curvenodelist.size(); i++){
		if (markreplace[i] < 0){
			NODE node_ = curvenodelist[i];
			curvenodelist2.push_back(node_);
		}
	}
	for (int i = 0; i < segmentlist.size(); i++){
		int i1 = segmentlist[i].node(0);
		int i2 = segmentlist[i].node(1);
		if (markreplace[i1] >= 0){
			i1 = markreplace[i1];
		}
		if (markreplace[i2] >= 0){
			i2 = markreplace[i2];
		}
		i1 = i1 - markreplacenumber[i1];
		i2 = i2 - markreplacenumber[i2];
		segment_.node(0, i1);
		segment_.node(1, i2);
		segmentlist2.push_back(segment_);
	}

	region.readincurves_back(curvenodelist2, segmentlist2); //f-b-l-r


	//for left boundary
	segmentlist.clear();
	segmentlist2.clear();
	curvenodelist.clear();
	curvenodelist2.clear();
	markreplace.clear();
	markreplacenumber.clear();
	cc = 0;
	for (int ic = 0; ic < left_boundary_curves.size(); ic++){
		for (int iv = 0; iv < left_boundary_curves[ic].vertex_list.size() / 3; iv++){
			node_.x(left_boundary_curves[ic].vertex_list[iv * 3]);
			node_.y(left_boundary_curves[ic].vertex_list[iv * 3 + 1]);
			node_.z(left_boundary_curves[ic].vertex_list[iv * 3 + 2]);
			curvenodelist.push_back(node_);
		}
		for (int iv = 0; iv < left_boundary_curves[ic].edge_list.size() / 2; iv++){
			segment_.node(0, left_boundary_curves[ic].edge_list[iv * 2] + cc);
			segment_.node(1, left_boundary_curves[ic].edge_list[iv * 2 + 1] + cc);
			segmentlist.push_back(segment_);
		}
		cc = cc + left_boundary_curves[ic].vertex_list.size() / 3;
	}

	markreplace.resize(curvenodelist.size());
	markreplacenumber.resize(curvenodelist.size());
	for (int i = 0; i < markreplace.size(); i++){
		markreplace[i] = -1;
		markreplacenumber[i] = 0;
	}
	for (int inode = 0; inode < curvenodelist.size(); inode++){
		if (markreplace[inode] < 0){
			double x = curvenodelist[inode].x();
			double y = curvenodelist[inode].y();
			double z = curvenodelist[inode].z();
			for (int i = 0; i < curvenodelist.size(); i++){
				double xx = curvenodelist[i].x();
				double yy = curvenodelist[i].y();
				double zz = curvenodelist[i].z();
				if (std::abs(x - xx) < 0.0001 && std::abs(y - yy) < 0.0001 && std::abs(z - zz) < 0.0001){
					if (inode != i && markreplace[inode] <0){
						markreplace[i] = inode;//remove i
						//markreplace[inode] = -2;
						for (int j = i + 1; j < markreplacenumber.size(); j++){
							markreplacenumber[j]++;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < curvenodelist.size(); i++){
		if (markreplace[i] < 0){
			NODE node_ = curvenodelist[i];
			curvenodelist2.push_back(node_);
		}
	}
	for (int i = 0; i < segmentlist.size(); i++){
		int i1 = segmentlist[i].node(0);
		int i2 = segmentlist[i].node(1);
		if (markreplace[i1] >= 0){
			i1 = markreplace[i1];
		}
		if (markreplace[i2] >= 0){
			i2 = markreplace[i2];
		}
		i1 = i1 - markreplacenumber[i1];
		i2 = i2 - markreplacenumber[i2];
		segment_.node(0, i1);
		segment_.node(1, i2);
		segmentlist2.push_back(segment_);
	}

	region.readincurves_left(curvenodelist2, segmentlist2); //f-b-l-r


	//for right boundary 
	segmentlist.clear();
	segmentlist2.clear();
	curvenodelist.clear();
	curvenodelist2.clear();
	markreplace.clear();
	markreplacenumber.clear();
	
	cc = 0;
	for (int ic = 0; ic < right_boundary_curves.size(); ic++){
		for (int iv = 0; iv < right_boundary_curves[ic].vertex_list.size() / 3; iv++){
			node_.x(right_boundary_curves[ic].vertex_list[iv * 3]);
			node_.y(right_boundary_curves[ic].vertex_list[iv * 3 + 1]);
			node_.z(right_boundary_curves[ic].vertex_list[iv * 3 + 2]);
			curvenodelist.push_back(node_);
		}
		for (int iv = 0; iv < right_boundary_curves[ic].edge_list.size() / 2; iv++){
			segment_.node(0, right_boundary_curves[ic].edge_list[iv * 2] + cc);
			segment_.node(1, right_boundary_curves[ic].edge_list[iv * 2 + 1] + cc);
			segmentlist.push_back(segment_);
		}
		cc = cc + right_boundary_curves[ic].vertex_list.size() / 3;
	}

	markreplace.resize(curvenodelist.size());
	markreplacenumber.resize(curvenodelist.size());
	for (int i = 0; i < markreplace.size(); i++){
		markreplace[i] = -1;
		markreplacenumber[i] = 0;
	}
	for (int inode = 0; inode < curvenodelist.size(); inode++){
		if (markreplace[inode] < 0){
			double x = curvenodelist[inode].x();
			double y = curvenodelist[inode].y();
			double z = curvenodelist[inode].z();
			for (int i = 0; i < curvenodelist.size(); i++){
				double xx = curvenodelist[i].x();
				double yy = curvenodelist[i].y();
				double zz = curvenodelist[i].z();
				if (std::abs(x - xx) < 0.0001 && std::abs(y - yy) < 0.0001 && std::abs(z - zz) < 0.0001){
					if (inode != i && markreplace[inode] <0){
						markreplace[i] = inode;//remove i
						//markreplace[inode] = -2;
						for (int j = i + 1; j < markreplacenumber.size(); j++){
							markreplacenumber[j]++;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < curvenodelist.size(); i++){
		if (markreplace[i] < 0){
			NODE node_ = curvenodelist[i];
			curvenodelist2.push_back(node_);
		}
	}
	for (int i = 0; i < segmentlist.size(); i++){
		int i1 = segmentlist[i].node(0);
		int i2 = segmentlist[i].node(1);
		if (markreplace[i1] >= 0){
			i1 = markreplace[i1];
		}
		if (markreplace[i2] >= 0){
			i2 = markreplace[i2];
		}
		i1 = i1 - markreplacenumber[i1];
		i2 = i2 - markreplacenumber[i2];
		segment_.node(0, i1);
		segment_.node(1, i2);
		segmentlist2.push_back(segment_);
	}

	region.readincurves_right(curvenodelist2, segmentlist2); //f-b-l-r

	return true;
}


void FlowDiagnosticsInterface::getOilInPlace(double &oil_in_place)
{
    oil_in_place = region.oilinplace();
}


void FlowDiagnosticsInterface::setSaturationMethod(int i){
    region.saturationflag(i);
}
void FlowDiagnosticsInterface::setPermeabilityCurve(unsigned int region_id, int pcurve_id)
{
    return;
}

void FlowDiagnosticsInterface::getAvailablePermeabilityCurves(std::vector<int> &pcurves_ids)
{
    pcurves_ids = { 0, 1, 2, 3 };
    return;
}

#include <cmath>




void FlowDiagnosticsInterface::setRegion(unsigned int id, double x, double y, double z,
    double min_perm, double max_perm,
    double min_poros, double max_poros) //the one used
{


    min_perm = min_perm > 0 ? min_perm : 0;
    min_poros = min_poros > 0 ? min_poros : 0;

     PROPERTYAREA p;
    p.x(x);
    p.y(y);
    p.z(z);
    p.permlow(min_perm*0.987e-15);
    p.permhigh(max_perm*0.987e-15);
    p.porolow(min_poros);
    p.porohigh(max_poros);
    region.modifypropertyarea(id, p);

    return;
}

void FlowDiagnosticsInterface::setRegionSinglePhase(unsigned int id, double Kxlow_, double Kylow_, double Kzlow_, double Kxhigh_,
	double Kyhigh_, double Kzhigh_, double porolow_, double porohigh_) //the one NOT used
{
	PROPERTYAREA p;
	p.Kxlow(Kxlow_*0.987e-15);
	p.Kxhigh(Kxhigh_*0.987e-15);
	p.Kylow(Kylow_*0.987e-15);
	p.Kyhigh(Kyhigh_*0.987e-15);
	p.Kzlow(Kzlow_*0.987e-15);
	p.Kzhigh(Kzhigh_*0.987e-15);
	p.porolow(porolow_);
	p.porohigh(porohigh_);
	region.modifypropertyarea(id, p);

	return;
}

void FlowDiagnosticsInterface::setRegionMultiphase(unsigned int id, double Kxlow_, double Kylow_, double Kzlow_, double Kxhigh_,
	double Kyhigh_, double Kzhigh_, double porolow_, double porohigh_,
	double Pct_, double Si_, double lambda_, double Sw_) //the one used
{
	PROPERTYAREA p;
	p.Kxlow(Kxlow_*0.987e-15);
	p.Kxhigh(Kxhigh_*0.987e-15);
	p.Kylow(Kylow_*0.987e-15);
	p.Kyhigh(Kyhigh_*0.987e-15);
	p.Kzlow(Kzlow_*0.987e-15);
	p.Kzhigh(Kzhigh_*0.987e-15);
	p.porolow(porolow_);
	p.porohigh(porohigh_);
	p.Pct(Pct_);
	p.Siw(Si_);
	p.lamda(lambda_);
	p.Sw(Sw_);
	region.modifypropertyarea(id, p);

	return;
}

//void FlowDiagnosticsInterface::setRegion(unsigned int id,
//	double min_perm, double max_perm,
//	double min_poros, double max_poros) 
//{
//
//	min_perm = min_perm > 0 ? min_perm : 0;
//	min_poros = min_poros > 0 ? min_poros : 0;
//
//	PROPERTYAREA p;
//	p.permlow(min_perm*0.987e-15);
//	p.permhigh(max_perm*0.987e-15);
//	p.porolow(min_poros);
//	p.porohigh(max_poros);
//	region.modifypropertyarea(id, p);
//
//	return;
//}


void FlowDiagnosticsInterface::setOilGravity(double g){
    region.oildensity(g);
}

void FlowDiagnosticsInterface::setNumberOfPhases(int i){
    if ( i == 1 )
    {
        multi_phase_flow = false;
    }
    else if ( i > 1 )
    {
        multi_phase_flow = true;
    }

    region.numberofphases(i);
}

int FlowDiagnosticsInterface::getNumberOfPhases()
{
	return region.numberofphases();
}


///@TODO Septemeber
///New visualization entities
bool FlowDiagnosticsInterface::getCapillaryPressure(std::vector< double >& values) {
    if ( multi_phase_flow == false )
    {
        return false;
    }

    values = region.getpc();

    return true;
}

bool FlowDiagnosticsInterface::getWaterSaturationByCells(std::vector< double >& values) {
    if ( multi_phase_flow == false )
    {
        return false;
    }

    values = region.getwatersaturation_elements();

    return true;
}

bool FlowDiagnosticsInterface::getKroByCells(std::vector< double >& values) {
    if ( multi_phase_flow == false )
    {
        return false;
    }

    values = region.getkro_elements();

    return true;
}

bool FlowDiagnosticsInterface::getKrwByCells(std::vector< double >& values) {
    if ( multi_phase_flow == false )
    {
        return false;
    }

    values = region.getkrw_elements();

    return true;
}



void FlowDiagnosticsInterface::setWell(unsigned int id, unsigned int type, double value, int sign){
	WELL w;
	if (type == 1 || type == 3){//type should be 1 in this version
		value = value*1.0e+5;
	}
	w.type(type);
	w.sign(sign);
	w.value(value);
	region.modifywell(id, w);
}

void FlowDiagnosticsInterface::loadWellsGeometry(char* file){
	region.readwellgeometry(file);
}

void FlowDiagnosticsInterface::getWell(unsigned int id, unsigned int& type, double& value, int& sign){
	type = region.well(id).type();
	value = region.well(id).value();
	sign = region.well(id).sign();
}

void FlowDiagnosticsInterface::setfreewaterlevel(double d){//not in gui yet
	region.setfreewaterlevel(d);
}


void FlowDiagnosticsInterface::setSkeletonData(unsigned int surfaces_number, const std::vector< unsigned int >& nu,
	const std::vector< unsigned int >& nv, const std::vector< double >& positions){
	region.setskeletondata(surfaces_number, nu, nv, positions);
}


void FlowDiagnosticsInterface::getSurfaceSkeleton(unsigned int& surfaces_number, std::vector< unsigned int >& nu,
	std::vector< unsigned int >& nv, std::vector< double >& positions) const
{
	std::vector<PARASURFACE> parasurfacelist;
	parasurfacelist = region.getparasurfacelist();
	surfaces_number = parasurfacelist.size();

	PARASURFACE parasurface_;
	UVNODE uvnode_;

	for (int k = 0; k < surfaces_number; k++){


		parasurface_ = parasurfacelist[k];

		int nu_ = parasurface_.nu();
		int nv_ = parasurface_.nv();


		nu.push_back(nu_);
		nv.push_back(nv_);

		int l = 0;
		for (int j = 0; j < nv_; j++){
			for (int i = 0; i < nu_; i++){

				uvnode_ = parasurface_.uvnode(l);

				float x = uvnode_.x();
				float y = uvnode_.y();
				float z = uvnode_.z();

				positions.push_back(x);
				positions.push_back(y);
				positions.push_back(z);


				++l;
			}
		}

	}
}

void FlowDiagnosticsInterface::computeProperties(){
	if (region.meshinfo_type() == 1){
		region.steadystateflowsolver();
		region.flowdiagnostics_tracing();
		//region.writeresult_cvfe_sf_log2t("result.vtk");
		region.derivedquantities_both();
		//region.writeflowdiagnostics("flowdiagnostics.txt");
	}
}


/* The following methods were included for the June 2017 RRM integration. */


bool FlowDiagnosticsInterface::setVerticalWell(unsigned int id, WellType t, double pressure_value,
	double qt_x, double qt_y, double topd_, double botd_) //use this one
{
	switch (t)
	{
	case INJECTOR:
		/* Set an INJECTOR well here */
		setWell(id, 1, pressure_value, 1);
		break;

	case PRODUCER:
		/* Set an PRODUCER well here */
		setWell(id, 1, pressure_value, -1);
		break;

	default:
		break;
	}
	region.setverticalwellgeometry(id, qt_x, qt_y, topd_, botd_);
	return true;
}

//functions that are not being used

bool FlowDiagnosticsInterface::setWell(unsigned int id, WellType t, double pressure_value,
	double qt_x, double qt_y, double qt_z, double well_depth) //not being used; please use setverticalwell with two sides slider
{
	switch (t)
	{
	case INJECTOR:
		/* Set an INJECTOR well here */
		setWell(id, 1, pressure_value, 1);
		break;

	case PRODUCER:
		/* Set an INJECTOR well here */
		setWell(id, 1, pressure_value, -1);
		break;

	default:
		break;
	}

	region.setverticalwellgeometry(id, qt_x, qt_y, 0, 0 + well_depth);
	return true;
}



//void FlowDiagnosticsInterface::setRegion(unsigned int id, double x, double y, double z, double perm,
//	double poros){
//	//not being used
//	PROPERTYAREA p;
//	p.x(x);
//	p.y(y);
//	p.z(z);
//	p.permlow(perm*0.987e-15);
//	p.permhigh(perm*0.987e-15);
//
//	p.porolow(poros);
//	p.porohigh(poros);
//	region.modifypropertyarea(id, p);
//}

void FlowDiagnosticsInterface::getRegion(unsigned int id, double& x, double& y, double& z, double& perm,
	double &poros, double& visc, double &porevolume) { //not being used
	x = region.propertyarea(id).x();
	y = region.propertyarea(id).y();
	z = region.propertyarea(id).z();
	perm = region.propertyarea(id).permlow() / 0.987e-15;

	// The method: propertyarea(int).potosity() is not available anymore
	//poros = region.propertyarea(id).porosity();
	poros = region.propertyarea(id).porolow();//if porolow and porohigh are both needed then get both

	visc = region.oilviscosity();
	porevolume = region.propertyarea(id).porevolume();
}


void FlowDiagnosticsInterface::readSkeletonFile(const std::string& skeleton_file){//not being used
	region.readskeleton(skeleton_file.c_str());
}

void FlowDiagnosticsInterface::getSurfaceVertices(std::vector< float >& vertices) const {}//not being used


void FlowDiagnosticsInterface::getSurfaceFaces(std::vector< unsigned int >& faces) const {}//not being used

/* Allow changing the linear solver for Zhao's flow diagnostics */

bool FlowDiagnosticsInterface::setSolver(SolverType solver)
{
	return false;
}
