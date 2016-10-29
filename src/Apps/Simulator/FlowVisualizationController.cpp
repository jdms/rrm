
#include "FlowVisualizationController.h"


FlowVisualizationController::FlowVisualizationController( QWidget *parent )
{
    mesh_ok = false;
    volumetric_ok = false;
    properties_computed = false;
    user_input_ok = false;

    current_method = MESHING_METHOD::UNSTRUCTURED;

    code_interface.loadDefaultValues();

}




void FlowVisualizationController::readSkeletonFiles( const std::string& filename )
{

    if( mesh_ok == true ) return;


    code_interface.readSkeletonFile( filename );
    // chmar funcao para desenhar a superficie


}


void FlowVisualizationController::buildSurfaceSkeleton( std::vector< double >& positions, std::vector< unsigned int >& faces )
{

    unsigned int surfaces_number = 0;
    std::vector< unsigned int > nu_list;
    std::vector< unsigned int > nv_list;

    code_interface.getSurfaceSkeleton( surfaces_number, nu_list, nv_list, positions );

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

    mesh_ok = true;

}




void FlowVisualizationController::readInputParameters( const std::string& input_file )
{
    code_interface.readUserInput( input_file );
    user_input_ok = true;
}




void FlowVisualizationController::generateCornerPoint()
{

    if( mesh_ok == false || user_input_ok == false ) return;

    code_interface.buildCPGVolumetricMesh();
    volumetric_ok = true;

}


void FlowVisualizationController::generateUnstructured()
{

    if( mesh_ok == false || user_input_ok == false ) return;


    code_interface.buildVolumetricMesh();
    volumetric_ok = true;


}




void FlowVisualizationController::updateSurfaces( std::vector< float >& vertices, std::vector< unsigned int >& faces )
{

   code_interface.getSurfaceVertices( vertices );
   code_interface.getSurfaceFaces( faces );

}


void FlowVisualizationController::updateCornerPoint( std::vector< float >& vertices, std::vector< unsigned int >& edges, std::vector< unsigned int >& faces )
{

     code_interface.getCPGVolumeVertices( vertices );
     code_interface.getCPGVolumeEdges( edges );
     code_interface.getCPGVolumeCells( faces );
     volumetric_ok = true;
}


void FlowVisualizationController::updateVolumetricMesh( std::vector< float >& vertices, std::vector< unsigned int >& edges, std::vector< unsigned int >& faces )
{


    code_interface.getVolumeVertices( vertices );
    code_interface.getVolumeEdges( edges );
    code_interface.getVolumeCells( faces );
    volumetric_ok = true;

}



//// -- Begin jd
//using CurveType = RRM::PolygonalCurve2D<double>; //decltype(cross_section.edges_[0].segment.curve);
////    cross_section.edges_[0].segment.a;

//struct Colour {
//    double r;
//    double g;
//    double b;
//};

//struct SegmentedCurve {
//    bool is_segmented = false;
//    bool has_visible_segment = false;
//    size_t nsegments = 0;
//    size_t isegments = 0;
//    std::vector<CurveType> edges;
//    std::vector<Colour> colours;
//    std::vector<int> segment_ids;

//public:
//    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
//};


