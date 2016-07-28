
#include "FlowVisualizationController.h"


FlowVisualizationController::FlowVisualizationController( QWidget *parent )
{
    mesh_ok = false;
    volumetric_ok = false;
    properties_computed = false;
    user_input_ok = false;

    current_method = MESHING_METHOD::UNSTRUCTURED;
//    region.resetStandardValues();

}


void FlowVisualizationController::readPolyFiles( const std::string& mesh_file )
{

    if( mesh_ok == true ) return;

    region.readsurfacemeshPOLY( (char *) mesh_file.c_str() );
    emit updatePolyMesh();
    mesh_ok = true;
}


void FlowVisualizationController::readSkeletonFiles( const std::string& all_filename/*, std::vector< double >& positions, std::vector< unsigned int >& faces */ )
{

    if( mesh_ok == true ) return;

    std::vector< int > nu;
    std::vector< int > nv;
    std::vector< double > positions;


    region.readskeleton( (char *) all_filename.c_str() );
    region.getSkeleton( nu, nv, positions );

    std::vector< unsigned int > faces;
    faces = getSkeletonFaces( nu, nv, positions );
    if( faces.empty() == true ) return;


    mesh_ok = true;
    emit updateMesh( Mesh::TYPE::QUADRILATERAL, positions, faces );

}


void FlowVisualizationController::readInputParameters( const std::string& input_file )
{
    region.userinput( (char *) input_file.c_str() );
    user_input_ok = true;
}


void FlowVisualizationController::generateCornerPoint()
{


    if( mesh_ok == false || user_input_ok == false ) return;

    counter.start( 0, 2 );
    region.cornerpointgrid();
    counter.update( 1 );
    emit updateCornerPoint();
    counter.update( 2 );

    volumetric_ok = true;

}


void FlowVisualizationController::generateUnstructured()
{

    if( mesh_ok == false || user_input_ok == false ) return;


    counter.start( 0, 4 );
    region.unstructuredsurfacemesh();
    counter.update( 1 );
    region.unstructuredvolumemesh();
    counter.update( 2 );
    region.modelpreparation();
    counter.update( 3 );
    emit updateVolumetricMesh();
    counter.update( 4 );

    volumetric_ok = true;


}


void FlowVisualizationController::getMeshVisualizationParameters(std::string& trianglecmd, int& resolutiontype, int& npartitionedge, double& lenghtedge )
{
    region.getVisualizationParameters( trianglecmd, resolutiontype, npartitionedge, lenghtedge);
}


void FlowVisualizationController::setMeshVisualizationParameters( const std::string& trianglecmd, const int& resolutiontype, const int& npartitionedge, const double& lenghtedge )
{
    region.setVisualizationParameters( trianglecmd, resolutiontype, npartitionedge, lenghtedge);
}



void FlowVisualizationController::updateMeshFromFile( Mesh* mesh )
{
   obj = region.getTetgenioObject();

   std::vector< float > vertices = getMeshVerticesfromFile( obj );
   std::vector< unsigned int > edges = getMeshWireFramefromFile( obj );
   std::vector< unsigned int > faces = getMeshFacesfromFile( obj );

   mesh->setMeshType( Mesh::TYPE::QUADRILATERAL );
   mesh->setVertices( vertices );
   mesh->setWireframe( edges );
   mesh->setFaces( faces );
   mesh->buildBoundingBox();



}


void FlowVisualizationController::updateMeshFromSurface( Mesh* mesh )
{

    std::vector< float > vertices;
    std::vector< unsigned int > faces;

    region.getSurface( vertices, faces );

    mesh->setMeshType( Mesh::TYPE::TRIANGLES );
    mesh->setVertices( vertices );
    mesh->setFaces( faces );
    mesh->buildBoundingBox();


}


void FlowVisualizationController::updateCornerPointFromSurface( Mesh* mesh )
{

    std::vector< float > vertices;
    std::vector< unsigned int > faces;
    std::vector< unsigned int > edges;

    region.getCornerPoint( vertices, edges, faces );

    mesh->setMeshType( Mesh::TYPE::QUADRILATERAL );
    mesh->setVertices( vertices );
    mesh->setWireframe( edges );
    mesh->setFaces( faces );
    mesh->buildBoundingBox();


}



