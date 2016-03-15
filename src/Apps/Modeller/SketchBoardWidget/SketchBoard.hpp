// Data created August 6, 2015

#ifndef _SURFACE_HPP_
#define _SURFACE_HPP_

#include <QtWidgets/QGraphicsView>



/**
 * @brief Contains the scene (class SketchSession).
 *
 * */

class SketchBoard : public QGraphicsView
{
	Q_OBJECT

public:
	SketchBoard( QWidget *parent = 0 );
	virtual ~SketchBoard();


private:

};

#endif /* _SURFACE_HPP_ */
