#include "FlowRenderingOptionsMenu.h"


FlowRenderingOptionsMenu::FlowRenderingOptionsMenu( QWidget* parent )
{
    create();
    createActions( parent );
}

void FlowRenderingOptionsMenu::create()
{


    createPropertiesMenu();
    createColorMapMenu();
    createVisualizationMenu();


    addMenu( mn_coloring_byvertex );
    addMenu( mn_coloring_byfaces );
    addAction ( wa_visualization_options );
    addAction ( wa_colormaps );

}


void FlowRenderingOptionsMenu::createActions( QWidget* parent ){


    connect( chk_show_vertices, SIGNAL( clicked( bool ) ), parent, SLOT( showVertices( bool ) ) );
    connect( chk_show_edges, SIGNAL( clicked( bool ) ), parent, SLOT( showEdges( bool ) ) );
    connect( chk_show_faces, SIGNAL( clicked( bool ) ), parent, SLOT( showFaces( bool ) ) );


}


void FlowRenderingOptionsMenu::createVisualizationMenu()
{
    chk_show_vertices = new QCheckBox ( tr ( "Show Points" ) );
    chk_show_vertices->setChecked ( false );

    chk_show_edges = new QCheckBox ( tr ( "Show WireFrame" ) );
    chk_show_edges->setChecked ( false );

    chk_show_faces = new QCheckBox ( tr ( "Show Volume" ) );
    chk_show_faces->setChecked ( true );


    QVBoxLayout *vb_layout = new QVBoxLayout;
    vb_layout->addWidget ( chk_show_vertices );
    vb_layout->addWidget ( chk_show_edges );
    vb_layout->addWidget ( chk_show_faces );

    QGroupBox *gb_visualization = new QGroupBox( "Rendering" );
    gb_visualization->setFlat ( true );
    gb_visualization->setLayout ( vb_layout );

    wa_visualization_options = new QWidgetAction ( this );
    wa_visualization_options->setDefaultWidget ( gb_visualization );

}


void FlowRenderingOptionsMenu::createColorMapMenu()
{

    rd_colormap_constant = new QRadioButton( tr ( "Constant" ) );
    rd_colormap_JET = new QRadioButton( tr ( "JET" ) );

    rd_colormap_constant->setChecked( true );
    rd_colormap_JET->setChecked( false );


    QVBoxLayout *vb_layout = new QVBoxLayout;
    vb_layout->addWidget ( rd_colormap_constant );
    vb_layout->addWidget ( rd_colormap_JET );

    QGroupBox *gb_colormaps = new QGroupBox ( "Colormaps ");
    gb_colormaps->setFlat ( true );
    gb_colormaps->setLayout ( vb_layout );

    wa_colormaps = new QWidgetAction ( this );
    wa_colormaps->setDefaultWidget ( gb_colormaps );


    connect ( rd_colormap_constant , &QRadioButton::toggled , this , [=](){ emit setConstantColormap(); } );
    connect ( rd_colormap_JET, &QRadioButton::toggled , this , [=](){ emit setJETColormap(); } );

}

void FlowRenderingOptionsMenu::createPropertiesMenu()
{
    mn_coloring_byvertex = new QMenu ( tr ( "Vertex Properties" ) );
    mn_coloring_byfaces = new QMenu ( tr ( "Faces Properties" ) );
}

