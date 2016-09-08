#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>

#include <vector>

#include "Sketching/BoundaryItem2D.hpp"
#include "model_new/CrossSection.hpp"
#include "Sketching/StratigraphicItem.hpp"

#include "MainWindow/ExtrusionRulesProcessor.hpp"


class Controller: public QObject
{

    Q_OBJECT

	public: 
	

		enum class ReconstructionMode { EXTRUSION, INTERPOLATION };
		
	
        Controller();


        void addCrossSection( const float& d = 0.0f );
        inline bool hasCrossSection(){ return !( crosssections_list.empty() ); }


        bool addBoundary( int w, int h );
        void editBoundary( int x, int y, int w, int h );
        Boundary* getCurrentBoundary();



        bool addCurve( Stratigraphy::Curve2D curve );
        bool addStratigraphy();
		bool interpolateStratigraphy();
        Stratigraphy* getCurrentStratigraphy();



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
