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

#include "FlowDiagnosticsInterface.hpp"


FlowDiagnosticsInterface::FlowDiagnosticsInterface(){}


void FlowDiagnosticsInterface::loadDefaultValues(int i)// 1 for unstructured 2 for cpg
{
	if (i == 1){
        region1.RRMdefaultvalues_unstructured();
	}
	else if (i == 2){
        region1.RRMdefaultvalues_cpg();
	}
}




void FlowDiagnosticsInterface::setNumberofRegions( unsigned int regions_number ){
    region1.setnumberofpropertyareas(regions_number);
}

void FlowDiagnosticsInterface::setRegion( unsigned int id, double x, double y, double z, double perm,
                                          double poros, double visc ){

    PROPERTYAREA p;
    p.x(x);
    p.y(y);
    p.z(z);
    p.perm(perm*0.987e-15);
    p.porosity(poros);
    p.viscosity(visc*1.0e-3);
    region1.modifypropertyarea(id, p);
}



unsigned int FlowDiagnosticsInterface::getNumberofRegions() const {
    return region1.numberofpropertyareas();
}


void FlowDiagnosticsInterface::getRegion( unsigned int id, double& x, double& y, double& z, double& perm,
                                          double &poros, double& visc, double &porevolume ) {
    x = region1.propertyarea(id).x();
    y = region1.propertyarea(id).y();
    z = region1.propertyarea(id).z();
    perm = region1.propertyarea(id).perm()/0.987e-15;
    poros = region1.propertyarea(id).porosity();
    visc = region1.propertyarea(id).viscosity();
    porevolume = region1.propertyarea(id).porevolume();
}


void FlowDiagnosticsInterface::clearRegions(){
    region1.clearpropertyareas();
}




void FlowDiagnosticsInterface::setNumberofWells( unsigned int wells_number ){
    region1.setnumberofwells(wells_number);
}

void FlowDiagnosticsInterface::setWell( unsigned int id, unsigned int type, double value, int sign ){
    WELL w;
    if (type == 1 || type == 3){//type should be 1 in this version
        value = value*1.0e+5;
    }
    w.type(type);
    w.sign(sign);
    w.value(value);
    region1.modifywell(id, w);
}


void FlowDiagnosticsInterface::loadWellsGeometry(char* file){
    region1.inputwells(file);
}




unsigned int FlowDiagnosticsInterface::getNumberofWells() const { return region1.numberofwells(); }

//here
void FlowDiagnosticsInterface::getWell( unsigned int id, unsigned int& type, double& value, int& sign ){
    type = region1.well(id).type();
    value = region1.well(id).value();
    sign = region1.well(id).sign();
}


void FlowDiagnosticsInterface::clearWells(){
    region1.clearwells();
}


void FlowDiagnosticsInterface::readUserInput(const std::string& input_file ){
    region1.userinput(input_file.c_str());
}


void FlowDiagnosticsInterface::readSkeletonFile( const std::string& skeleton_file ){
    region1.readskeleton(skeleton_file.c_str());
}



void FlowDiagnosticsInterface::setSkeletonData( unsigned int surfaces_number, const std::vector< unsigned int >& nu,
                                                const std::vector< unsigned int >& nv, const std::vector< double >& positions ){
    region1.setskeletondata(surfaces_number, nu, nv, positions);
}


void FlowDiagnosticsInterface::getSurfaceSkeleton( unsigned int& surfaces_number, std::vector< unsigned int >& nu,
                         std::vector< unsigned int >& nv, std::vector< double >& positions ) const
{
    surfaces_number = region1.parasurfacelist.size();

    PARASURFACE parasurface_;
    UVNODE uvnode_;

    for ( int k = 0; k < surfaces_number; k++){


        parasurface_ = region1.parasurfacelist[ k ];

        int nu_ = parasurface_.nu();
        int nv_ = parasurface_.nv();


        nu.push_back( nu_ );
        nv.push_back( nv_ );

        int l = 0;
        for ( int j = 0; j < nv_; j++){
            for ( int i = 0; i < nu_; i++){

                uvnode_ = parasurface_.uvnode( l );

                float x = uvnode_.x();
                float y = uvnode_.y();
                float z = uvnode_.z();

                positions.push_back( x );
                positions.push_back( y );
                positions.push_back( z );


                ++l;
            }
        }

    }
}