void FlowVisualizationController::setCounterProgressinData()
{
    region.setProgressCounter( &counter );

    connect( &counter, SIGNAL( startProcess( const unsigned int&, const unsigned int& ) ), controllerParent, SLOT( startProgressBar( const unsigned int&, const unsigned int& ) ) );
    connect( &counter, SIGNAL( updateProcess( const unsigned int& ) ), controllerParent, SLOT( updateProgressBar( const unsigned int& ) ) );
}



void FlowVisualizationController::updateVolumetricMesh( Mesh* mesh )
{

    std::vector< NODE > nodes = region.getnodelist();
    std::vector< TETRAHEDRON > elements = region.getelementlist();

    std::vector< float > vertices = getVolumeVerticesfromNodeStructure( nodes );
    std::vector< unsigned int > edges = getVolumeEdgesfromTetrahedronStructure( elements );
    std::vector< unsigned int > faces = getVolumeCellsfromTetrahedronStructure( elements );

    mesh->setMeshType( Mesh::TYPE::TETRAHEDRAL );
    mesh->setVertices( vertices );
    mesh->setWireframe( edges );
    mesh->setFaces( faces );
    mesh->buildBoundingBox();



    volumetric_ok = true;

}


std::vector< float > FlowVisualizationController::getMeshVerticesfromFile( const tetgenio& obj )
{

    int number_of_points = obj.numberofpoints;
    double *points_list = obj.pointlist;

    std::vector< float > vertices;

    for( int i = 0; i < number_of_points; ++i )
    {
        vertices.push_back( (float)points_list[ 3*i ] );
        vertices.push_back( (float)points_list[ 3*i + 1 ] );
        vertices.push_back( (float)points_list[ 3*i + 2 ] );
    }

    return vertices;

}


 std::vector< unsigned int > FlowVisualizationController::getMeshWireFramefromFile( const tetgenio& obj )
{

    std::vector< unsigned int > wireframe;

    int number_of_faces = obj.numberoffacets;
    tetgenio::facet* faces_list = obj.facetlist;

    for( int i = 0; i < number_of_faces; ++i )
    {
        tetgenio::facet f = faces_list[ i ];


        int number_of_polygons = f.numberofpolygons;
        tetgenio::polygon *polygons_list = f.polygonlist;

        for( int j = 0; j < number_of_polygons; ++j )
        {
            tetgenio::polygon p = polygons_list[ j ];
            int number_of_vertices = p.numberofvertices;
            int *vertices_list = p.vertexlist;

            if( number_of_vertices != 4 ) continue;

            wireframe.push_back( vertices_list[ 0 ] );
            wireframe.push_back( vertices_list[ 1 ] );

            wireframe.push_back( vertices_list[ 1 ] );
            wireframe.push_back( vertices_list[ 2 ] );

            wireframe.push_back( vertices_list[ 2 ] );
            wireframe.push_back( vertices_list[ 3 ] );

            wireframe.push_back( vertices_list[ 3 ] );
            wireframe.push_back( vertices_list[ 0 ] );

        }

    }

    return wireframe;

}


std::vector< unsigned int > FlowVisualizationController::getMeshFacesfromFile( const tetgenio& obj )
{

    std::vector< unsigned int > faces;

    int number_of_faces = obj.numberoffacets;
    tetgenio::facet* faces_list = obj.facetlist;

    for( int i = 0; i < number_of_faces; ++i )
    {
        tetgenio::facet f = faces_list[ i ];

        int number_of_polygons = f.numberofpolygons;
        tetgenio::polygon *polygons_list = f.polygonlist;

        for( int j = 0; j < number_of_polygons; ++j )
        {
            tetgenio::polygon p = polygons_list[ j ];

            int number_of_vertices = p.numberofvertices;
            int *vertices_list = p.vertexlist;

            if( number_of_vertices != 4 ) continue;

            for( int l = 0; l < number_of_vertices; ++l )
                faces.push_back( vertices_list[ l ] );
        }

    }


    return faces;

}