void FlowVisualizationController::getSurfacesFromCrossSection(/*const CrossSection &_cross_section*/)
{

//    CrossSection cross_section = _cross_section;
//    std::cout << "Got into getSurfaceFromCrossSection\n";

//    std::vector< double > positions;
//    vector< unsigned int > nu, nv;

//    int extrusion_size = 3;
//    /* int number_points = 0; /// Number of point per sketch */
//    int total_number_points = 0;

//    std::vector<unsigned int> number_edges_; // Number of Sketch Segments
//    std::vector< float > colors;

//    // Number of Internal Surface
//    //int number_of_surfaces = cross_section.curves_history_.size();
//    int number_of_surfaces = 0;

//    float diagonal = std::pow((cross_section.viewPort_.first.x() - cross_section.viewPort_.second.x()), 2) + std::pow((cross_section.viewPort_.first.y() - cross_section.viewPort_.second.y()), 2);
//    diagonal = std::sqrt(diagonal);

//    //    using CurveType = RRM::PolygonalCurve2D<Real>; //decltype(cross_section.edges_[0].segment.curve);
//    ////    cross_section.edges_[0].segment.a;

//    //    struct Colour {
//    //        double r;
//    //        double g;
//    //        double b;
//    //    };

//    //    struct SegmentedCurve {
//    //        bool is_segmented = false;
//    //        bool has_visible_segment = false;
//    //        size_t nsegments = 0;
//    //        std::vector<CurveType> edges;
//    //        std::vector<Colour> colours;
//    //        std::vector<int> segment_ids;

//    //    public:
//    //        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
//    //    };

//    /* vector<SegmentedCurve> curves; */
//    /* curves.resize(cross_section.curves_history_.size()); */
//    std::map<int, SegmentedCurve> curves;
//    int index;
//    bool visible;

//    // Prepare an intermediate data structure to process the sketches
//    //    // for (auto& history_iterator : cross_section.curves_history_)

//    std::cout << "Building local data structure.\n";
//    for (auto& e : cross_section.edges_)
//    {
//        if (e.second.is_boundary_ == false) {
//            index = e.second.segment.curve_index;
//            std::cout << "Current edge curve index: " << index << std::endl;

//            // smooth it out...
//            e.second.segment.curve.chaikinFilter(3);

//            visible = e.second.is_visible_;
//            std::cout << "     Edge is visible: " << visible << std::endl;
//            curves[index].is_segmented |= (visible == false);
//            std::cout << "     Curve (" << index << ") is segmented: " << curves[index].is_segmented << std::endl;
//            curves[index].has_visible_segment |= visible;
//            std::cout << "     Curve (" << index << ") has at least one visible segment so far: " << curves[index].has_visible_segment << std::endl;


//            if (visible) {
//                curves[index].segment_ids.push_back(e.second.id_);
//                std::cout << "     Processing edge (" << e.second.id_ << ") in curve: " << index << std::endl;
//                curves[index].edges.push_back(e.second.segment.curve);
//                curves[index].colours.push_back(Colour{ e.second.r, e.second.g, e.second.b });
//                total_number_points += e.second.segment.curve.size() * extrusion_size;
//                curves[index].nsegments++;
//            }
//            else {
//                curves[index].isegments++;
//                std::cout << "     Ignoring current edge.\n";
//            }

//            std::cout << "     So far " << curves[index].nsegments << " segments were visible in curve " << index << std::endl;
//            std::cout << "     So far " << curves[index].isegments << " segments were invisible in curve " << index << std::endl;

//        }
//    }

//    // assemble the list of surfaces generators to be sent to Zhao's implementation
//    std::vector<CurveType> processed_curves;
//    CurveType current_curve;
//    std::vector<Colour> processed_curves_colours;

//    double tolerance = 1E-01;
//    total_number_points = 0;

//    for (auto &c : curves)
//    {
//        std::cout << "Got a raw_curve with index:  " << c.first << std::endl << std::flush;

//        if (c.second.has_visible_segment == false)
//        {
//            std::cout << "Ignoring invisible curve.\n";
//            continue;
//        }

//        // if the curve is entire, put all segments together.
//        if (c.second.isegments == 2)
//        {
//            std::cout << "Got a curve that I hope is entire, and has " << c.second.nsegments << " segments. \n";
//            current_curve.clear();
//            for (int s = 0; s < c.second.nsegments; ++s)
//            {
//                current_curve.join(c.second.edges[s], tolerance);
//            }

//            nu.push_back(current_curve.size());
//            std::cout << "     Nu size:" << current_curve.size() << std::endl << std::flush;
//            nv.push_back(extrusion_size);
//            std::cout << "     Nv size:" << extrusion_size << std::endl << std::flush;

//            total_number_points += current_curve.size() * extrusion_size;
//            processed_curves.push_back(current_curve);
//            processed_curves_colours.push_back(c.second.colours[0]);
//        }
//        else // if ( c.second.isegments != 2 )
//        {
//            std::cout << "Got a curve is not entire, going to process " << c.second.nsegments << " segments. \n";
//            for (int s = 0; s < c.second.nsegments; ++s)
//            {
//                nu.push_back(c.second.edges[s].size());
//                std::cout << "     Nu size:" << c.second.edges[s].size() << std::endl << std::flush;
//                nv.push_back(extrusion_size);
//                std::cout << "     Nv size:" << extrusion_size << std::endl << std::flush;

//                total_number_points += c.second.edges[s].size() * extrusion_size;
//                processed_curves.push_back(c.second.edges[s]);
//                processed_curves_colours.push_back(c.second.colours[s]);
//            }

//        }
//    }

//    number_of_surfaces = processed_curves.size();
//    std::cout << "Num curves == num colours? " << (processed_curves.size() == processed_curves_colours.size()) << std::endl << std::flush;



//    /* number_points = 0; */

//    float extrusion_step = 0; // (diagonal * 0.5);
//    index = 0;
//    int offset = 0;

//    std::cout << "Preparing points and colors vectors with 3 * " << total_number_points << " entries.\n" << std::flush;
//    positions.resize(3 * total_number_points);
//    colors.resize(3 * total_number_points);

//    for (int k = 0; k < processed_curves.size(); ++k)
//    {
//        auto &c = processed_curves[k];
//        extrusion_step = 0;
//        for (int j = 0; j < extrusion_size; j++)

//        {
//            for (int i = 0; i < c.size(); ++i)
//            {
//                index = j*c.size() + i + offset;

//                positions[3 * index + 0] = c[i].x(); //(cross_section.edges_[edge_id].segment.curve[it].x());
//                positions[3 * index + 1] = (j + extrusion_step);
//                positions[3 * index + 2] = c[i].y(); //(cross_section.edges_[edge_id].segment.curve[it].y());

//                colors[3 * index + 0] = processed_curves_colours[k].r; //(cross_section.edges_[edge_id].r);
//                colors[3 * index + 1] = processed_curves_colours[k].g; //(cross_section.edges_[edge_id].g);
//                colors[3 * index + 2] = processed_curves_colours[k].b; //(cross_section.edges_[edge_id].b);
//                std::cout << "Element (" << index << "): "
//                          << "x = " << c[i].x()
//                          << ", y = " << j + extrusion_step
//                          << ", z = " << c[i].y()
//                          << std::endl << std::flush;
//            }
//            extrusion_step += (diagonal * 0.5);
//        }
//        offset += c.size() * extrusion_size;
//    }


//    std::cout << "------ Post processing...---------\n" << std::flush;
//    std::vector< unsigned int > faces;
//std:cout << "Setting skeleton: " << std::flush;

//    //    this->//region.setSkeleton(number_of_surfaces, nu, nv, positions);

//    code_interface.setSkeletonData( number_of_surfaces, nu, nv, positions );


//    std::cout << "done. \n" << std::flush;

//    // chamar a funcao pra desenhar


}

