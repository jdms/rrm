#ifndef PLANEITEM_H
#define PLANEITEM_H

#include <QGraphicsPolygonItem>


/**
 *  A data structure to help to represent the plane in the QGraphicsScene
 *  It inherits from QGraphicsRectItem.
 */

class PlaneItem: public QGraphicsRectItem
{
    public:


        /**
        * Constructor. parent_ is passed to QGraphicsRectItem's constructor.
        * @param parent_ the parent item
        */
        PlaneItem( QGraphicsItem *parent_ = nullptr );


        /**
        * Destructor.
        */
        ~PlaneItem() override;


        /**
        * Method to get the bounding box of the plane
        * @see QRectF
        * @return QRectF the bounding box of the plane
        */
        QRectF boundingRect() const override ;


        /**
        * This method set the bottom-left corner of the plane
        * @param start_ the bottom-left corner of the plane
        * @see QPointF
        * @return void.
        */
        void setStartPoint( const QPointF& start_ );


        /**
        * This method set the right-top corner of the plane
        * @param end_ the right-top corner of the plane
        * This point helps to compute the plane dimensions
        * @see QPointF
        * @return void.
        */
        void setEndPoint( const QPointF& end_ );


        /**
        * This method define new dimensions to the plane
        * @param dimensions the x coordinate represents the plane width and the second the plane height
        * @param an optional parameter representing the origin of the plane, i.e., the bottom left corner of the plane
        * @see QPointF
        * @see publicVar()
        * @return void.
        */
        void resize( const QPointF& dimensions_, const QPointF& origin_ = QPointF( 0.0f, 0.0f ) );


        /**
        * This method define new dimensions to the plane
        * @param width_ the plane width
        * @param height_ the plane height
        * @param ox_ an optional parameter representing the x coordinate of the origin of the plane
        * @param oy_ an optional parameter representing the y coordinate of the origin of the plane
        * @return void.
        */
        void resize( double width_, double height_, double ox_ = 0.0, double oy_ = 0.0 );


        /**
        * Method to clear the plane and reset all members to their default values
        * @return void.
        */
        void clear();


        /**
        * Method to check if the plane is empty
        * @return Boolean. It is true if the plane is empty and false if it is not.
        */
        bool isEmpty() const;


        /**
        * Method to set the color of the border
        * @param red the red component of the color (integer)
        * @param green the green component of the color (integer)
        * @param blue the blue component of the color (integer)
        * @return void.
        */
        void setBorderColor( int r_, int g_, int b_ );


        /**
        * Method to get the color of the border
        * @param red reference to the red component of the color (integer)
        * @param green reference to the green component of the color (integer)
        * @param blue reference to the blue component of the color (integer)
        * @return void.
        */
        void getBorderColor( int& r_, int& g_, int& b_ ) const;


        /**
        * Method to set the color of the border using a QColor
        * @param color the new color of the border
        * @return void.
        */
        void setBorderColor( const QColor& color_ );


        /**
        * Method to get the color of the border by QColor
        * @return QColor
        */
        QColor getBorderColor() const;


        /**
        * Method to set the width of the border line
        * @param width the width of the border line
        * @return void.
        */
        void setBorderWidth( int width_ );


        /**
        * Method to get the width of the border line
        * @return int the width of the border line
        */
        int getBorderWidth() const;


        /**
        * Method to fill the plane with a given color
        * @param red the red component of the color (integer)
        * @param green the green component of the color (integer)
        * @param blue the blue component of the color (integer)
        * @return void.
        */
        void setFillColor( int r_, int g_, int b_ );


        /**
        * Method to get the color of internal part of the plane
        * @param red reference to the red component of the color (integer)
        * @param green reference to the green component of the color (integer)
        * @param blue reference to the blue component of the color (integer)
        * @return void.
        */
        void getFillColor( int& r_, int& g_, int& b_ ) const;


        /**
        * Method to fill the plane with a given QColor
        * @param color the new color of the curve
        * @see QColor
        * @return void.
        */
        void setFillColor( const QColor& color_ );


        /**
        * Method to get the color of internal part of the plane by QColor
        * @see QColor
        * @return QColor
        */
        QColor getFillColor() const;


        /**
        * Method to reset the color of the border to default.
        * @see DEFAULT_LINE_COLOR
        * @return void.
        */
        void resetToDefaultBorderColor();


        /**
        * Method to reset the color of internal part of the plane to default.
        * @see DEFAULT_LINE_COLOR
        * @return void.
        */
        void resetToDefaultFillColor();


        /**
        * Method to reset the width of the border to default.
        * @see DEFAULT_LINE_WIDTH
        * @return void.
        */
        void resetToDefaultBorderWidth();


        /**
        * This method checks if the plane should be visible or not
        * @see QGraphicsRectItem::isVisible()
        * @return boolean. Returns true if the plane is visible, and false otherwise.
        */
        virtual bool isVisible() const { return QGraphicsRectItem::isVisible(); }


        /**
        * This method checks if the plane should be selected or not
        * @see QGraphicsRectItem::isSelected()
        * @return boolean. Returns true if the plane is selected, and false otherwise.
        */
        virtual bool isSelected() const { return QGraphicsRectItem::isSelected(); }



    protected:


        /**
        * Method to repaint the curve
        * @see QPainter performs painting on the widget
        * @see QStyleOptionGraphicsItem provides style options for the item
        * @see QWidget  it is an optional parameter. If provided, it points to the widget that is being painted on; otherwise, it is nullptr
        * @return void
        */
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                    QWidget* widget ) override;


        /**
        * @struct Color
        * @brief It is a data structure to represent color
        * @var red red component of a color
        * @var green green component of a color
        * @var blue blue component of a color
        */
        struct Color{ int red = 255, green = 0, blue = 0;
                      Color( int r_, int g_, int b_ ): red( r_ ), green( g_ ),
                                                       blue( b_ ){} };


        QRectF plane;                                                               /**< A data structure to hold the plane data */

        const int DEFAULT_BORDER_WIDTH = 1;                                         /**< An integer to define the default width of the border  */

        int border_width = DEFAULT_BORDER_WIDTH;                                    /**< An integer to define the current width of the border  */

        Color DEFAULT_BORDER_COLOR = { 0, 200, 0 };                                 /**< A Color to define the default color of the border  */

        Color DEFAULT_FILL_COLOR  = { 55, 100, 55 };                                /**< A Color to define the default color of the plane  */

        Color border_color = DEFAULT_BORDER_COLOR;                                  /**< A Color to define the current color of the border  */

        Color fill_color = DEFAULT_FILL_COLOR;                                      /**< A Color to define the current color of the plane  */

};

#endif // PLANEITEM_H
