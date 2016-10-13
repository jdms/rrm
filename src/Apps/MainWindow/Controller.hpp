#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>

#include <vector>

#include "Sketching/BoundaryItem2D.hpp"
#include "Model/CrossSection.hpp"
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
		bool interpolateStratigraphy();
        Stratigraphy* getCurrentStratigraphy();

        bool defineRegion( const std::vector< size_t >& surfaces );

        void initRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth );
        void editRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth );



    public slots:


        void setCurrentStratigraphicRule( const std::string& rule, const std::vector< unsigned int >& selected );


        void undo();
        void redo();


        void clear();
        void update();



    signals:

        void updateScene();
        void enableUndo( bool option );
        void enableRedo( bool option );
        void removeStratigraphy( unsigned int id );
        void waitingSelection( const std::vector< size_t >& );
		

    protected:


        ReconstructionMode current_reconstruction;
	
        float current_crosssection;
        unsigned int current_stratigraphy;
	
        std::map< unsigned int, Stratigraphy* > stratigraphics_list;
        std::map< unsigned int, unsigned int > index_stratigraphy_map;
        std::map< float, CrossSection* > crosssections_list;
        RRM::ExtrusionRulesProcessor rules_processor;


        std::string save_rule;
};


#endif
