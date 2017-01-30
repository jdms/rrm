#include "FlowRenderingOptionsMenu.h"


FlowRenderingOptionsMenu::FlowRenderingOptionsMenu( QWidget* parent )
{
    create();
    createActions( parent );
}

void FlowRenderingOptionsMenu::create()
{

    createLoadMenu();
    createFlowActionsMenu();
    createPropertiesMenu();
    createColorMapMenu();
    createVisualizationMenu();
    createExportMenu();


    addSection( "" );

    addSection( "Properties" );

    addMenu( mn_coloring_byvertex );
    addMenu( mn_coloring_byfaces );

    addSeparator();

    addAction ( wa_visualization_options );

    addSeparator();

    addMenu ( mn_colormaps );

    addSeparator();

    //addMenu( mn_export );

    addSeparator();

    ac_clear = new QAction( "Clear", this );
    addAction( ac_clear );

}


void FlowRenderingOptionsMenu::createActions( QWidget* parent ){


    connect( chk_show_bbox, SIGNAL( clicked( bool ) ), parent, SLOT( showBoundingBox( bool ) ) );
    connect( ac_clear,  &QAction::triggered, this, [=](){ emit clearAll(); } );

}


void FlowRenderingOptionsMenu::createLoadMenu()
{

}


void FlowRenderingOptionsMenu::createFlowActionsMenu()
{
}


void FlowRenderingOptionsMenu::createVisualizationMenu()
{
    chk_show_bbox = new QCheckBox ( tr ( "Show Bounding Box" ) );
    chk_show_bbox->setChecked ( true );


    QVBoxLayout *vb_layout = new QVBoxLayout;
    vb_layout->addWidget ( chk_show_bbox );

    QWidget *wd_visualization = new QWidget();
    wd_visualization->setLayout ( vb_layout );

    wa_visualization_options = new QWidgetAction ( this );
    wa_visualization_options->setDefaultWidget ( wd_visualization );

}


void FlowRenderingOptionsMenu::createColorMapMenu()
{

    rd_colormap_constant = new QRadioButton( tr ( "Constant" ) );
    rd_colormap_JET = new QRadioButton( tr ( "JET" ) );
    rd_colormap_hot = new QRadioButton( tr ( "Hot" ) );
    rd_colormap_cool = new QRadioButton( tr ( "Cool" ) );
    rd_colormap_spring = new QRadioButton( tr ( "Spring" ) );
    rd_colormap_summer = new QRadioButton( tr ( "Summer" ) );
    rd_colormap_copper = new QRadioButton( tr ( "Copper" ) );
    rd_colormap_polar = new QRadioButton( tr ( "Polar" ) );
    rd_colormap_winter = new QRadioButton( tr ( "Winter" ) );

    rd_colormap_constant->setChecked( true );
    rd_colormap_JET->setChecked( false );
    rd_colormap_hot->setChecked( false );
    rd_colormap_cool->setChecked( false );
    rd_colormap_spring->setChecked( false );
    rd_colormap_summer->setChecked( false );
    rd_colormap_copper->setChecked( false );
    rd_colormap_polar->setChecked( false );
    rd_colormap_winter->setChecked(  false );


    QVBoxLayout *vb_layout = new QVBoxLayout;
    vb_layout->addWidget ( rd_colormap_constant );
    vb_layout->addWidget ( rd_colormap_JET );
    vb_layout->addWidget ( rd_colormap_hot );
    vb_layout->addWidget ( rd_colormap_cool );
    vb_layout->addWidget ( rd_colormap_spring );
    vb_layout->addWidget ( rd_colormap_summer );
    vb_layout->addWidget ( rd_colormap_copper );
    vb_layout->addWidget ( rd_colormap_polar );
    vb_layout->addWidget ( rd_colormap_winter );

    QGroupBox *gb_colormaps = new QGroupBox ( "Colormaps ");
    gb_colormaps->setFlat ( true );
    gb_colormaps->setLayout ( vb_layout );

    wa_colormaps = new QWidgetAction ( this );
    wa_colormaps->setDefaultWidget ( gb_colormaps );

    mn_colormaps = new QMenu( tr( "Colormaps" ) );
    mn_colormaps->addAction( wa_colormaps );


    connect ( rd_colormap_constant , &QRadioButton::toggled , this , [=](){ emit setConstantColormap(); } );
    connect ( rd_colormap_JET, &QRadioButton::toggled , this , [=](){ emit setJETColormap(); } );
    connect ( rd_colormap_hot, &QRadioButton::toggled , this , [=](){ emit seHotColormap(); } );
    connect ( rd_colormap_cool, &QRadioButton::toggled , this , [=](){ emit setCoolColormap(); } );
    connect ( rd_colormap_spring, &QRadioButton::toggled , this , [=](){ emit setSpringColormap(); } );
    connect ( rd_colormap_summer, &QRadioButton::toggled , this , [=](){ emit setSummerColormap(); } );
    connect ( rd_colormap_copper, &QRadioButton::toggled , this , [=](){ emit setCopperColormap(); } );
    connect ( rd_colormap_polar, &QRadioButton::toggled , this , [=](){ emit setPolarColormap(); } );
    connect ( rd_colormap_winter, &QRadioButton::toggled , this , [=](){ emit setWinterColormap(); } );

}


void FlowRenderingOptionsMenu::createPropertiesMenu()
{
    mn_coloring_byvertex = new QMenu ( tr ( "Vertex Properties" ) );
    mn_coloring_byfaces = new QMenu ( tr ( "Faces Properties" ) );
}


void FlowRenderingOptionsMenu::createExportMenu()
{
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


        QWidget *wd_vectormethods = new QWidget();
        wd_vectormethods->setLayout ( vb_layout );


        wa_vectormethods_byvertex = new QWidgetAction ( this );
        wa_vectormethods_byvertex->setDefaultWidget ( wd_vectormethods );


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

        QWidget *wd_vectormethods = new QWidget();
        wd_vectormethods->setLayout ( vb_layout );


        wa_vectormethods_byface = new QWidgetAction ( this );
        wa_vectormethods_byface->setDefaultWidget ( wd_vectormethods );


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

    else if( rd_colormap_hot->isChecked() == true )
        return "JET";

    else if( rd_colormap_cool->isChecked() == true )
        return "COOL";

    else if( rd_colormap_spring->isChecked() == true )
        return "SPRING";

    else if( rd_colormap_summer->isChecked() == true )
        return "SUMMER";

    else if( rd_colormap_copper->isChecked() == true )
        return "COPPER";

    else if( rd_colormap_polar->isChecked() == true )
        return "POLAR";

    else if( rd_colormap_winter->isChecked() == true )
        return "WINTER";


}


void FlowRenderingOptionsMenu::clear()
{

    mn_vectorsproperties_byvertex.clear();
    rd_vectormethods_byvertex.clear();
    ac_vertex_property.clear();

    mn_coloring_byvertex->clear();


    mn_vectorsproperties_byface.clear();
    rd_vectormethods_byface.clear();

    ac_face_property.clear();

    mn_coloring_byfaces->clear();

}
