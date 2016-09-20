#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>

#include <vector>

#include "Sketching/BoundaryItem2D.hpp"
#include "model_new/CrossSection.hpp"
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


        bool addBoundary( const float& w, const float& h, const float& d );
        void editBoundary( const float& x, const float& y, const float& w, const float& h, const float& d );
        Boundary* getCurrentBoundary();



        bool addCurve( const Curve2D &curve );
        bool addStratigraphy();
		bool interpolateStratigraphy();
        Stratigraphy* getCurrentStratigraphy();


        void initRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth );
        void editRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth );



    public slots:


        void setCurrentStratigraphicRule( const std::string& rule );


        void undo();
        void redo();


        void update();



    signals:

        void updateScene();
        void enableUndo( bool option );
        void enableRedo( bool option );
		

    protected:


        ReconstructionMode current_reconstruction;
	
        float current_crosssection;
        unsigned int current_stratigraphy;
	
        std::vector< Stratigraphy* > stratigraphics_list;
        std::map< float, CrossSection* > crosssections_list;

        RRM::ExtrusionRulesProcessor rules_processor;

};


#endif