// -- End jd




void FlowVisualizationController::computeFlowProperties()
{

    if( volumetric_ok == false || mesh_ok == false || properties_computed == true ) return;


    code_interface.init();
    code_interface.flowdiagnostics();

    emit clearPropertiesMenu();


    if( current_method == MESHING_METHOD::UNSTRUCTURED ) // Unstructure Volumetric Mesh
    {
        emit propertybyVertexComputed( "Corrected Pressure", "SCALAR" ) ;
        emit propertybyVertexComputed( "Backward TOF", "SCALAR" ) ;
        emit propertybyVertexComputed( "Forward TOF", "SCALAR" ) ;
        emit propertybyVertexComputed( "Total TOF", "SCALAR" ) ;
        emit propertybyVertexComputed( "Max Backward Tracer", "SCALAR" ) ;
        emit propertybyVertexComputed( "Max Forward Tracer", "SCALAR" ) ;

        emit propertybyFaceComputed( "Velocity", "VECTOR" );
        emit propertybyFaceComputed( "Permeability", "SCALAR" );

    }

    else if( current_method == MESHING_METHOD::CORNERPOINT ) // Corner Point Mesh
    {
        emit propertybyFaceComputed( "Corrected Pressure", "SCALAR" ) ;
        emit propertybyFaceComputed( "Backward TOF", "SCALAR" ) ;
        emit propertybyFaceComputed( "Total TOF", "SCALAR" ) ;

        emit propertybyFaceComputed( "Max Backward Tracer", "SCALAR" ) ;
        emit propertybyFaceComputed( "Max Forward Tracer", "SCALAR" ) ;
    }



    emit setColorMap();



    properties_computed = true;




}


void FlowVisualizationController::getRegionsColor( std::vector< QColor >& color_by_cells )
{


    std::vector< int > idregion_by_cell;
    code_interface.getRegionId( idregion_by_cell );


    std::vector< int > regions_id = idregion_by_cell;
    auto last = std::unique( regions_id.begin(), regions_id.end() );
    regions_id.erase( last, regions_id.end() );


    std::random_device rd;
    std::mt19937 seed(rd());
    std::uniform_int_distribution< unsigned int > distr( 0, 255 );


    std::map< int, QColor > color_by_region;
    for( int i = 0; i < regions_id.size(); ++i )
    {
        int id = regions_id[ i ];

        int r = distr( seed );
        int g = distr( seed );
        int b = distr( seed );

        color_by_region[ id ] = QColor( r, g, b );
    }

    for( int i = 0; i < idregion_by_cell.size(); ++i )
    {
        int id = idregion_by_cell[ i ];
        color_by_cells.push_back( color_by_region[ id ] );
    }

}