void FlowDiagnosticsInterface::init(){
    region1.wellmethod(2);
    if (region1.meshinfo_type() == 1){ //unstructured
        region1.tetrahedralmesh_postprocessCVFE();
        region1.properties_tetcvfe();
        region1.boundarycondition();
        region1.wellcondition(); //well boundary condition overwrite surface boundary condition
        region1.computeporevolume();
    }
    else if (region1.meshinfo_type() == 2){ //CPG
        region1.properties_cpgfv();
        region1.wellcondition_cpgfv();
        region1.transmatrix_cpgfv();
    }
}


void FlowDiagnosticsInterface::setVolumeDimensions( double width, double height, double depth )
{
    region1.setvolumedimension(width, height, depth);
}


void FlowDiagnosticsInterface::computeProperties(){
    region1.steadystateflowsolver();
    region1.flowdiagnostics();
    //if (region1.meshinfo_type() == 1){ //only triggered when we can visualise them.
    //	region1.derivedquantities_all();
	//}
    //else if (region1.meshinfo_type() == 2){
    //	region1.derivedquantities_cpgfv();
	//}
}


void FlowDiagnosticsInterface::buildVolumetricMesh(){
    region1.meshinfo_type(1);
    region1.unstructuredsurfacemesh();
    region1.unstructuredvolumemesh();
}


void FlowDiagnosticsInterface::buildCPGVolumetricMesh(){
    region1.meshinfo_type(2);
    region1.cornerpointgrid();
}

void FlowDiagnosticsInterface::getPressure( std::vector< double >& values ) {
    values = region1.getpressure();
}


void FlowDiagnosticsInterface::getVelocitybyVertices( std::vector< double >& values ) {
    values = region1.getvelocity_nodes();
}


void FlowDiagnosticsInterface::getVelocitybyCells( std::vector< double >& values ) {
    values = region1.getvelocity_elements();
}


void FlowDiagnosticsInterface::getBackwardTOF( std::vector< double >& values ) {
    values = region1.getbackwardtof();
}


void FlowDiagnosticsInterface::getForwardTOF( std::vector< double >& values ) {
    values = region1.getforwardtof();
}


void FlowDiagnosticsInterface::getTotalTOF( std::vector< double >& values ) {
    values = region1.gettotaltof();
}


void FlowDiagnosticsInterface::getMaxBackwardTracer( std::vector< double >& values ) {
    values = region1.getmaxbackwardtracer();
}


void FlowDiagnosticsInterface::getMaxForwardTracer( std::vector< double >& values ) {
    values = region1.getmaxforwardtracer();
}


void FlowDiagnosticsInterface::getPermeabilitybyCells( std::vector< double >& values ) {
    values=region1.getpermeability_elements();
}


void FlowDiagnosticsInterface::getPermeabilitybyVertices( std::vector< double >& values ) {
    values = region1.getpermeability_nodes();
}

void FlowDiagnosticsInterface::getPorosity( std::vector< double >& values )
{
    values = region1.getporosity_elements();
}


void FlowDiagnosticsInterface::getPoreVolumebyRegion( std::vector< double >& values )
{
    values = region1.getporevolume();
}



void FlowDiagnosticsInterface::getRegionId( std::vector< int >& regions_id ) {
    regions_id = region1.getregionid();
}



void FlowDiagnosticsInterface::getFlowDiagnosticInformation(){
    region1.writeflowdiagnostics("fd.txt", "fd_F.txt", "fd_Phi.txt");
}


void FlowDiagnosticsInterface::getCPGPressure( std::vector< double >& values ) {
    values = region1.getpressure_cpg();
}


void FlowDiagnosticsInterface::getCPGTOF( std::vector< double >& values ) {
    values = region1.getforwardtof_cpg();
}


