#include "FlowDiagnosticsInterface.hpp"


FlowDiagnosticsInterface::FlowDiagnosticsInterface(){}


void FlowDiagnosticsInterface::loadDefaultValues()
{
    region.RRMdefaultvalues();
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



unsigned int FlowDiagnosticsInterface::getNumberofRegions() const {
    return region.numberofpropertyareas();
}


void FlowDiagnosticsInterface::getRegion( unsigned int id, double& x, double& y, double& z, double& perm,
                                          double &poros, double& visc ) {
    x = region.propertyarea(id).x();
    y = region.propertyarea(id).y();
    z = region.propertyarea(id).z();
    perm = region.propertyarea(id).perm();
    poros = region.propertyarea(id).porosity();
    visc = region.propertyarea(id).viscosity();
    //double porevolume = region.propertyarea(id).porevolume();
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




unsigned int FlowDiagnosticsInterface::getNumberofWells() const { return region.numberofwells(); }

//here
void FlowDiagnosticsInterface::getWell( unsigned int id, unsigned int& type, double& value, int& sign ){
    type = region.well(id).type();
    value = region.well(id).value();
    sign = region.well(id).sign();
}


void FlowDiagnosticsInterface::clearWells(){
    region.clearwells();
}


void FlowDiagnosticsInterface::readUserInput(const std::string& input_file ){
    region.userinput(input_file.c_str());
}


void FlowDiagnosticsInterface::readSkeletonFile( const std::string& skeleton_file ){
    region.readskeleton(skeleton_file.c_str());
}



void FlowDiagnosticsInterface::setSkeletonData( unsigned int surfaces_number, const std::vector< unsigned int >& nu,
                                                const std::vector< unsigned int >& nv, const std::vector< double >& positions ){
    region.setskeletondata(surfaces_number, nu, nv, positions);
}


void FlowDiagnosticsInterface::getSurfaceSkeleton( unsigned int& surfaces_number, std::vector< unsigned int >& nu,
                         std::vector< unsigned int >& nv, std::vector< double >& positions ) const
{
    surfaces_number = region.parasurfacelist.size();

    PARASURFACE parasurface_;
    UVNODE uvnode_;

    for ( int k = 0; k < surfaces_number; k++){


        parasurface_ = region.parasurfacelist[ k ];

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

void FlowDiagnosticsInterface::computeProperties(){
    region.steadystateflowsolver();
    region.flowdiagnostics();
    region.derivedquantities_all();
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

void FlowDiagnosticsInterface::getPressure( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.nodevector().size(); i++){
        values.push_back(region.nodevector()[i].P());
    }
}


void FlowDiagnosticsInterface::getVelocitybyVertices( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.nodevector().size(); i++){
        values.push_back(region.nodevector()[i].Ux());
        values.push_back(region.nodevector()[i].Uy());
        values.push_back(region.nodevector()[i].Uz());
    }
}


void FlowDiagnosticsInterface::getVelocitybyCells( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.elementvector().size(); i++){
        values.push_back(region.elementvector()[i].Ux());
        values.push_back(region.elementvector()[i].Uy());
        values.push_back(region.elementvector()[i].Uz());
    }
}

void FlowDiagnosticsInterface::getBackwardTOF( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.nodevector().size(); i++){
        values.push_back(region.nodevector()[i].tof_back());
    }
}


void FlowDiagnosticsInterface::getForwardTOF( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.nodevector().size(); i++){
        values.push_back(region.nodevector()[i].tof());
    }
}


void FlowDiagnosticsInterface::getTotalTOF( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.nodevector().size(); i++){
        values.push_back(region.nodevector()[i].tof_total());
    }
}


void FlowDiagnosticsInterface::getMaxBackwardTracer( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.nodevector().size(); i++){
        values.push_back(region.nodevector()[i].markmaxtracer_back());
    }
}


void FlowDiagnosticsInterface::getMaxForwardTracer( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.nodevector().size(); i++){
        values.push_back(region.nodevector()[i].markmaxtracer());
    }
}


void FlowDiagnosticsInterface::getPermeability( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.nodevector().size(); i++){
        values.push_back(region.nodevector()[i].markmaxtracer());
    }
}



void FlowDiagnosticsInterface::getRegionId( std::vector< int >& regions_id ) {
    int i;
    for (i = 0; i < region.elementvector().size(); i++){
        regions_id.push_back(region.elementvector()[i].markregion());
    }
}



void FlowDiagnosticsInterface::getFlowDiagnosticInformation(){
    region.writeflowdiagnostics("fd.txt", "fd_F.txt", "fd_Phi.txt");
}


void FlowDiagnosticsInterface::getCPGPressure( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.cpgelementvector().size(); i++){
        values.push_back(region.cpgelementvector()[i].press());
    }
}


void FlowDiagnosticsInterface::getCPGTOF( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.cpgelementvector().size(); i++){
        values.push_back(region.cpgelementvector()[i].tof());
    }
}


