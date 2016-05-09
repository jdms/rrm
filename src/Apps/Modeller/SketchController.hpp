/*
 * SketchController.hpp
 *
 *  Created on: Mar 21, 2016
 *      Author: Felipe Moura de Carvalho
 */

#ifndef _SKETCHCONTROLLER_HPP_
#define _SKETCHCONTROLLER_HPP_

// Qt5
#include <QtCore/QObject>
#include <QtCore/QtGlobal>
#include <QtGui/QPixmap>

// Model CrossSection
#include "Model/CrossSection.hpp"

#include "StratigraphyItem.hpp"

class SketchController : public QObject
{

	Q_OBJECT

	public:

		typedef qreal				     Real;
		typedef RRM::CrossSection<Real>	     	     CrossSection;
		typedef CrossSection::Curve2D		     Curve2D;
		typedef CrossSection::Point2D		     Point2D;

		SketchController ( QObject *parent = nullptr );

		~SketchController ( );

		void clear ( );

	        QPolygonF convertCurves ( Curve2D& _curve );
	        Curve2D   convertCurves ( QPolygonF _polygon );

		void crossSection_1 ( CrossSection& _cross_section , double scale = 100 );
		void crossSection_2 ( CrossSection& _cross_section , double scale = 100 );
		void crossSection_3 ( CrossSection& _cross_section , double scale = 100 );

	public slots:

        	void insertCurve ( QPolygonF _raw_sketch_curve );

		// By using a background image
		void newSession ( QPixmap pixmap );
		// Sketching a new boundary
		void newSession ( Real x , Real y , Real width , Real height );

		void setCrossSection ( const CrossSection& _cross_section );
		CrossSection getCrossSection ( ) const ;

        	void updateSBIM( );

        signals:
		void updateSBIM        ( const std::map<unsigned int, QPolygonF>& ,
					 const std::map<unsigned int, QPointF>& );
	private:
        	// The Model
		CrossSection cross_section_;
        	int next;
        	// The View
};

#endif /* _SKETCHCONTROLLER_HPP_ */
