// Data created August 6, 2015

#include "Modeller/SketchBoardWidget/SketchBoard.hpp"

SketchBoard::SketchBoard( QWidget *parent) : QGraphicsView(parent)
{

// The following is just according to local coordinates centred at (0,0)
//
//			                   |
//					 -y        |
//					  |        |
//			  -x<---(0,0)--->x | height
//					  |        |
//					  y        |
//			                   |
//            <-----width----->

	this->setFocusPolicy(Qt::NoFocus);
}

SketchBoard::~SketchBoard()
{

}




