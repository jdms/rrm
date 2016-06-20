#ifndef FLOWRENDERINGOPTIONSMENU_H
#define FLOWRENDERINGOPTIONSMENU_H

#include <vector>
#include <string>

#include <QMenu>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QWidgetAction>


class FlowRenderingOptionsMenu: public QMenu
{
    Q_OBJECT

    public:

        FlowRenderingOptionsMenu( QWidget* parent );

        std::string getCurrentColorMap();
        inline bool showVertices(){ return chk_show_vertices->isChecked(); }
        inline bool showEdges(){ return chk_show_edges->isChecked(); }
        inline bool showFaces(){ return chk_show_faces->isChecked(); }




        void clear();


    protected:

        void create();
        void createActions( QWidget* parent );

        void createVisualizationMenu();
        void createPropertiesMenu();
        void createColorMapMenu();
        void createExportMenu();
        void createLoadMenu();
        void createFlowActionsMenu();

    public slots:

        void addVertexProperty( std::string name, std::string dimension );
        void addFaceProperty( std::string name, std::string dimension );



    signals:

        void coloringVertexbyScalarProperty( std::string );
        void coloringVertexbyVectorProperty( std::string, std::string );
        void coloringFacebyScalarProperty( std::string );
        void coloringFacebyVectorProperty( std::string, std::string );

        void setConstantColormap();
        void setJETColormap();

        void reloadcrosssection();
        void loadfile();
        void editparameters();
        void buildvolumetric();
        void computeproperties();
        void applycrosssection();

        void clearAll();


    private:

        QCheckBox* chk_show_vertices;
        QCheckBox* chk_show_edges;
        QCheckBox* chk_show_faces;
        QCheckBox* chk_show_bbox;

        QWidgetAction* wa_visualization_options;

        QMenu *mn_coloring_byvertex;
        QMenu *mn_coloring_byfaces;

        QAction* ac_exportsurfacetovtk;
        QAction* ac_exportvolumetovtk;
        QAction* ac_exportcornerpointtovtk;

        std::vector< QMenu *> mn_vectorsproperties_byvertex;
        std::vector< QRadioButton *> rd_vectormethods_byvertex;
        QWidgetAction* wa_vectormethods_byvertex;
        std::vector<QAction* > ac_vertex_property;


        std::vector< QMenu *> mn_vectorsproperties_byface;
        std::vector< QRadioButton *> rd_vectormethods_byface;
        QWidgetAction* wa_vectormethods_byface;
        std::vector<QAction* > ac_face_property;


        QRadioButton* rd_colormap_constant;
        QRadioButton* rd_colormap_JET;
        QWidgetAction* wa_colormaps;


        QAction* ac_reloadcrosssection;
        QAction* ac_loadfile;
        QAction* ac_editparameters;
        QAction* ac_buildvolumetric;
        QAction* ac_computeproperties;
        QAction* ac_applycrosssection;

        QAction* ac_clear;

};

#endif // FLOWRENDERINGOPTIONSMENU_H