std::vector< double > FlowVisualizationController::getVerticesPropertyValues( std::string name_of_property, std::string method, double& min, double& max )
{

    std::string type = "SCALAR";
    std::vector< double > values;


    if ( name_of_property.compare( "Corrected Pressure" ) == 0 )
    {
        code_interface.getPressure( values );
    }
    else if( name_of_property.compare( "Backward TOF" ) == 0 )
    {
        code_interface.getBackwardTOF( values );
    }
    else if( name_of_property.compare( "Forward TOF" ) == 0 )
    {
        code_interface.getForwardTOF( values );
    }
    else if( name_of_property.compare( "Total TOF" ) == 0 )
    {
        code_interface.getTotalTOF( values );
    }
    else if( name_of_property.compare( "Max Backward Tracer" ) == 0 )
    {
        code_interface.getMaxBackwardTracer( values );
    }
    else if( name_of_property.compare( "Max Forward Tracer" ) == 0 )
    {
        code_interface.getMaxForwardTracer( values );
    }




    if ( values.empty() == false )
    {

        std::vector< double >::iterator itmin = std::min_element( values.begin(), values.end() );
        std::vector< double >::iterator itmax = std::max_element( values.begin(), values.end() );

        int idmin = std::distance( values.begin(), itmin );
        int idmax = std::distance( values.begin(), itmax );

        min = values[ idmin ];
        max = values[ idmax ];

    }

    return values;

}


std::vector< double > FlowVisualizationController::getFacesPropertyValues( std::string name_of_property, std::string method, double& min, double& max )
{

    std::string type = "SCALAR";
    std::vector< double > values;

    if( current_method == MESHING_METHOD::UNSTRUCTURED ) // Unstructure Volumetric Mesh
    {
        if ( name_of_property.compare( "Velocity" ) == 0 )
        {
            type = "VECTOR";
            code_interface.getVelocitybyCells( values );
        }
         else if( name_of_property.compare( "Permeability" ) == 0 )
        {
            code_interface.getPermeability( values );
        }

    }

    else if( current_method == MESHING_METHOD::CORNERPOINT ) // Corner Point Mesh  // by cell
    {
        if ( name_of_property.compare( "Corrected Pressure" ) == 0 )
        {
            code_interface.getCPGPressure( values );
        }
        else if( name_of_property.compare( "Backward TOF" ) == 0 )
        {
            code_interface.getCPGBackwardTOF( values );
        }
        else if( name_of_property.compare( "Total TOF" ) == 0 )
        {
            code_interface.getCPGTotalTOF( values );
        }
        else if( name_of_property.compare( "TOF" ) == 0 )
        {
            code_interface.getCPGTOF( values );
        }
        else if( name_of_property.compare( "Backward Tracer" ) == 0 )
        {
            code_interface.geBackwardTracer( values );
        }
        else if( name_of_property.compare( "Tracer" ) == 0 )
        {
            code_interface.getTracer( values );
        }

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






void FlowVisualizationController::setPropertyArea( const int np, const std::vector< double >& values , const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc )
{

    for( int i = 0; i < np; ++i )
    {
        code_interface.setRegion( i, values[ 3*i ], values[ 3*i + 1 ], values[ 3*i +2 ], perm[ i ], poros[ i ], visc[ i ] );
    }

}


void FlowVisualizationController::getPropertyArea( int& np, std::vector< double >& values , std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc )
{
    np = code_interface.getNumberofRegions();

    values.resize( 3*np );
    perm.resize( np );
    poros.resize( np );
    visc.resize( np );

    for( int i = 0; i < np; ++i )
    {
        code_interface.getRegion( i, values[ 3*i ], values[ 3*i + 1 ], values[ 3*i +2 ], perm[ i ], poros[ i ], visc[ i ] );
    }

}



void FlowVisualizationController::setWellsValues( const int nw,  const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign )
{
    for( int i = 0; i < nw; ++i )
    {
        code_interface.setWell( i, type[ i ], value[ i ], sign[ i ] );
    }

}


void FlowVisualizationController::getWellsValues( int& nw,  std::vector< unsigned int >& type, std::vector< double >& value, std::vector< int >& sign )
{

    nw = code_interface.getNumberofWells();
    type.resize( nw );
    value.resize( nw );
    sign.resize( nw );

    for( int i = 0; i < nw; ++i )
    {
        code_interface.getWell( i, type[ i ], value[ i ], sign[ i ] );
    }

}





void FlowVisualizationController::exportSurfacetoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;

    code_interface.exportSurfacetoVTK( filename.toStdString() );
}


void FlowVisualizationController::exportVolumetoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;


     code_interface.exportSurfacetoVTK( filename.toStdString() );
}


void FlowVisualizationController::exportCornerPointtoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;


     code_interface.exportCornerPointtoGRDECL( filename.toStdString() );
}


void FlowVisualizationController::exportCornerPointtoGRDECL()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;

     code_interface.exportCornerPointtoGRDECL( filename.toStdString() );
}


void FlowVisualizationController::exportResultstoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;


     code_interface.exportResultstoVTK( filename.toStdString() );
}




void FlowVisualizationController::clear()
{
    mesh_ok = false;
    user_input_ok = false;
    volumetric_ok = false;
    properties_computed = false;
    code_interface.clear();
}