void FlowDiagnosticsInterface::getCPGBackwardTOF( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.cpgelementvector().size(); i++){
        values.push_back(region.cpgelementvector()[i].tof_back());
    }
}


void FlowDiagnosticsInterface::getCPGTotalTOF( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.cpgelementvector().size(); i++){
        values.push_back(region.cpgelementvector()[i].tof_total());
    }
}


//void FlowDiagnosticsInterface::getTracer( std::vector< double >& values ) {}
void FlowDiagnosticsInterface::getCPGMaxForwardTracer(std::vector< double >& values) {
    int i;
    for (i = 0; i < region.cpgelementvector().size(); i++){
        values.push_back(region.cpgelementvector()[i].markmaxtracer());
    }
}

void FlowDiagnosticsInterface::getCPGMaxBackwardTracer( std::vector< double >& values ) {
    int i;
    for (i = 0; i < region.cpgelementvector().size(); i++){
        values.push_back(region.cpgelementvector()[i].markmaxtracer_back());
    }
}


void FlowDiagnosticsInterface::getSurfaceVertices( std::vector< float >& vertices ) const {}

void FlowDiagnosticsInterface::getSurfaceFaces( std::vector< unsigned int >& faces ) const {}


void FlowDiagnosticsInterface::getVolumeVertices( std::vector< float >& vertices )
{
    int number_of_points = (int)region.nodelist.size();

    for( int i = 0; i < number_of_points; ++i )
    {
        vertices.push_back( region.nodelist[ i ].x() );
        vertices.push_back( region.nodelist[ i ].y() );
        vertices.push_back( region.nodelist[ i ].z() );
    }

}

void FlowDiagnosticsInterface::getVolumeEdges( std::vector< unsigned int >& edges )
{

    int number_of_cells = (int)region.elementlist.size();

    for( int i = 0; i < number_of_cells; ++i )
    {
        TETRAHEDRON tet = region.elementlist[ i ];

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
    int number_of_cells = (int)region.elementlist.size();


    for( int i = 0; i < number_of_cells; ++i )
    {
        TETRAHEDRON tet = region.elementlist[ i ];

        cells.push_back( ( unsigned int ) tet.node( 0 ) );
        cells.push_back( ( unsigned int ) tet.node( 1 ) );
        cells.push_back( ( unsigned int ) tet.node( 2 ) );
        cells.push_back( ( unsigned int ) tet.node( 3 ) );
    }


}




void FlowDiagnosticsInterface::getCPGVolumeVertices( std::vector< float >& vertices )
{


    for (int i = 0; i < region.nodelist.size(); i++){
        vertices.push_back((float)region.nodelist[i].x());
        vertices.push_back((float)region.nodelist[i].y());
        vertices.push_back((float)region.nodelist[i].z());
    }

}



void FlowDiagnosticsInterface::getCPGVolumeEdges( std::vector< unsigned int >& edges )
{

    for (int i = 0; i < region.cpgelementlist.size(); i++){


        unsigned int id0 = (unsigned int)region.cpgelementlist[i].node(0);
        unsigned int id1 = (unsigned int)region.cpgelementlist[i].node(1);
        unsigned int id2 = (unsigned int)region.cpgelementlist[i].node(2);
        unsigned int id3 = (unsigned int)region.cpgelementlist[i].node(3);
        unsigned int id4 = (unsigned int)region.cpgelementlist[i].node(4);
        unsigned int id5 = (unsigned int)region.cpgelementlist[i].node(5);
        unsigned int id6 = (unsigned int)region.cpgelementlist[i].node(6);
        unsigned int id7 = (unsigned int)region.cpgelementlist[i].node(7);


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

    for (int i = 0; i < region.cpgelementlist.size(); i++){


        unsigned int id0 = (unsigned int)region.cpgelementlist[i].node(0);
        unsigned int id1 = (unsigned int)region.cpgelementlist[i].node(1);
        unsigned int id2 = (unsigned int)region.cpgelementlist[i].node(2);
        unsigned int id3 = (unsigned int)region.cpgelementlist[i].node(3);
        unsigned int id4 = (unsigned int)region.cpgelementlist[i].node(4);
        unsigned int id5 = (unsigned int)region.cpgelementlist[i].node(5);
        unsigned int id6 = (unsigned int)region.cpgelementlist[i].node(6);
        unsigned int id7 = (unsigned int)region.cpgelementlist[i].node(7);

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
    region.writesurfacemeshVTK(filename.c_str());
}

void FlowDiagnosticsInterface::exportCornerPointtoVTK( const std::string& filename ){
    region.writecornerpointgridVTK(filename.c_str());
}

void FlowDiagnosticsInterface::exportCornerPointtoGRDECL( const std::string& filename ){
    region.writecornerpointgridGRDECL((char*)filename.c_str());
}

void FlowDiagnosticsInterface::exportResultstoVTK( const std::string& filename ){
    region.writeresult((char*)filename.c_str());
}


void FlowDiagnosticsInterface::clear(){
    region.clearregion();
}




