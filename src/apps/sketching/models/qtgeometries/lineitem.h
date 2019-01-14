#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsLineItem>


/**
 *  A graphical representation to a line in the QGraphicsScene.
 *  It inherits from QGraphicsLineItem.
 */


class LineItem: public QGraphicsLineItem
{
    public:


        /**
        * Constructor.
        */
        LineItem( QGraphicsItem *parent_ = nullptr);


        /**
        * Destructor.
        */
        ~LineItem() override;


        /**
        * Method to get the bounding box of the line
        * @see testMeToo()
        * @see publicVar()
        * @return QRectF the bounding box of the line
        */
        QRectF boundingRect() const override ;


        /**
        * This method clears the line
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void clear();


        /**
        * Method to check if the line is empty
        * @see testMeToo()
        * @see publicVar()
        * @return Boolean. It is true if the QLineF is empty and false if it is not.
        */
        bool isEmpty() const;


        /**
        * Method to set the color of the line
        * @param red the red component of the color (integer)
        * @param green the green component of the color (integer)
        * @param blue the blue component of the color (integer)
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setLineColor( int r_, int g_, int b_ );


        /**
        * Method to get the color of the line
        * @param red reference to the red component of the color (integer)
        * @param green reference to the green component of the color (integer)
        * @param blue reference to the blue component of the color (integer)
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getLineColor( int& r_, int& g_, int& b_ ) const;


        /**
        * Method to set the color of the line using a QColor
        * @param color the new color of the line
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setLineColor( QColor color_ );


        /**
        * Method to get the color of the line by QColor
        * @see testMeToo()
        * @see publicVar()
        * @return QColor
        */
        QColor getLineColor() const;


        /**
        * Method to set the width of the line
        * @param width the width of the line
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setLineWidth( int width_ );


        /**
        * Method to get the width of the line
        * @see testMeToo()
        * @see publicVar()
        * @return int the width of the line
        */
        int getLineWidth() const;


        /**
        * This method checks if the curve should be visible or not
        * @see testMeToo()
        * @see publicVar()
        * @return boolean. Returns true if the line is visible, and false otherwise.
        */
        virtual bool isVisible() const { return QGraphicsLineItem::isVisible(); }


        /**
        * Method to reset the color of the line to default.
        * @see DEFAULT_LINE_COLOR
        * @see publicVar()
        * @return Void
        */
        void resetToDefaultLineColor();


    protected:


        //TODO: finish to document the parameters.
        /**
        * Method to repaint the curve
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void paint( QPainter* painter_, const QStyleOptionGraphicsItem* option_,
                    QWidget* widget_ ) override;


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

        QLineF line;                                                            /**< A data structure to represent the line  */

        Color DEFAULT_LINE_COLOR = { 0, 255, 255 };                             /**< A Color to define the default color of the line */

        Color line_color = DEFAULT_LINE_COLOR;                                  /**< A Color to define the current color of the line */

        const int DEFAULT_LINE_WIDTH = 2;                                       /**< An integer to define the default width of the line  */

        int line_width = DEFAULT_LINE_WIDTH;                                    /**< An integer to define the current width of the line  */


};

#endif // LINEITEM_H
