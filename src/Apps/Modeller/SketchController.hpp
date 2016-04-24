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

		typedef RRM::CrossSection<qreal>	     CrossSection;
		typedef CrossSection::Curve2D		     Curve2D;
		typedef CrossSection::Point2D		     Point2D;

		SketchController ( CrossSection& _cross_section , QObject *parent = nullptr );

		~SketchController ( );

		void initialize ( const CrossSection& _cross_section );

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
		void newSession ( qreal x , qreal y , qreal width , qreal height );

		void setCrossSection ( CrossSection& _cross_section );

        	void updateSBIM( );

        	void newSection ( )
        	{
//        		this->sketch_view_->initialization ( 0.0 , 0.0 , 700 , 400 );  // The View
        	}

        signals:
		void updateSBIM ( const std::map<unsigned int, QPolygonF>& , const std::map<unsigned int, QPointF>& );

	private:
        	// The Model
		CrossSection cross_section_;
        	int next;
        	// The View
};

#endif /* _SKETCHCONTROLLER_HPP_ */
