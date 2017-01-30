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
		void clear();


    signals:

        void coloringVertexbyScalarProperty( std::string );
        void coloringVertexbyVectorProperty( std::string, std::string );
        void coloringFacebyScalarProperty( std::string );
        void coloringFacebyVectorProperty( std::string, std::string );

        void setConstantColormap();
        void setJETColormap();
        void seHotColormap();
        void setCoolColormap();
        void setParulaColormap();
        void setSpringColormap();
        void setSummerColormap();
        void setCopperColormap();
        void setPolarColormap();
        void setWinterColormap();

        void clearAll();


    private:


        QCheckBox* chk_show_bbox;

        QWidgetAction* wa_visualization_options;

        QMenu *mn_coloring_byvertex;
        QMenu *mn_coloring_byfaces;


        std::vector< QMenu *> mn_vectorsproperties_byvertex;
        std::vector< QRadioButton *> rd_vectormethods_byvertex;
        QWidgetAction* wa_vectormethods_byvertex;
        std::vector<QAction* > ac_vertex_property;


        std::vector< QMenu *> mn_vectorsproperties_byface;
        std::vector< QRadioButton *> rd_vectormethods_byface;
        QWidgetAction* wa_vectormethods_byface;
        std::vector<QAction* > ac_face_property;

        QMenu* mn_colormaps;
        QRadioButton* rd_colormap_constant;
        QRadioButton* rd_colormap_JET;
        QRadioButton* rd_colormap_hot;
        QRadioButton* rd_colormap_cool;
        QRadioButton* rd_colormap_parula;
        QRadioButton* rd_colormap_spring;
        QRadioButton* rd_colormap_summer;
        QRadioButton* rd_colormap_copper;
        QRadioButton* rd_colormap_polar;
        QRadioButton* rd_colormap_winter;
        QWidgetAction* wa_colormaps;


        QAction* ac_clear;

};

#endif // FLOWRENDERINGOPTIONSMENU_H