void FlowRenderingOptionsMenu::addVertexProperty( std::string name, std::string dimension )
{

    if( dimension.compare( "VECTOR" ) == 0 )
    {
        int id = mn_vectorsproperties_byvertex.size();

        mn_vectorsproperties_byvertex.push_back( new QMenu( tr( name.c_str() ) ) );

        rd_vectormethods_byvertex.push_back ( new QRadioButton ( tr ( "X" ) ) );
        rd_vectormethods_byvertex.push_back ( new QRadioButton ( tr ( "Y" ) ) );
        rd_vectormethods_byvertex.push_back ( new QRadioButton ( tr ( "Z" ) ) );
        rd_vectormethods_byvertex.push_back ( new QRadioButton ( tr ( "Magnitude" ) ) );

        QVBoxLayout *vb_layout = new QVBoxLayout ( this );
        vb_layout->addWidget ( rd_vectormethods_byvertex[ 4*id + 0 ] );
        vb_layout->addWidget ( rd_vectormethods_byvertex[ 4*id + 1 ] );
        vb_layout->addWidget ( rd_vectormethods_byvertex[ 4*id + 2 ] );
        vb_layout->addWidget ( rd_vectormethods_byvertex[ 4*id + 3 ] );

        QGroupBox *gb_vectormethods = new QGroupBox ();
        gb_vectormethods->setFlat ( true );
        gb_vectormethods->setLayout ( vb_layout );

        wa_vectormethods_byvertex = new QWidgetAction ( this );
        wa_vectormethods_byvertex->setDefaultWidget ( gb_vectormethods );


        mn_vectorsproperties_byvertex[ id ]->addAction( wa_vectormethods_byvertex );


        connect ( rd_vectormethods_byvertex[ 4*id + 0 ] , &QRadioButton::clicked, [=](){ emit coloringVertexbyVectorProperty( name.c_str(), "COORDX" ); } );
        connect ( rd_vectormethods_byvertex[ 4*id + 1 ] , &QRadioButton::clicked, [=](){ emit coloringVertexbyVectorProperty( name.c_str(), "COORDY" ); } );
        connect ( rd_vectormethods_byvertex[ 4*id + 2 ] , &QRadioButton::clicked, [=](){ emit coloringVertexbyVectorProperty( name.c_str(), "COORDZ" ); } );
        connect ( rd_vectormethods_byvertex[ 4*id + 3 ] , &QRadioButton::clicked, [=](){ emit coloringVertexbyVectorProperty( name.c_str(), "LENGTH" ); } );

        mn_coloring_byvertex->addMenu( mn_vectorsproperties_byvertex[ id ] );

    }
    else if( dimension.compare( "SCALAR" ) == 0 )
    {
        ac_vertex_property.push_back( new QAction( tr( name.c_str() ) , this ) );
        mn_coloring_byvertex->addAction( ac_vertex_property.back() );

        connect ( ac_vertex_property.back() , &QAction::triggered ,[=](){ emit coloringVertexbyScalarProperty( name.c_str() ); } );


    }


}


void FlowRenderingOptionsMenu::addFaceProperty( std::string name, std::string dimension )
{

    if( dimension.compare( "VECTOR" ) == 0 )
    {
        int id = mn_vectorsproperties_byface.size();

        mn_vectorsproperties_byface.push_back( new QMenu( tr( name.c_str() ) ) );

        rd_vectormethods_byface.push_back ( new QRadioButton ( tr ( "X" ) ) );
        rd_vectormethods_byface.push_back ( new QRadioButton ( tr ( "Y" ) ) );
        rd_vectormethods_byface.push_back ( new QRadioButton ( tr ( "Z" ) ) );
        rd_vectormethods_byface.push_back ( new QRadioButton ( tr ( "Magnitude" ) ) );

        QVBoxLayout *vb_layout = new QVBoxLayout ( this );
        vb_layout->addWidget ( rd_vectormethods_byface[ 4*id + 0 ] );
        vb_layout->addWidget ( rd_vectormethods_byface[ 4*id + 1 ] );
        vb_layout->addWidget ( rd_vectormethods_byface[ 4*id + 2 ] );
        vb_layout->addWidget ( rd_vectormethods_byface[ 4*id + 3 ] );

        QGroupBox *gb_vectormethods = new QGroupBox ();
        gb_vectormethods->setFlat ( true );
        gb_vectormethods->setLayout ( vb_layout );

        wa_vectormethods_byface = new QWidgetAction ( this );
        wa_vectormethods_byface->setDefaultWidget ( gb_vectormethods );


        mn_vectorsproperties_byface[ id ]->addAction( wa_vectormethods_byface );


        connect ( rd_vectormethods_byface[ 4*id + 0 ] , &QRadioButton::clicked, [=](){ emit coloringFacebyVectorProperty( name.c_str(), "COORDX" ); } );
        connect ( rd_vectormethods_byface[ 4*id + 1 ] , &QRadioButton::clicked, [=](){ emit coloringFacebyVectorProperty( name.c_str(), "COORDY" ); } );
        connect ( rd_vectormethods_byface[ 4*id + 2 ] , &QRadioButton::clicked, [=](){ emit coloringFacebyVectorProperty( name.c_str(), "COORDZ" ); } );
        connect ( rd_vectormethods_byface[ 4*id + 3 ] , &QRadioButton::clicked, [=](){ emit coloringFacebyVectorProperty( name.c_str(), "LENGTH" ); } );

        mn_coloring_byfaces->addMenu( mn_vectorsproperties_byface[ id ] );

    }
    else if( dimension.compare( "SCALAR" ) == 0 )
    {
        ac_face_property.push_back( new QAction( tr( name.c_str() ) , this ) );
        mn_coloring_byfaces->addAction( ac_face_property.back() );

        connect ( ac_face_property.back() , &QAction::triggered ,[=](){ emit coloringFacebyScalarProperty( name.c_str() ); } );


    }


}
std::string FlowRenderingOptionsMenu::getCurrentColorMap()
{
    if( rd_colormap_constant->isChecked() == true )
        return "CONSTANT";

    else if( rd_colormap_JET->isChecked() == true )
        return "JET";
}

