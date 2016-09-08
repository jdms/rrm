#include "Controller.hpp"

#include "model_new/Boundary.hpp"

#include "Core/Geometry/PolygonalCurve2D.hpp"



Controller::Controller()
{
    current_reconstruction = ReconstructionMode::EXTRUSION;

    rules_processor.init();

}




void Controller::addCrossSection( const float& d )
{

    CrossSection* cross_section = new CrossSection();
    cross_section->setDepth( d );

    crosssections_list[ d ] = cross_section;
    current_crosssection = d;

}




bool Controller::addBoundary( int w, int h )
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

    boundary_data->edit( 0, 0, w, h );
    cross_section->setBoundary( boundary_data );



    rules_processor.setOrigin( 0, 0, 0 );
    rules_processor.setLenght( w, h, 400 );


    return true;

}


void Controller::editBoundary( int x, int y, int w, int h )
{

    CrossSection* cross_section = crosssections_list[ current_crosssection ];
    Boundary* boundary_data = cross_section->getBoundary();


    boundary_data->edit( x, y, w, h );


    update();

}


Boundary* Controller::getCurrentBoundary()
{

    CrossSection* cross_section = crosssections_list[ current_crosssection ];
    return cross_section->getBoundary();

}







bool Controller::addCurve( Stratigraphy::Curve2D curve )
{

    if( stratigraphics_list.empty() == true )
        addStratigraphy();

    Stratigraphy* strat = stratigraphics_list[ current_stratigraphy ];
    strat->addCurve( current_crosssection, curve );


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


    if( stratigraphics_list.empty() == true ) return false;

    Stratigraphy* strat = stratigraphics_list[ current_stratigraphy ];
    Stratigraphy::Curve2D& c = strat->getCurve( current_crosssection );


    rules_processor.update( c, strat->getId() );


    bool can_undo = rules_processor.canUndo();
    emit enableUndo( can_undo );

    bool can_redo = rules_processor.canUndo();
    emit enableRedo( can_redo );



    update();
    return true;
}




void Controller::setCurrentStratigraphicRule( const std::string& rule )
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
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::DA_SKETCHING );

    if( rule.compare( "DB_SKETCHING" ) == 0 )
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::DB_SKETCHING );

    if( rule.compare( "DR_SKETCHING" ) == 0 )
        rules_processor.update( RRM::ExtrusionRulesProcessor::State::DR_SKETCHING );


}



void Controller::undo()
{

    bool undo_ok = rules_processor.undo();

}


void Controller::redo()
{
    bool redo_ok = rules_processor.redo();
}



void Controller::update()
{


    int number_of_stratigrapies = (int)stratigraphics_list.size();

    for( int i = 0; i < number_of_stratigrapies; ++i )
    {


        std::vector< Stratigraphy::Point2D > curve_vertices;
        std::vector< unsigned int > curve_edges;

        std::vector< Stratigraphy::Point2D > surface_vertices;
        std::vector< unsigned int > surface_faces;


        Stratigraphy* strat = stratigraphics_list[ i ];
//        rules_processor.getCurve( strat->getId(), curve_vertices, curve_edges );
//        rules_processor.getMesh( strat->getId(), surface_vertices, surface_faces );


//        strat->


    }


    emit updateScene();



}
