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

#include "Model/CrossSection.hpp"
// Model CrossSection
#include "Modeller/SketchBoardWidget/SketchSession/SketchSession.hpp"
#include "Modeller/SketchBoardWidget/SketchSession/SketchSessionTesting.hpp"

#include "StratigraphyItem.hpp"


class SketchController : public QObject
{
	Q_OBJECT

	public:

		typedef RRM::CrossSection<qreal>	     CrossSection;
		typedef CrossSection::Curve2D		     Curve2D;
		typedef CrossSection::Point2D		     Point2D;

		SketchController ( RRM::CrossSection<qreal>& _cross_section, QObject *parent = nullptr);

	        QPolygonF convertCurves ( Curve2D& _curve );
	        Curve2D   convertCurves ( QPolygonF _polygon );

	        virtual ~SketchController ( );

	public slots:

		//void smoothCurve ( QPolygonF raw_sketch_line );

        	void insertCurve ( QPolygonF _raw_sketch_curve );

		// By using a background image
        	void newSession(QPixmap pixmap);
        	// Sketching a new boundary
        	void newSession(qreal x , qreal y, qreal width, qreal height);

        	void newSection ( )
        	{
//        		this->sketch_view_->initialization ( 0.0 , 0.0 , 700 , 400 );  // The View
        	}
        signals:
		void updateSBIM(std::map<unsigned int, QPolygonF> curves_);

	private:
        	// The Model
        	RRM::CrossSection<qreal>  cross_section_;
        	// The View
};

#endif /* _SKETCHCONTROLLER_HPP_ */
