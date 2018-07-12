#ifndef PLANEITEM_H
#define PLANEITEM_H

#include <QGraphicsPolygonItem>

class PlaneItem: public QGraphicsRectItem
{
    public:

        PlaneItem( QGraphicsItem *parent_ = nullptr );
        ~PlaneItem();

        QRectF boundingRect() const override ;

        void setStartPoint( const QPointF& start_ );
        void setEndPoint( const QPointF& end_ );

        void resize( const QPointF& dimensions_, const QPointF& origin_ = QPointF( 0.0f, 0.0f ) );
        void resize( double width_, double height_, double ox_ = 0.0, double oy_ = 0.0 );

        void clear();
        bool isEmpty() const;

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


    protected:

        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                    QWidget* widget ) override;


        struct Color{ int red = 255, green = 0, blue = 0;
                      Color( int r_, int g_, int b_ ): red( r_ ), green( g_ ),
                                                       blue( b_ ){} };

        QRectF plane;

        const int DEFAULT_BORDER_WIDTH = 1;
        int border_width = DEFAULT_BORDER_WIDTH;

        Color DEFAULT_BORDER_COLOR = { 0, 200, 0 };
        Color DEFAULT_FILL_COLOR  = { 55, 100, 55 };

        Color border_color = DEFAULT_BORDER_COLOR;
        Color fill_color = DEFAULT_FILL_COLOR;

};

#endif // PLANEITEM_H
