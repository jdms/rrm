#include "FlowDiagnosticsInterface.hpp"



FlowDiagnosticsInterface::FlowDiagnosticsInterface(){}


void FlowDiagnosticsInterface::loadDefaultValues()
{
    
}


void FlowDiagnosticsInterface::setNumberofRegions( unsigned int regions_number ){
	region.setnumberofpropertyareas(regions_number);
}

void FlowDiagnosticsInterface::setRegion( unsigned int id, double x, double y, double z, double perm,
                                          double poros, double visc ){
	PROPERTYAREA p;
	p.x(x);
	p.y(y);
	p.z(z);
	p.perm(perm);
	p.porosity(poros);
	p.viscosity(visc);
	region.modifypropertyarea(id, p);
}



unsigned int FlowDiagnosticsInterface::getNumberofRegions() {
	return region.numberofpropertyareas();
}

void FlowDiagnosticsInterface::getRegion( unsigned int id, double& x, double& y, double& z, double& perm,
                          double &poros, double& visc )
{
//	return(region.propertyarea(id));
}


void FlowDiagnosticsInterface::clearRegions(){
	region.clearpropertyareas();
}


void FlowDiagnosticsInterface::setNumberofWells( unsigned int wells_number ){
	region.setnumberofwells(wells_number);
}

void FlowDiagnosticsInterface::setWell( unsigned int id, unsigned int type, double value, int sign ){
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
    region.inputwells(file);
}




unsigned int FlowDiagnosticsInterface::getNumberofWells() { 
	return region.numberofwells(); 
}

void FlowDiagnosticsInterface::getWell( unsigned int id, unsigned int& type, double& value, int& sign ){
	region.well(id);
}


void FlowDiagnosticsInterface::clearWells(){
	region.clearwells();
}


void FlowDiagnosticsInterface::readUserInput( const std::string& input_file )
{
//	region.userinput(input_file);
}


void FlowDiagnosticsInterface::readSkeletonFile( const std::string& skeleton_file ){
//	region.readskeleton(skeleton_file);
}


void FlowDiagnosticsInterface::setSkeletonData( unsigned int surfaces_number, const std::vector< unsigned int >& nu,
                                                const std::vector< unsigned int >& nv, const std::vector< double >& positions ){
	region.setskeletondata(surfaces_number, nu, nv, positions);
}



void FlowDiagnosticsInterface::getSurfaceSkeleton( unsigned int& surfaces_number, std::vector< unsigned int >& nu,
                         std::vector< unsigned int >& nv, std::vector< double >& positions ) const
{
}



void FlowDiagnosticsInterface::buildVolumetricMesh(){
	region.meshinfo_type(1);
	region.unstructuredsurfacemesh();
	region.unstructuredvolumemesh();
}

void FlowDiagnosticsInterface::buildCPGVolumetricMesh(){
	region.meshinfo_type(2);
	region.cornerpointgrid();
}




void FlowDiagnosticsInterface::init(){

	if (region.meshinfo_type() == 1){ //unstructured
		region.tetrahedralmesh_postprocessCVFE();
		region.properties_tetcvfe();
		region.boundarycondition();
		region.wellcondition(); //well boundary condition overwrite surface boundary condition
		region.computeporevolume();
	}
	else if (region.meshinfo_type() == 2){ //CPG
		region.properties_cpgfv();
		region.wellcondition_cpgfv();
		region.transmatrix_cpgfv();
	}
}

void FlowDiagnosticsInterface::flowdiagnostics(){
	region.steadystateflowsolver();
	region.flowdiagnostics();
	region.derivedquantities_all();
}

void FlowDiagnosticsInterface::getPressure( std::vector< double >& values ) {
	std::vector<double> p;
	int i;
	for (i = 0; i < region.nodevector().size(); i++){
		p.push_back(region.nodevector()[i].P());
	}
//	return p;
}


//void FlowDiagnosticsInterface::getVelocitybyVertices( std::vector< double >& values ) const {}


void FlowDiagnosticsInterface::getVelocitybyCells( std::vector< double >& values ) {
	std::vector<double> v;
	int i;
	for (i = 0; i < region.elementvector().size(); i++){
		v.push_back(region.elementvector()[i].Ux());
		v.push_back(region.elementvector()[i].Uy());
		v.push_back(region.elementvector()[i].Uz());
	}
//	return v;
}


void FlowDiagnosticsInterface::getBackwardTOF( std::vector< double >& values ) {
	std::vector<double> v;
	int i;
	for (i = 0; i < region.nodevector().size(); i++){
		v.push_back(region.nodevector()[i].tof_back());
	}
//	return v;
}


void FlowDiagnosticsInterface::getForwardTOF( std::vector< double >& values ) {
	std::vector<double> v;
	int i;
	for (i = 0; i < region.nodevector().size(); i++){
		v.push_back(region.nodevector()[i].tof());
	}
//	return v;
}