void FlowDiagnosticsInterface::getCPGBackwardTOF( std::vector< double >& values ) {
    values = region1.getbackwardtof_cpg();
}


void FlowDiagnosticsInterface::getCPGTotalTOF( std::vector< double >& values ) {
    values = region1.gettotaltof_cpg();
}


void FlowDiagnosticsInterface::getCPGMaxForwardTracer(std::vector< double >& values) {
    values = region1.getmaxforwardtracer_cpg();
}

void FlowDiagnosticsInterface::getCPGMaxBackwardTracer( std::vector< double >& values ) {
    values = region1.getmaxbackwardtracer_cpg();
}


void FlowDiagnosticsInterface::getCPGPermeability( std::vector< double >& values ) {
    values=region1.getpermeability_elements_cpg();
}


void FlowDiagnosticsInterface::getCPGPorosity( std::vector< double >& values )
{
    values = region1.getporosity_elements_cpg();
}

void FlowDiagnosticsInterface::getCPGVelocity( std::vector< double >& values )
{

}


void FlowDiagnosticsInterface::getSurfaceVertices( std::vector< float >& vertices ) const {}


void FlowDiagnosticsInterface::getSurfaceFaces( std::vector< unsigned int >& faces ) const {}


void FlowDiagnosticsInterface::getVolumeVertices( std::vector< float >& vertices )
{
    int number_of_points = (int)region1.nodelist.size();

    for( int i = 0; i < number_of_points; ++i )
    {
        vertices.push_back( region1.nodelist[ i ].x() );
        vertices.push_back( region1.nodelist[ i ].y() );
        vertices.push_back( region1.nodelist[ i ].z() );
    }

}

void FlowDiagnosticsInterface::getVolumeEdges( std::vector< unsigned int >& edges )
{

    int number_of_cells = (int)region1.elementlist.size();

    for( int i = 0; i < number_of_cells; ++i )
    {
        TETRAHEDRON tet = region1.elementlist[ i ];

        edges.push_back( ( unsigned int ) tet.node( 0 ) );
        edges.push_back( ( unsigned int ) tet.node( 1 ) );

        edges.push_back( ( unsigned int ) tet.node( 1 ) );
        edges.push_back( ( unsigned int ) tet.node( 3 ) );

        edges.push_back( ( unsigned int ) tet.node( 3 ) );
        edges.push_back( ( unsigned int ) tet.node( 0 ) );

        edges.push_back( ( unsigned int ) tet.node( 1 ) );
        edges.push_back( ( unsigned int ) tet.node( 2 ) );

        edges.push_back( ( unsigned int ) tet.node( 2 ) );
        edges.push_back( ( unsigned int ) tet.node( 3 ) );

        edges.push_back( ( unsigned int ) tet.node( 2 ) );
        edges.push_back( ( unsigned int ) tet.node( 0 ) );

        edges.push_back( ( unsigned int ) tet.node( 3 ) );
        edges.push_back( ( unsigned int ) tet.node( 2 ) );
    }


}


void FlowDiagnosticsInterface::getVolumeCells( std::vector< unsigned int >& cells )
{
    int number_of_cells = (int)region1.elementlist.size();


    for( int i = 0; i < number_of_cells; ++i )
    {
        TETRAHEDRON tet = region1.elementlist[ i ];

        cells.push_back( ( unsigned int ) tet.node( 0 ) );
        cells.push_back( ( unsigned int ) tet.node( 1 ) );
        cells.push_back( ( unsigned int ) tet.node( 2 ) );
        cells.push_back( ( unsigned int ) tet.node( 3 ) );
    }


}




void FlowDiagnosticsInterface::getCPGVolumeVertices( std::vector< float >& vertices )
{


    for (int i = 0; i < region1.nodelist.size(); i++){
        vertices.push_back((float)region1.nodelist[i].x());
        vertices.push_back((float)region1.nodelist[i].y());
        vertices.push_back((float)region1.nodelist[i].z());
        std::cout << " x " << (float)region1.nodelist[i].x() << std::endl;
    }


}



