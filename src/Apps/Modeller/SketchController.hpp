/*
 * SketchController.hpp
 *
 *  Created on: Mar 21, 2016
 *      Author: felipe
 */

#ifndef _SKETCHCONTROLLER_HPP_
#define _SKETCHCONTROLLER_HPP_

// Qt5
#include <QtCore/QObject>
#include <QtCore/QtGlobal>
#include <QtGui/QPixmap>
// Model CrossSection
#include "Model/CrossSectionTesting.hpp"
#include "Modeller/SketchBoardWidget/SketchSession/SketchSession.hpp"

#include "Modeller/HorizonController.h"


class SketchController : public QObject
{
	Q_OBJECT

	public:

		typedef RRM::CrossSectionTesting<qreal>            CrossSection;
		typedef CrossSection::Curve2D			   Curve2D;

		SketchController ( CrossSection* _cross_section, SketchSession* _sketch_view , QObject *parent = nullptr);

	        QPolygonF covertCurves ( Curve2D& _curve );
	        Curve2D   covertCurves ( QPolygonF _polygon );

	        void smoothCurve ( QPolygonF raw_sketch_line );

	        void insertCurve ( QPolygonF _raw_sketch_line );

	        virtual ~SketchController ( );

	public slots:

		// By using a background image
        	void newSession(QPixmap pixmap);
        	// Sketching a new boundary
        	void newSession(qreal x , qreal y, qreal width, qreal height);

        	void newSection ( )
        	{
        		this->sketch_view_->initialization ( 0.0 , 0.0 , 700 , 400 );  // The View
        		this->cross_section_->initialization ( 0.0 , 0.0 , 700 , 400 );   // The  Model
        	}

	private:
        	// The Model
        	CrossSection*   cross_section_;
        	// The View
        	SketchSession* sketch_view_;
};

#endif /* _SKETCHCONTROLLER_HPP_ */
