#ifndef POLYGONITEM_H
#define POLYGONITEM_H

#include <QGraphicsPolygonItem>
#include<QGraphicsSceneMouseEvent>


/**
 *  A data structure to help to represent a polygon in the QGraphicsScene
 *  It inherits from QGraphicsPolygonItem.
 */

class PolygonItem: public QGraphicsPolygonItem
{

    public:

        /**
        * Constructor.
        */
        PolygonItem( QGraphicsItem *parent_ = nullptr );


        /**
        * Destructor.
        */
        ~PolygonItem() override;


        /**
        * Method to get the bounding box of the polygon
        * @see testMeToo()
        * @see publicVar()
        * @return QRectF the bounding box of the polygon
        */
        QRectF boundingRect() const override ;


        /**
        * Method to define the shape of the polygon
        * @see testMeToo()
        * @see publicVar()
        * @return
        */
        QPainterPath shape() const override;


        /**
        * Method to clear the polygon
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void clear();


        /**
        * Method to check if the polygon is empty
        * @see testMeToo()
        * @see publicVar()
        * @return Boolean. It is true if the polygon is empty and false if it is not.
        */
        bool isEmpty() const;


        /**
        * Method to add a point to the polygon
        * @param p_ a point to be added to the polygon
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void addPoint( const QPointF& p_ );


        /**
        * Method to remove a point from the polygon
        * @param p_ a point to be removed from the polygon
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void removePoint( const QPointF& p_ );


        /**
        * Set the polygon as done. This not allow add any point more to the polygon.
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setDone( bool status_ );


        /**
        * Check if the polygon is done.
        * @see testMeToo()
        * @see publicVar()
        * @return Boolean. Return true if the polygon is done, and false if not.
        */
        bool isDone() const;


        /**
        * Method to set the color of the border
        * @param red the red component of the color (integer)
        * @param green the green component of the color (integer)
        * @param blue the blue component of the color (integer)
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setBorderColor( int r_, int g_, int b_ );


        /**
        * Method to get the color of the border
        * @param red reference to the red component of the color (integer)
        * @param green reference to the green component of the color (integer)
        * @param blue reference to the blue component of the color (integer)
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getBorderColor( int& r_, int& g_, int& b_ ) const;


        /**
        * Method to set the color of the border using a QColor
        * @param color the new color of the border
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setBorderColor( const QColor& color_ );


        /**
        * Method to get the color of the border by QColor
        * @see testMeToo()
        * @see publicVar()
        * @return QColor
        */
        QColor getBorderColor() const;


        /**
        * Method to set the width of the border line
        * @param width the width of the border line
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setBorderWidth( int width_ );


        /**
        * Method to get the width of the border line
        * @see testMeToo()
        * @see publicVar()
        * @return int the width of the border line
        */
        int getBorderWidth() const;


        /**
        * Method to set the border visible or not.
        * @param status_ indicates if the border is visible or not. It is true if the border is visible and false otherwise.
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setBorderVisible( bool status_ );


        /**
        * Method to check if the border visible or not.
        * @param status_ indicates
        * @see testMeToo()
        * @see publicVar()
        * @return boolean Returns true if the border is visible and false otherwise.
        */
        bool isBorderVisible() const ;


        /**
        * Method to set if the fill color has transparency or not.
        * @param status_ indicates if the fill color has transparency or not. It is true if there is transparency in the fill color and false otherwise.
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setTransparency( bool status_ );


        /**
        * Method to check if the fill color has transparency or not.
        * @param status_ indicates if the fill color has transparency or not. It is true if there is transparency in the fill color and false otherwise.
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        bool hasTransparency() const ;


        /**
        * Method to fill the polygon with a given color
        * @param red the red component of the color (integer)
        * @param green the green component of the color (integer)
        * @param blue the blue component of the color (integer)
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setFillColor( int r_, int g_, int b_ );


        /**
        * Method to get the color of internal part of the polygon
        * @param red reference to the red component of the color (integer)
        * @param green reference to the green component of the color (integer)
        * @param blue reference to the blue component of the color (integer)
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getFillColor( int& r_, int& g_, int& b_ ) const;


        /**
        * Method to fill the polygon with a given QColor
        * @param color the new color of the curve
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setFillColor( const QColor& color_ );


        /**
        * Method to get the color of internal part of the polygon by QColor
        * @see testMeToo()
        * @see publicVar()
        * @return QColor
        */
        QColor getFillColor() const;


        /**
        * Method to reset the color of the border to default.
        * @see DEFAULT_LINE_COLOR
        * @see publicVar()
        * @return Void
        */
        void resetToDefaultBorderColor();


        /**
        * Method to reset the color of internal part of the polygon to default.
        * @see DEFAULT_LINE_COLOR
        * @see publicVar()
        * @return Void
        */
        void resetToDefaultFillColor();


        /**
        * Method to reset the width of the border to default.
        * @see DEFAULT_LINE_WIDTH
        * @see publicVar()
        * @return Void
        */
        void resetToDefaultBorderWidth();


        /**
        * This method checks if the polygon should be visible or not
        * @see testMeToo()
        * @see publicVar()
        * @return boolean. Returns true if the polygon is visible, and false otherwise.
        */
        virtual bool isVisible() const { return QGraphicsPolygonItem::isVisible(); }


        /**
        * This method checks if the polygon should be selected or not
        * @see testMeToo()
        * @see publicVar()
        * @return boolean. Returns true if the polygon is selected, and false otherwise.
        */
        virtual bool isSelected() const { return QGraphicsPolygonItem::isSelected(); }

        void setPolygon( const QPolygonF& pol_ );


    protected:



        //TODO: finish to document the parameters.
        /**
        * Method to repaint the curve
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                    QWidget* widget ) override;


        /**
        * @struct Color
        * @brief It is a data structure to represent color
        * @var red red component of a color
        * @var green green component of a color
        * @var blue blue component of a color
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        struct Color{ int red = 255, green = 0, blue = 0;
                      Color( int r_, int g_, int b_ ): red( r_ ), green( g_ ),
                                                       blue( b_ ){} };

        QPolygonF plane;                                                    /**< A data structure to hold the points of the polygon  */

        const int DEFAULT_BORDER_WIDTH = 1;                                 /**< An integer to define the default width of the border  */

        int border_width = DEFAULT_BORDER_WIDTH;                            /**< An integer to define the current width of the border  */

        Color DEFAULT_BORDER_COLOR = { 0, 200, 0 };                         /**< A Color to define the default color of the border  */

        Color DEFAULT_FILL_COLOR  = { 55, 100, 55 };                        /**< A Color to define the default color of the polygon  */

        Color border_color = DEFAULT_BORDER_COLOR;                          /**< A Color to define the current color of the border  */

        Color fill_color = DEFAULT_FILL_COLOR;                              /**< A Color to define the current color of the polygon  */

        bool is_done = true;                                                /**< A boolean to define is the polygon is done or not  */

        bool has_border = true;                                             /**< A boolean to define if the polygon has border or not  */

        bool is_transparent = false;                                        /**< A boolean to define if the polygon has transparency or not */

};

#endif // POLYGONITEM_H