std::vector< float > FlowVisualizationController::getVolumeVerticesfromNodeStructure( const std::vector< NODE >& nodes )
{

    int number_of_points = (int)nodes.size();

    std::vector< float > vertices;
    for( int i = 0; i < number_of_points; ++i )
    {
        vertices.push_back( nodes[ i ].x() );
        vertices.push_back( nodes[ i ].y() );
        vertices.push_back( nodes[ i ].z() );
    }

    return vertices;

}


 std::vector< unsigned int > FlowVisualizationController::getVolumeEdgesfromTetrahedronStructure( const std::vector< TETRAHEDRON >& elements )
{

     int number_of_cells = (int)elements.size();

     std::vector< unsigned int > wireframe;

     for( int i = 0; i < number_of_cells; ++i )
     {
         TETRAHEDRON tet = elements[ i ];

         wireframe.push_back( ( unsigned int ) tet.node( 0 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 1 ) );

         wireframe.push_back( ( unsigned int ) tet.node( 1 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 3 ) );

         wireframe.push_back( ( unsigned int ) tet.node( 3 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 0 ) );

         wireframe.push_back( ( unsigned int ) tet.node( 1 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 2 ) );

         wireframe.push_back( ( unsigned int ) tet.node( 2 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 3 ) );

         wireframe.push_back( ( unsigned int ) tet.node( 2 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 0 ) );

         wireframe.push_back( ( unsigned int ) tet.node( 3 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 2 ) );
     }


    return wireframe;

}


std::vector< unsigned int > FlowVisualizationController::getVolumeCellsfromTetrahedronStructure( const std::vector< TETRAHEDRON >& elements )
{

    int number_of_cells = (int)elements.size();

    std::vector< unsigned int > faces;

    for( int i = 0; i < number_of_cells; ++i )
    {
        TETRAHEDRON tet = elements[ i ];

        faces.push_back( ( unsigned int ) tet.node( 0 ) );
        faces.push_back( ( unsigned int ) tet.node( 1 ) );
        faces.push_back( ( unsigned int ) tet.node( 2 ) );
        faces.push_back( ( unsigned int ) tet.node( 3 ) );
    }


    return faces;

}



// -- Begin jd
using CurveType = RRM::PolygonalCurve2D<double>; //decltype(cross_section.edges_[0].segment.curve);
//    cross_section.edges_[0].segment.a;

struct Colour {
    double r;
    double g;
    double b;
};