void FlowDiagnosticsInterface::getCPGVolumeEdges( std::vector< unsigned int >& edges )
{

    for (int i = 0; i < region1.cpgelementlist.size(); i++){


        unsigned int id0 = (unsigned int)region1.cpgelementlist[i].node(0);
        unsigned int id1 = (unsigned int)region1.cpgelementlist[i].node(1);
        unsigned int id2 = (unsigned int)region1.cpgelementlist[i].node(2);
        unsigned int id3 = (unsigned int)region1.cpgelementlist[i].node(3);
        unsigned int id4 = (unsigned int)region1.cpgelementlist[i].node(4);
        unsigned int id5 = (unsigned int)region1.cpgelementlist[i].node(5);
        unsigned int id6 = (unsigned int)region1.cpgelementlist[i].node(6);
        unsigned int id7 = (unsigned int)region1.cpgelementlist[i].node(7);


        edges.push_back( id0 );
        edges.push_back( id1 );

        edges.push_back( id1 );
        edges.push_back( id3 );

        edges.push_back( id3 );
        edges.push_back( id2 );

        edges.push_back( id2 );
        edges.push_back( id0 );


        edges.push_back( id4 );
        edges.push_back( id5 );

        edges.push_back( id5 );
        edges.push_back( id7 );

        edges.push_back( id7 );
        edges.push_back( id6 );

        edges.push_back( id6 );
        edges.push_back( id4 );


        edges.push_back( id0 );
        edges.push_back( id4 );

        edges.push_back( id1 );
        edges.push_back( id5 );


        edges.push_back( id3 );
        edges.push_back( id7 );

        edges.push_back( id2 );
        edges.push_back( id6 );


    }
}



void FlowDiagnosticsInterface::getCPGVolumeCells( std::vector< unsigned int >& cells )
{

    for (int i = 0; i < region1.cpgelementlist.size(); i++){


        unsigned int id0 = (unsigned int)region1.cpgelementlist[i].node(0);
        unsigned int id1 = (unsigned int)region1.cpgelementlist[i].node(1);
        unsigned int id2 = (unsigned int)region1.cpgelementlist[i].node(2);
        unsigned int id3 = (unsigned int)region1.cpgelementlist[i].node(3);
        unsigned int id4 = (unsigned int)region1.cpgelementlist[i].node(4);
        unsigned int id5 = (unsigned int)region1.cpgelementlist[i].node(5);
        unsigned int id6 = (unsigned int)region1.cpgelementlist[i].node(6);
        unsigned int id7 = (unsigned int)region1.cpgelementlist[i].node(7);

        cells.push_back( id0 );
        cells.push_back( id1 );
        cells.push_back( id5 );
        cells.push_back( id4 );


        cells.push_back( id1 );
        cells.push_back( id3 );
        cells.push_back( id7 );
        cells.push_back( id5 );

        cells.push_back( id2 );
        cells.push_back( id3 );
        cells.push_back( id7 );
        cells.push_back( id6 );

        cells.push_back( id2 );
        cells.push_back( id0 );
        cells.push_back( id4 );
        cells.push_back( id6 );


        cells.push_back( id2 );
        cells.push_back( id3 );
        cells.push_back( id1 );
        cells.push_back( id0 );

        cells.push_back( id4 );
        cells.push_back( id5 );
        cells.push_back( id7 );
        cells.push_back( id6 );


    }
}



void FlowDiagnosticsInterface::exportSurfacetoVTK( const std::string& filename ){
    region1.writesurfacemeshVTK(const_cast<char *>(filename.c_str()));
}

void FlowDiagnosticsInterface::exportCornerPointtoVTK( const std::string& filename ){
    region1.writecornerpointgridVTK(const_cast<char *>(filename.c_str()));
}

void FlowDiagnosticsInterface::exportCornerPointtoGRDECL( const std::string& filename ){
    region1.writecornerpointgridGRDECL(const_cast<char *>(filename.c_str()));
}

void FlowDiagnosticsInterface::exportResultstoVTK( const std::string& filename ){
    region1.writeresult(const_cast<char *>(filename.c_str()));
}


void FlowDiagnosticsInterface::clear(){
    region1.clearregion();
}




