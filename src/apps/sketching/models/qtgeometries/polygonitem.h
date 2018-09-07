#ifndef POLYGONITEM_H
#define POLYGONITEM_H

#include <QGraphicsPolygonItem>
#include<QGraphicsSceneMouseEvent>

class PolygonItem: public QGraphicsPolygonItem
{

    public:

        PolygonItem( QGraphicsItem *parent_ = nullptr );
        ~PolygonItem();

        QRectF boundingRect() const override ;
        QPainterPath shape() const;

        void clear();
        bool isEmpty() const;

        void addPoint( const QPointF& p_ );
        void removePoint( const QPointF& p_ );

        void setDone( bool status_ );
        bool isDone() const;

        void setBorderColor( int r_, int g_, int b_ );
        void getBorderColor( int& r_, int& g_, int& b_ ) const;

        void setBorderColor( const QColor& color_ );
        QColor getBorderColor() const;

        void setBorderWidth( int width_ );
        int getBorderWidth() const;

        void setFillColor( int r_, int g_, int b_ );
        void getFillColor( int& r_, int& g_, int& b_ ) const;

        void setFillColor( const QColor& color_ );
        QColor getFillColor() const;

        void resetToDefaultBorderColor();
        void resetToDefaultFillColor();
        void resetToDefaultBorderWidth();

        virtual bool isVisible() const { return QGraphicsPolygonItem::isVisible(); }

        void setPolygon( const QPolygonF& pol_ );


    protected:

        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                    QWidget* widget ) override;



        struct Color{ int red = 255, green = 0, blue = 0;
                      Color( int r_, int g_, int b_ ): red( r_ ), green( g_ ),
                                                       blue( b_ ){} };

        QPolygonF plane;

        const int DEFAULT_BORDER_WIDTH = 1;
        int border_width = DEFAULT_BORDER_WIDTH;

        Color DEFAULT_BORDER_COLOR = { 0, 200, 0 };
        Color DEFAULT_FILL_COLOR  = { 55, 100, 55 };

        Color border_color = DEFAULT_BORDER_COLOR;
        Color fill_color = DEFAULT_FILL_COLOR;

        bool is_done = true;

};

#endif // POLYGONITEM_H
