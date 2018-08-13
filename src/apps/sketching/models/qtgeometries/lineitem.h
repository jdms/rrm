#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsLineItem>

class LineItem: public QGraphicsLineItem
{
    public:

        LineItem( QGraphicsItem *parent_ = nullptr);
        ~LineItem();

        QRectF boundingRect() const override ;

        void clear();
        bool isEmpty() const;

        void setLineColor( int r_, int g_, int b_ );
        void getLineColor( int& r_, int& g_, int& b_ ) const;

        void setLineColor( QColor color_ );
        QColor getLineColor() const;

        void setLineWidth( int width_ );
        int getLineWidth() const;

        void resetToDefaultLineColor();

        virtual bool isVisible() const { return QGraphicsLineItem::isVisible(); }

    protected:

        void paint( QPainter* painter_, const QStyleOptionGraphicsItem* option_,
                    QWidget* widget_ ) override;


        struct Color{ int red = 255, green = 0, blue = 0;
                      Color( int r_, int g_, int b_ ): red( r_ ), green( g_ ),
                                                       blue( b_ ){} };

        QLineF line;

        Color DEFAULT_LINE_COLOR = { 0, 255, 255 };
        Color line_color = DEFAULT_LINE_COLOR;

        const int DEFAULT_LINE_WIDTH = 2;
        int line_width = DEFAULT_LINE_WIDTH;

};

#endif // LINEITEM_H
