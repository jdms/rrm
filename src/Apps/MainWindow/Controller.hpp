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


#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>

#include <vector>

#include "Sketching/BoundaryItem2D.hpp"
#include "./src/Core/Geology/Models/CrossSection.hpp"
#include "Sketching/StratigraphicItem.hpp"

#include "MainWindow/ExtrusionRulesProcessor.hpp"

#include "3dView/Model3DUtils.hpp"


class Controller: public QObject
{

    Q_OBJECT

	public: 
	

		enum class ReconstructionMode { EXTRUSION, INTERPOLATION };
		
	
        Controller();


        void addCrossSection( const float& d = 0.0f );
        inline bool hasCrossSection(){ return !( crosssections_list.empty() ); }
        float getCurrentCrossSection(){ return current_crosssection; }


        bool addBoundary( const float& origin_x, const float& origin_y, const float& origin_z, const float& width, const float& height, const float& depth );
        void editBoundary( const float& origin_x, const float& origin_y, const float& origin_z, const float& width, const float& height, const float& depth );
        Boundary* getCurrentBoundary();



        bool addCurve( const Curve2D &curve );
        bool addStratigraphy();
        bool interpolateStratigraphy( const std::vector< size_t >& lower_bound = std::vector< size_t >(),
                                      const std::vector< size_t >& upper_bound = std::vector< size_t >() );

        Stratigraphy* getCurrentStratigraphy();


        bool defineSketchingAbove( std::vector< size_t >& allowed );
        bool defineSketchingBelow( std::vector< size_t >& allowed );

        bool defineRegionAbove( const std::vector< size_t >& selections );
        bool defineRegionBelow( const std::vector< size_t >& selections );



        bool stopSketchingAbove();
        bool stopSketchingBelow();


        void initRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth );
        void editRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth );



    public slots:


        size_t getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );

        void setStratigraphicRule( const std::string& rule );
        void changeResolution( const int numI_, const int numJ_ );

        void undo();
        void redo();


        void clear();
        void update();



    signals:

        void updateScene();
        void enableUndo( bool option );
        void enableRedo( bool option );
        void removeStratigraphy( size_t id );
        void waitingSelection( bool, const std::vector< size_t >& );
        void changeStratigraphyRulesStatus( const std::string& );
        void changeDefineRegionStatus( const bool, const bool );
        void pickingRegion( bool );
		

    protected:


        ReconstructionMode current_reconstruction;
	
        float current_crosssection;
        size_t current_stratigraphy;
	
        std::map< size_t, Stratigraphy* > stratigraphics_list;
        std::map< size_t, size_t > index_stratigraphy_map;
        std::map< float, CrossSection* > crosssections_list;
        RRM::ExtrusionRulesProcessor rules_processor;


};


#endif
