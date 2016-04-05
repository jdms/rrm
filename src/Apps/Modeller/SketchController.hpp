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
#include "Modeller/SketchBoardWidget/SketchSession/SketchSessionTesting.hpp"

#include "Model/Arrangment.hpp"

#include "Modeller/HorizonController.h"


class SketchController : public QObject
{
	Q_OBJECT

	public:

		typedef RRM::CrossSectionTesting<qreal>            CrossSection;
		typedef CrossSection::Curve2D			   Curve2D;

		SketchController ( CrossSection* _cross_section, SketchSessionTesting* _sketch_view , QObject *parent = nullptr);

	        QPolygonF convertCurves ( Curve2D& _curve );
	        Curve2D   convertCurves ( QPolygonF _polygon );

	        virtual ~SketchController ( );

	public slots:

		//void smoothCurve ( QPolygonF raw_sketch_line );

        	void insertCurve ( QPolygonF _raw_sketch_line );

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
        	RRM::Arrangment<qreal>  arrangment;
        	// The View
        	SketchSessionTesting* sketch_view_;
};

#endif /* _SKETCHCONTROLLER_HPP_ */
