#include "Controller.hpp"

#include "model_new/Boundary.hpp"

#include "Core/Geometry/PolygonalCurve2D.hpp"



Controller::Controller()
{

    current_reconstruction = ReconstructionMode::EXTRUSION;

    current_crosssection = 0.0f;
    current_stratigraphy = 0;


}




void Controller::addCrossSection( const float& d )
{

    CrossSection* cross_section = new CrossSection();
    cross_section->setDepth( d );

    crosssections_list[ d ] = cross_section;
    current_crosssection = d;

}



bool Controller::addBoundary( const float& w, const float& h, const float& d )
{

    if( crosssections_list.empty() == true ) return false;


    CrossSection* cross_section = crosssections_list[ current_crosssection ];
    Boundary* boundary_data;


    if( cross_section->hasBoundary() == false )
    {
        boundary_data = new Boundary();
    }
    else
    {
        boundary_data = cross_section->getBoundary();
    }


    boundary_data->edit( 0, 0, 0, w, h, d );
    cross_section->setBoundary( boundary_data );


    return true;

}


void Controller::editBoundary( const float &x, const float &y, const float &w, const float &h, const float &d )
{

    CrossSection* cross_section = crosssections_list[ current_crosssection ];
    Boundary* boundary_data = cross_section->getBoundary();


    boundary_data->edit( x, y, current_crosssection, w, h, d );

    update();

}


Boundary* Controller::getCurrentBoundary()
{

    CrossSection* cross_section = crosssections_list[ current_crosssection ];
    return cross_section->getBoundary();

}




void Controller::initRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth )
{

    rules_processor.init();
    rules_processor.setOrigin( orig_x, orig_y, orig_z );
    rules_processor.setLenght( width, height, depth );
    rules_processor.update( RRM::ExtrusionRulesProcessor::State::SKETCHING );


}


void Controller::editRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth )
{

    rules_processor.setOrigin( orig_x, orig_y, orig_z );
    rules_processor.setLenght( width, height, depth );

}




bool Controller::addCurve( const Curve2D& curve )
{

    if( stratigraphics_list.empty() == true )
        addStratigraphy();

    Stratigraphy* strat = stratigraphics_list[ current_stratigraphy ];
    strat->addCurve( current_crosssection, curve );


    CrossSection* cross_section = crosssections_list[ current_crosssection ];
    cross_section->addStratigraphy( strat );


    return true;


}


bool Controller::addStratigraphy()
{

    Stratigraphy* strat = new Stratigraphy();
    current_stratigraphy = (int) stratigraphics_list.size();

    strat->setId( current_stratigraphy );
    stratigraphics_list.push_back( strat );

    return true;


}


Stratigraphy* Controller::getCurrentStratigraphy()
{

    if( stratigraphics_list.empty() == true ) return 0;

    return stratigraphics_list[ current_stratigraphy ];

}


bool Controller::interpolateStratigraphy()
{

    std::cout << "-- Interpolate Stratigraphy" << std::endl;


    if( stratigraphics_list.empty() == true ) return false;


    Stratigraphy* strat = stratigraphics_list[ current_stratigraphy ];
    Curve2D* c = strat->getCurve( current_crosssection );


    rules_processor.update( *c, strat->getId() );


    bool can_undo = rules_processor.canUndo();
    emit enableUndo( can_undo );

    bool can_redo = rules_processor.canUndo();
    emit enableRedo( can_redo );



    update();
    return true;

}




void Controller::setCurrentStratigraphicRule( const std::string& rule )
{

    std::cout << "-- Setting rule: " << rule.c_str() << std::endl;


    if( rule.compare( "SKETCHING" ) == 0 )
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::SKETCHING );

    if( rule.compare( "RA_SKETCHING" ) == 0 )
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::RA_SKETCHING );

    if( rule.compare( "RAI_SKETCHING" ) == 0 )
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::RAI_SKETCHING );

    if( rule.compare( "RB_SKETCHING" ) == 0 )
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::RB_SKETCHING );

    if( rule.compare( "RBI_SKETCHING" ) == 0 )
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::RBI_SKETCHING );

    if( rule.compare( "DA_SKETCHING" ) == 0 )
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::DA_SKETCHING );

    if( rule.compare( "DB_SKETCHING" ) == 0 )
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::DB_SKETCHING );

    if( rule.compare( "DR_SKETCHING" ) == 0 )
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::DR_SKETCHING );


}



void Controller::undo()
{

    bool undo_ok = rules_processor.undo();
    update();

}


void Controller::redo()
{
    bool redo_ok = rules_processor.redo();
    update();
}



void Controller::update()
{


    std::cout << "-- Updating scene after applying rule" << std::endl;

    int number_of_stratigrapies = (int)stratigraphics_list.size();

    for( int i = 0; i < number_of_stratigrapies; ++i )
    {

        std::vector< float > curve_vertices;
        std::vector< unsigned int > curve_edges;

        std::vector< float > surface_vertices;
        std::vector< unsigned int > surface_faces;


        Stratigraphy* strat = stratigraphics_list[ i ];
        rules_processor.getCurve( strat->getId(), curve_vertices, curve_edges );
        rules_processor.getMesh( strat->getId(), surface_vertices, surface_faces );


//        Curve2D* c = strat->getCurve( current_crosssection );

//        int npoints = c->size();
//        for( int k = 0; k < npoints; ++k )
//        {
//            surface_vertices.push_back( c->at(k).x() );
//            surface_vertices.push_back( c->at(k).y() );
//        }

//        surface_vertices.push_back( -0.2f );
//        surface_vertices.push_back( -0.2 + i*0.05f );
//        surface_vertices.push_back( 0.2f );

//        surface_vertices.push_back( 0.2f );
//        surface_vertices.push_back( -0.2 + i*0.05f );
//        surface_vertices.push_back( 0.2f );

//        surface_vertices.push_back( 0.2f );
//        surface_vertices.push_back( -0.2 + i*0.05f );
//        surface_vertices.push_back( -0.2f  );


//        surface_vertices.push_back( -0.2f );
//        surface_vertices.push_back( -0.2 + i*0.05f );
//        surface_vertices.push_back( -0.2f );

//        surface_faces.push_back( 0 );
//        surface_faces.push_back( 1 );
//        surface_faces.push_back( 2 );

//        surface_faces.push_back( 2 );
//        surface_faces.push_back( 3 );
//        surface_faces.push_back( 0 );


        strat->updateCurve( current_crosssection, Model3DUtils::convertToCurve2D( curve_vertices ) );
        strat->updateSurface( surface_vertices, surface_faces );


    }


    // provavelmente recalcular as intersecoes novamente para cada cross section.


    emit updateScene();



}