struct SegmentedCurve {
    bool is_segmented = false;
    bool has_visible_segment = false;
    size_t nsegments = 0;
    size_t isegments = 0;
    std::vector<CurveType> edges;
    std::vector<Colour> colours;
    std::vector<int> segment_ids;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


void FlowVisualizationController::getSurfacesFromCrossSection(const CrossSection &_cross_section)
{

    CrossSection cross_section = _cross_section;
    std::cout << "Got into getSurfaceFromCrossSection\n";

    std::vector< double > positions;
    vector< int > nu, nv;

    int extrusion_size = 3;
    /* int number_points = 0; /// Number of point per sketch */
    int total_number_points = 0;

    std::vector<unsigned int> number_edges_; // Number of Sketch Segments
    std::vector< float > colors;

    // Number of Internal Surface
    //int number_of_surfaces = cross_section.curves_history_.size();
    int number_of_surfaces = 0;

    float diagonal = std::pow((cross_section.viewPort_.first.x() - cross_section.viewPort_.second.x()), 2) + std::pow((cross_section.viewPort_.first.y() - cross_section.viewPort_.second.y()), 2);
    diagonal = std::sqrt(diagonal);

    //    using CurveType = RRM::PolygonalCurve2D<Real>; //decltype(cross_section.edges_[0].segment.curve);
    ////    cross_section.edges_[0].segment.a;

    //    struct Colour {
    //        double r;
    //        double g;
    //        double b;
    //    };

    //    struct SegmentedCurve {
    //        bool is_segmented = false;
    //        bool has_visible_segment = false;
    //        size_t nsegments = 0;
    //        std::vector<CurveType> edges;
    //        std::vector<Colour> colours;
    //        std::vector<int> segment_ids;

    //    public:
    //        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    //    };

    /* vector<SegmentedCurve> curves; */
    /* curves.resize(cross_section.curves_history_.size()); */
    std::map<int, SegmentedCurve> curves;
    int index;
    bool visible;

    // Prepare an intermediate data structure to process the sketches
    //    // for (auto& history_iterator : cross_section.curves_history_)

    std::cout << "Building local data structure.\n";
    for (auto& e : cross_section.edges_)
    {
        if (e.second.is_boundary_ == false) {
            index = e.second.segment.curve_index;
            std::cout << "Current edge curve index: " << index << std::endl;

            // smooth it out...
            e.second.segment.curve.chaikinFilter(3);

            visible = e.second.is_visible_;
            std::cout << "     Edge is visible: " << visible << std::endl;
            curves[index].is_segmented |= (visible == false);
            std::cout << "     Curve (" << index << ") is segmented: " << curves[index].is_segmented << std::endl;
            curves[index].has_visible_segment |= visible;
            std::cout << "     Curve (" << index << ") has at least one visible segment so far: " << curves[index].has_visible_segment << std::endl;


            if (visible) {
                curves[index].segment_ids.push_back(e.second.id_);
                std::cout << "     Processing edge (" << e.second.id_ << ") in curve: " << index << std::endl;
                curves[index].edges.push_back(e.second.segment.curve);
                curves[index].colours.push_back(Colour{ e.second.r, e.second.g, e.second.b });
                total_number_points += e.second.segment.curve.size() * extrusion_size;
                curves[index].nsegments++;
            }
            else {
                curves[index].isegments++;
                std::cout << "     Ignoring current edge.\n";
            }

            std::cout << "     So far " << curves[index].nsegments << " segments were visible in curve " << index << std::endl;
            std::cout << "     So far " << curves[index].isegments << " segments were invisible in curve " << index << std::endl;

        }
    }

    // assemble the list of surfaces generators to be sent to Zhao's implementation
    std::vector<CurveType> processed_curves;
    CurveType current_curve;
    std::vector<Colour> processed_curves_colours;

    double tolerance = 1E-01;
    total_number_points = 0;

    for (auto &c : curves)
    {
        std::cout << "Got a raw_curve with index:  " << c.first << std::endl << std::flush;

        if (c.second.has_visible_segment == false)
        {
            std::cout << "Ignoring invisible curve.\n";
            continue;
        }

        // if the curve is entire, put all segments together.
        if (c.second.isegments == 2)
        {
            std::cout << "Got a curve that I hope is entire, and has " << c.second.nsegments << " segments. \n";
            current_curve.clear();
            for (int s = 0; s < c.second.nsegments; ++s)
            {
                current_curve.join(c.second.edges[s], tolerance);
            }

            nu.push_back(current_curve.size());
            std::cout << "     Nu size:" << current_curve.size() << std::endl << std::flush;
            nv.push_back(extrusion_size);
            std::cout << "     Nv size:" << extrusion_size << std::endl << std::flush;

            total_number_points += current_curve.size() * extrusion_size;
            processed_curves.push_back(current_curve);
            processed_curves_colours.push_back(c.second.colours[0]);
        }
        else // if ( c.second.isegments != 2 )
        {
            std::cout << "Got a curve is not entire, going to process " << c.second.nsegments << " segments. \n";
            for (int s = 0; s < c.second.nsegments; ++s)
            {
                nu.push_back(c.second.edges[s].size());
                std::cout << "     Nu size:" << c.second.edges[s].size() << std::endl << std::flush;
                nv.push_back(extrusion_size);
                std::cout << "     Nv size:" << extrusion_size << std::endl << std::flush;

                total_number_points += c.second.edges[s].size() * extrusion_size;
                processed_curves.push_back(c.second.edges[s]);
                processed_curves_colours.push_back(c.second.colours[s]);
            }

        }
    }

    number_of_surfaces = processed_curves.size();
    std::cout << "Num curves == num colours? " << (processed_curves.size() == processed_curves_colours.size()) << std::endl << std::flush;



    /* number_points = 0; */

    float extrusion_step = 0; // (diagonal * 0.5);
    index = 0;
    int offset = 0;

    std::cout << "Preparing points and colors vectors with 3 * " << total_number_points << " entries.\n" << std::flush;
    positions.resize(3 * total_number_points);
    colors.resize(3 * total_number_points);

    for (int k = 0; k < processed_curves.size(); ++k)
    {
        auto &c = processed_curves[k];
        extrusion_step = 0;
        for (int j = 0; j < extrusion_size; j++)

        {
            for (int i = 0; i < c.size(); ++i)
            {
                index = j*c.size() + i + offset;

                positions[3 * index + 0] = c[i].x(); //(cross_section.edges_[edge_id].segment.curve[it].x());
                positions[3 * index + 1] = (j + extrusion_step);
                positions[3 * index + 2] = c[i].y(); //(cross_section.edges_[edge_id].segment.curve[it].y());

                colors[3 * index + 0] = processed_curves_colours[k].r; //(cross_section.edges_[edge_id].r);
                colors[3 * index + 1] = processed_curves_colours[k].g; //(cross_section.edges_[edge_id].g);
                colors[3 * index + 2] = processed_curves_colours[k].b; //(cross_section.edges_[edge_id].b);
                std::cout << "Element (" << index << "): "
                    << "x = " << c[i].x()
                    << ", y = " << j + extrusion_step
                    << ", z = " << c[i].y()
                    << std::endl << std::flush;
            }
            extrusion_step += (diagonal * 0.5);
        }
        offset += c.size() * extrusion_size;
    }


    std::cout << "------ Post processing...---------\n" << std::flush;
    std::vector< unsigned int > faces;
std:cout << "Setting skeleton: " << std::flush;
    this->region.setSkeleton(number_of_surfaces, nu, nv, positions);

    std::cout << "done. \n" << std::flush;

    std::cout << "Getting skeleton: " << std::flush;
    faces = this->getSkeletonFaces(nu, nv, positions);
    std::cout << "done.\n" << std::flush;

    if (faces.empty() == true) return;

    this->mesh_ok = true;
    emit this->updateMesh(Mesh::TYPE::QUADRILATERAL, positions, faces);
    emit this->updateColors(colors);
}


std::vector< unsigned int > FlowVisualizationController::getSkeletonFaces( const std::vector< int >& nu_list, const std::vector< int >&nv_list, const std::vector< double >& positions )
{


    std::vector< unsigned int > faces;

    unsigned int number_lines_ = nu_list.size();
    int offset = 0;

    for( unsigned int k = 0; k < number_lines_; ++k  )
    {

        int nu_number = nu_list[ k ];
        int nv_number = nv_list[ k ];


        for(  unsigned int j = 0; j < nv_number - 1; ++j )
        {
            for( unsigned int i = 0; i < nu_number - 1 ; ++i )
            {

                unsigned int id0 = j*nu_number + i + offset;
                unsigned int id1 = j*nu_number + ( i + 1 ) + offset;
                unsigned int id2 = ( j + 1 )*nu_number + ( i + 1 ) + offset;
                unsigned int id3 = ( j + 1 )*nu_number + i  + offset;

                faces.push_back( id0 );
                faces.push_back( id1 );
                faces.push_back( id2 );
                faces.push_back( id3 );


            }
        }
        offset += nu_number*nv_number;

    }

    return faces;
}


void FlowVisualizationController::computeFlowProperties()
{

    if( volumetric_ok == false || mesh_ok == false || properties_computed == true ) return;


    emit clearPropertiesMenu();


    counter.start( 0, 3 );
    region.steadystateflowsolver();
    counter.update( 1 );
    region.flowdiagnostics();
    counter.update( 2 );

    emit propertybyVertexComputed( "Corrected Pressure", "SCALAR" ) ;
    emit propertybyVertexComputed( "TOF", "SCALAR" ) ;
    emit propertybyVertexComputed( "Tracer", "SCALAR" ) ;

    emit propertybyFaceComputed( "Permeability", "SCALAR" ) ;
    emit propertybyFaceComputed( "Velocity", "VECTOR" ) ;

    emit setaColorMap();


    counter.update( 3 );

    properties_computed = true;



    //    emit propertybyVertexComputed( "VELOCITY", "VECTOR" ) ;
    //    emit propertybyFaceComputed( "TOF", "SCALAR" ) ;
    //    emit propertybyFaceComputed( "TRACER", "SCALAR" ) ;


}


void FlowVisualizationController::showRegions()
{


    std::vector< double > positions;
    std::vector< int > regions;
    std::set< int > idregions;
    region.getRegionsPositions( positions, regions, idregions );


    std::vector< double > values = region.porevolume();

    std::random_device rd;
    std::mt19937 seed(rd());
    std::uniform_int_distribution<unsigned int> distr(0, 255);


    int number_of_colors = idregions.size();

    std::map< int, QColor > mapidtocolors;
    std::vector< QColor > colors;
    std::set<int>::iterator it;

    for (it=idregions.begin(); it!=idregions.end(); ++it)
    {
        int r = distr( seed );
        int g = distr( seed );
        int b = distr( seed );

        colors.push_back( QColor( r, g, b ) );
        mapidtocolors[ *it ] = QColor( r, g, b );
    }



    std::vector< QColor > cellscolors;
    for( int i = 0; i < regions.size(); ++i )
    {
        cellscolors.push_back( mapidtocolors[ regions[i] ] );
    }


    emit showPointMarkers( cellscolors, positions );
    emit showPoreVolumeResults( colors, values );

}


std::vector< double > FlowVisualizationController::getVerticesPropertyValues( std::string name_of_property, std::string method, double& min, double& max )
{

    std::string type = "SCALAR";
    std::vector< double > values;

    if( name_of_property.compare( "Velocity" ) == 0 )
    {
        type = "VECTOR";
        region.getNodesVelocity( values );
    }
    else if ( name_of_property.compare( "Corrected Pressure" ) == 0 )
    {
        region.getPressureValues( values );
    }

    else if( name_of_property.compare( "TOF" ) == 0 )
    {
        region.getNodesTOF( values );
    }
    else if( name_of_property.compare( "Tracer" ) == 0 )
    {
        region.getNodesTracer( values );
    }


    std::vector< double > scalar_values;


    if ( values.empty() == true ) return scalar_values;

    if( type.compare( "VECTOR" ) == 0 )
    {
        scalar_values = vectorToScalarProperties( values, method, min, max );
    }
    else
    {
        scalar_values = values;

        std::vector< double >::iterator itmin = std::min_element( scalar_values.begin(), scalar_values.end() );
        std::vector< double >::iterator itmax = std::max_element( scalar_values.begin(), scalar_values.end() );

        int idmin = std::distance( scalar_values.begin(), itmin );
        int idmax = std::distance( scalar_values.begin(), itmax );

        min = scalar_values[ idmin ];
        max = scalar_values[ idmax ];
    }


    return scalar_values;
}



std::vector< double > FlowVisualizationController::getFacesPropertyValues( std::string name_of_property, std::string method, double& min, double& max )
{

    std::string type = "SCALAR";
    std::vector< double > values;

    if( name_of_property.compare( "Velocity" ) == 0 )
    {
        type = "VECTOR";
        region.getElementalsVelocity( values );
    }
    else if ( name_of_property.compare( "Permeability" ) == 0 )
    {
        region.getPermeabilityValues( values );
    }


    else if( name_of_property.compare( "TOF" ) == 0 )
    {
        region.getElementalsTOF( values );
    }

    else if( name_of_property.compare( "Tracer" ) == 0 ){
        region.getElementalsTracer( values );
    }


    std::vector< double > scalar_values;


    if ( values.empty() == true ) return scalar_values;

    if( type.compare( "VECTOR" ) == 0 )
    {
        scalar_values = vectorToScalarProperties( values, method, min, max );
    }
    else
    {
        scalar_values = values;

        std::vector< double >::iterator itmin = std::min_element( scalar_values.begin(), scalar_values.end() );
        std::vector< double >::iterator itmax = std::max_element( scalar_values.begin(), scalar_values.end() );

        int idmin = std::distance( scalar_values.begin(), itmin );
        int idmax = std::distance( scalar_values.begin(), itmax );

        min = scalar_values[ idmin ];
        max = scalar_values[ idmax ];
    }


    return scalar_values;
}


std::vector< double > FlowVisualizationController::vectorToScalarProperties(const std::vector<double> &values, std::string type, double& min , double& max  )
{

    int number_of_vertices = (int)values.size()/3;
    std::vector< double > scalar_values;


    if( type.compare( "COORDX" ) == 0 )
    {

        for( int i = 0; i < number_of_vertices; ++i )
        {
            scalar_values.push_back( values[ 3*i ] );
        }

    }
    else if( type.compare( "COORDY" ) == 0 )
    {
        for( int i = 0; i < number_of_vertices; ++i )
        {
            scalar_values.push_back( values[ 3*i + 1 ] );
        }

    }
    else if( type.compare( "COORDZ" ) == 0 )
    {

        for( int i = 0; i < number_of_vertices; ++i )
        {
            scalar_values.push_back( values[ 3*i + 2 ] );
        }
    }
    else if( type.compare( "LENGTH" ) == 0 )
    {

        for( int i = 0; i < number_of_vertices; ++i )
        {
            double x = values[ 3*i ];
            double y = values[ 3*i + 1 ];
            double z = values[ 3*i + 2 ];

            double length = x*x + y*y + z*z;

            scalar_values.push_back( length );

        }
    }

    std::vector< double >::iterator itmin = std::min_element( scalar_values.begin(), scalar_values.end() );
    std::vector< double >::iterator itmax = std::max_element( scalar_values.begin(), scalar_values.end() );

    int idmin  = std::distance( scalar_values.begin(), itmin );
    int idmax = std::distance( scalar_values.begin(), itmax );

    min = scalar_values[ idmin ];
    max = scalar_values[ idmax ];

    return scalar_values;

}


void FlowVisualizationController::setTetgenCommand( std::string& cmd )
{
    region.setTetgenCommand( cmd.c_str() );
}


void FlowVisualizationController::setTriangleCommand( std::string& cmd )
{
    region.setTriangleCommand( cmd.c_str() );
}


void FlowVisualizationController::setPropertyArea( const int& np, const std::vector< double >& values )
{
    if( values.empty() == true )
        region.setPropertyArea( 0, values );
    else
        region.setPropertyArea( np, values );

}


void FlowVisualizationController::setBoundariesValues( int nb, vector< double > &values )
{

    if( values.empty() == true )
        region.setBoundariesSurface( 0, values );
    else
        region.setBoundariesSurface( nb, values );

//
}

void FlowVisualizationController::setWellsValues( int nw,  vector< double > &values )
{

    if( values.empty() == true )
        region.setWells( 0, values );
    else
        region.setWells( nw, values );


}


void FlowVisualizationController::setTofBoundaryValues( int n, vector< double > &values )
{

    vector< int > values_;

    if( values.empty() == true )
    {
        region.setTofBoundaries( 0, 1, values_ );
        return;
    }


    int ntfsignal = values[ 0 ];


    for( std::size_t it = 0; it < n; ++it )
    {
        values_.push_back( (int)values.at( it + 1 ) );
    }

    region.setTofBoundaries( n, ntfsignal, values_ );
}


void FlowVisualizationController::setTrBoundaryValues(int n, std::vector< double > &values )
{
    vector< int > values_;

    if( values.empty() == true )
    {
        region.setTracerBoundaries( 0, 1, values_ );
        return;
    }


    int ntrsignal = values[ 0 ];



    for( std::size_t it = 0; it < n; ++it )
    {
        values_.push_back( (int)values.at( /*2**/it + 1) );
    }

    region.setTracerBoundaries( n, ntrsignal, values_ );

}



void FlowVisualizationController::exportSurfacetoVTK( const std::string& filename )
{
    region.writesurfacemeshVTK( (char *)filename.c_str() );
}


void FlowVisualizationController::exportVolumetoVTK( const std::string& filename )
{
    region.writevolumemesh( (char *)filename.c_str() );
}


void FlowVisualizationController::exportCornerPointtoVTK( const std::string& filename )
{
    region.writecornerpointgridVTK( (char *)filename.c_str() );
}


void FlowVisualizationController::exportCornerPointtoGRDECL( const std::string& filename )
{
    region.writecornerpointgrideclipse( (char *)filename.c_str() );
}


void FlowVisualizationController::exportResultstoVTK( const std::string& filename )
{
    region.writeresult( (char*)filename.c_str() );
}


void FlowVisualizationController::increaseNumberofEdges()
{
    region.increaseNumberofEdges();
}


void FlowVisualizationController::decreaseNumberofEdges()
{
    region.decreaseNumberofEdges();
}


void FlowVisualizationController::increaseEdgeLength()
{
    region.increaseEdgeLength();
}


void FlowVisualizationController::decreaseEdgeLength()
{
    region.decreaseEdgeLength();
}


void FlowVisualizationController::clear()
{
    mesh_ok = false;
    user_input_ok = false;
    volumetric_ok = false;
    properties_computed = false;
    region.clearregion();
//    region.resetStandardValues();
}