void FlowDiagnosticsInterface::getTotalTOF( std::vector< double >& values ){
	std::vector<double> v;
	int i;
	for (i = 0; i < region.nodevector().size(); i++){
		v.push_back(region.nodevector()[i].tof_total());
	}
//	return v;
}


void FlowDiagnosticsInterface::getMaxBackwardTracer( std::vector< double >& values ){
	std::vector<double> v;
	int i;
	for (i = 0; i < region.nodevector().size(); i++){
		v.push_back(region.nodevector()[i].markmaxtracer_back());
	}
//	return v;
}


void FlowDiagnosticsInterface::getMaxForwardTracer( std::vector< double >& values ) {
	std::vector<double> v;
	int i;
	for (i = 0; i < region.nodevector().size(); i++){
		v.push_back(region.nodevector()[i].markmaxtracer());
	}
//	return v;
}


void FlowDiagnosticsInterface::getPermeability( std::vector< double >& values ){
	std::vector<double> v;
	int i;
	for (i = 0; i < region.nodevector().size(); i++){
		v.push_back(region.nodevector()[i].markmaxtracer());
	}
//	return v;
}


void FlowDiagnosticsInterface::getRegionId( std::vector< int >& regions_id ) {
	std::vector<int> v;
	int i;
	for (i = 0; i < region.elementvector().size(); i++){
		v.push_back(region.elementvector()[i].markregion());
	}
//	return v;
}



void FlowDiagnosticsInterface::getFlowDiagnosticInformation(){

}


void FlowDiagnosticsInterface::getCPGPressure( std::vector< double >& values ) {
	std::vector<double> v;
	int i;
	for (i = 0; i < region.cpgelementvector().size(); i++){
		v.push_back(region.cpgelementvector()[i].press());
	}
//	return v;
}


void FlowDiagnosticsInterface::getCPGTOF( std::vector< double >& values ) {
	std::vector<double> v;
	int i;
	for (i = 0; i < region.cpgelementvector().size(); i++){
		v.push_back(region.cpgelementvector()[i].tof());
	}
//	return v;
}

void FlowDiagnosticsInterface::getCPGBackwardTOF( std::vector< double >& values ) {
	std::vector<double> v;
	int i;
	for (i = 0; i < region.cpgelementvector().size(); i++){
		v.push_back(region.cpgelementvector()[i].tof_back());
	}
//	return v;
}


void FlowDiagnosticsInterface::getCPGTotalTOF( std::vector< double >& values ) {
	std::vector<double> v;
	int i;
	for (i = 0; i < region.cpgelementvector().size(); i++){
		v.push_back(region.cpgelementvector()[i].tof_total());
	}
//	return v;
}

void FlowDiagnosticsInterface::getTracer( std::vector< double >& values ) {
	std::vector<int> v;
	int i;
	for (i = 0; i < region.cpgelementvector().size(); i++){
		v.push_back(region.cpgelementvector()[i].markmaxtracer());
	}
//	return v;
}

void FlowDiagnosticsInterface::geBackwardTracer( std::vector< double >& values ) {
	std::vector<int> v;
	int i;
	for (i = 0; i < region.cpgelementvector().size(); i++){
		v.push_back(region.cpgelementvector()[i].markmaxtracer_back());
	}
//	return v;
}


void FlowDiagnosticsInterface::getSurfaceVertices( std::vector< float >& vertices ) {}


void FlowDiagnosticsInterface::getSurfaceFaces( std::vector< unsigned int >& faces ) {}




void FlowDiagnosticsInterface::getVolumeVertices( std::vector< float >& vertices ) {
//	return region.nodevector();
}

void FlowDiagnosticsInterface::getVolumeEdges( std::vector< unsigned int >& edges ) {
//	return region.edgevector();
}

void FlowDiagnosticsInterface::getVolumeCells( std::vector< unsigned int >& cells ) {
//	return region.elementvector();
}

void FlowDiagnosticsInterface::getCPGVolumeVertices( std::vector< float >& vertices ){
//	return region.nodevector();
}

void FlowDiagnosticsInterface::getCPGVolumeEdges( std::vector< unsigned int >& edges ) {}

void FlowDiagnosticsInterface::getCPGVolumeCells( std::vector< unsigned int >& cells ) {
//	return region.cpgelementvector();
}


void FlowDiagnosticsInterface::exportSurfacetoVTK( const std::string& filename ){
//	region.writesurfacemeshVTK(filename);

}

void FlowDiagnosticsInterface::exportCornerPointtoVTK( const std::string& filename ){
//	region.writecornerpointgridVTK(filename);
}

void FlowDiagnosticsInterface::exportCornerPointtoGRDECL( const std::string& filename ){
//	region.writecornerpointgridGRDECL(filename);
}

void FlowDiagnosticsInterface::exportResultstoVTK( const std::string& filename ){
//	region.writeresult(filename);
}


void FlowDiagnosticsInterface::clear(){}


