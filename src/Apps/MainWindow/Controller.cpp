#include "Controller.hpp"

#include "Model/Boundary.hpp"

#include "Core/Geometry/PolygonalCurve/PolygonalCurve2D.hpp"



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



bool Controller::addBoundary( const float& origin_x, const float& origin_y, const float& origin_z, const float& width, const float& height, const float& depth )
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


    boundary_data->edit( origin_x, origin_y, origin_z, width, height, depth );
    cross_section->setBoundary( boundary_data );


    return true;

}


void Controller::editBoundary( const float& origin_x, const float& origin_y, const float& origin_z, const float& width, const float& height, const float& depth )
{

    CrossSection* cross_section = crosssections_list[ current_crosssection ];
    Boundary* boundary_data = cross_section->getBoundary();


    boundary_data->edit( origin_x, origin_y, origin_z, width, height, depth );
    cross_section->setBoundary( boundary_data );

    update();

}


Boundary* Controller::getCurrentBoundary()
{

    CrossSection* cross_section = crosssections_list[ current_crosssection ];
    return cross_section->getBoundary();

}




void Controller::initRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth )
{


    rules_processor.setOrigin( orig_x, orig_y, orig_z );
    rules_processor.setLenght( width, height, depth );
    rules_processor.init();
    rules_processor.update( RRM::ExtrusionRulesProcessor::State::SKETCHING );


}


void Controller::editRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth )
{

    rules_processor.setOrigin( orig_x, orig_y, orig_z );
    rules_processor.setLenght( width, height, depth );
    rules_processor.init();

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

    current_stratigraphy = strat->getId();
    stratigraphics_list[ current_stratigraphy ] = strat;

    return true;


}


Stratigraphy* Controller::getCurrentStratigraphy()
{

    if( stratigraphics_list.empty() == true ) return 0;

    return stratigraphics_list[ current_stratigraphy ];

}


bool Controller::interpolateStratigraphy()
{

    if( stratigraphics_list.empty() == true ) return false;


    Stratigraphy* strat = stratigraphics_list[ current_stratigraphy ];
    Curve2D* c = strat->getCurve( current_crosssection );


    bool status_ok = rules_processor.update( *c, strat->getId() );
    if( status_ok == false ) return false;


    bool can_undo = rules_processor.canUndo();
    emit enableUndo( can_undo );

    bool can_redo = rules_processor.canRedo();
    emit enableRedo( can_redo );



    update();
    return true;

}




void Controller::setCurrentStratigraphicRule( const std::string& rule, const std::vector< unsigned int >& selected  )
{

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
    {
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::DA_SKETCHING );

        std::vector< size_t > allowed_surfaces;
//        bool da_ok = rules_processor.requestDefineRegion( allowed_surfaces );
//        if( da_ok == false ) return;

        emit waitingSelection( allowed_surfaces );

    }

    if( rule.compare( "DB_SKETCHING" ) == 0 )
    {
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::DB_SKETCHING );
        std::vector< size_t > allowed_surfaces;
//        bool db_ok = rules_processor.requestDefineRegion( allowed_surfaces );
//        if( db_ok == false ) return;

        emit waitingSelection( allowed_surfaces );

    }

    if( rule.compare( "DR_SKETCHING" ) == 0 )
    {
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::DR_SKETCHING );

        std::vector< size_t > allowed_surfaces;
//        bool db_ok = rules_processor.requestDefineRegion( allowed_surfaces );
//        if( db_ok == false ) return;

        emit waitingSelection( allowed_surfaces );

    }



    for( unsigned int i = 0; i < selected.size(); ++i )
    {
        std::cout << "Item " << selected[ i ] << " selected." << std::endl;
    }


}



void Controller::undo()
{

    bool undo_ok = rules_processor.undo();
    update();


    bool can_undo = rules_processor.canUndo();
    emit enableUndo( can_undo );

    bool can_redo = rules_processor.canRedo();
    emit enableRedo( can_redo );

}


void Controller::redo()
{
    bool redo_ok = rules_processor.redo();
    update();


    bool can_undo = rules_processor.canUndo();
    emit enableUndo( can_undo );

    bool can_redo = rules_processor.canRedo();
    emit enableRedo( can_redo );

}




void Controller::clear()
{
    current_reconstruction = ReconstructionMode::EXTRUSION;

    current_crosssection = 0.0f;
    current_stratigraphy = 0;

    stratigraphics_list.clear();
    crosssections_list.clear();

}



void Controller::update()
{


    /*
    std::vector< size_t > updated_surfaces = rules_processor.getActiveSurfaces();
    int number_of_changed_elements = (int) updated_surfaces.size();


    for( int i = 0; i < number_of_changed_elements; ++i )
    {

        std::vector< float > curve_vertices;
        std::vector< unsigned int > curve_edges;

        std::vector< float > surface_vertices;
        std::vector< unsigned int > surface_faces;

        size_t id_planin = updated_surfaces[ i ];

        Stratigraphy* strat = stratigraphics_list[ (unsigned int ) id_planin ];
        bool getcurve_ok = rules_processor.getCurve( strat->getId(), curve_vertices, curve_edges );
        bool getmesh_ok = rules_processor.getMesh ( strat->getId(), surface_vertices, surface_faces );



        if( ( getcurve_ok == false ) || ( getmesh_ok  == false ) )
        {
            strat->clear();
            continue;
        }


        strat->updateCurve( current_crosssection, Model3DUtils::convertToCurve2D( curve_vertices ) );
        strat->updateCurveWireframe( curve_edges );
        strat->updateSurface( surface_vertices, surface_faces );


    }


    // provavelmente recalcular as intersecoes novamente para cada cross section.

*/


    std::map< unsigned int, Stratigraphy* >::iterator it;

    for( it = stratigraphics_list.begin(); it != stratigraphics_list.end(); ++it )
    {

        std::vector< float > curve_vertices;
        std::vector< unsigned int > curve_edges;

        std::vector< float > surface_vertices;
        std::vector< unsigned int > surface_faces;


        Stratigraphy* strat = it->second;
        bool getcurve_ok = rules_processor.getCurve( strat->getId(), curve_vertices, curve_edges );
        bool getmesh_ok = rules_processor.getMesh ( strat->getId(), surface_vertices, surface_faces );



        if( ( getcurve_ok == false ) || ( getmesh_ok  == false ) )
        {
            strat->clear();
            continue;
        }


        strat->updateCurve( current_crosssection, Model3DUtils::convertToCurve2D( curve_vertices ) );
        strat->updateCurveWireframe( curve_edges );
        strat->updateSurface( surface_vertices, surface_faces );

    }


    // provavelmente recalcular as intersecoes novamente para cada cross section.



    emit updateScene();



}
