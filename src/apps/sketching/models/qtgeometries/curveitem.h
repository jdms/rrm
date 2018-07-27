#ifndef CURVEITEM_H
#define CURVEITEM_H

#include <limits>
#include <QGraphicsPathItem>

#include "./libs/sketchlibrary.h"


class SketchLibraryWrapper
{
    public:

    static QPainterPath overSketchQt( const QPolygonF& seg0_, const QPolygonF& seg1_ )
    {
        Curve2D sketch_ = fromQtToCurve2D( seg0_ );
        Curve2D segment_ = fromQtToCurve2D( seg1_ );

        SketchLibrary1::overSketch( sketch_, segment_ );

        QPainterPath curve;
        curve.addPolygon( fromCurve2DToQt( sketch_ ) );
        return curve;

    }

    static Curve2D fromQtToCurve2D( const QPolygonF& pol_ )
    {
        Curve2D curve_;
        std::size_t start = ( pol_.isClosed() ) ? 1 : 0 ;

        for( int i = start ; i < pol_.size() ; ++i )
            curve_.add( QPointFToPoint2D( pol_[ i ] ) );

        curve_.close( pol_.isClosed() );
        return curve_;

    }

    static QPolygonF fromCurve2DToQt( const Curve2D& curve_ )
    {

        QPolygonF pol_;

        for( std::size_t i = 0 ; i < curve_.size() ; ++i )
            pol_ << Point2DToQPointF( curve_[ i ] ) ;

        if( curve_.isClosed() && curve_.size() > 0 )
            pol_ << Point2DToQPointF( curve_[ 0 ] ) ;

        return pol_;
    }

    static QPointF Point2DToQPointF( const Point2D & p )
    {
        return QPointF( p.x() , p.y() ) ;
    }

    static Point2D QPointFToPoint2D( const QPointF &p )
    {
        return Point2D( p.x() , p.y() ) ;
    }

};


class CurveItem : public QGraphicsPathItem
{
    public:

        CurveItem( QGraphicsItem *parent_ = nullptr );
        ~CurveItem();

        QRectF boundingRect() const override ;

        bool create( const QPointF& p_ );
        bool add( const QPointF& p_ );
        void connect();

        void setDone();
        bool isDone() const;

        void clear();
        bool isEmpty() const;

        void setColor( int r_, int g_, int b_ );
        void getColor( int& r_, int& g_, int& b_ );

        void setColor( const QColor& color_ );
        QColor getColor() const;

        void setWidth( int width_ );
        int getWidth() const;

        void setStyle( const Qt::PenStyle& style_ );
        Qt::PenStyle getStyle() const;

        void resetToDefaultColor();
        void resetToDefaultWidth();
        void resetToDefaultStyle();

        void setCurve( const PolyCurve& curve_ );
        PolyCurve getCurve() const;

        virtual void update(){}


    protected:


        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                            QWidget* widget ) override;


        bool hasSubpaths();


    protected:


        struct Color{ int red = 255, green = 0, blue = 0; };

        QPainterPath curve;

        const Color DEFAULT_LINE_COLOR;
        Color line_color = DEFAULT_LINE_COLOR;

        const int DEFAULT_LINE_WIDTH = 2;
        int line_width = DEFAULT_LINE_WIDTH;

        const Qt::PenStyle DEFAULT_LINE_STYLE = Qt::SolidLine;
        Qt::PenStyle line_style = DEFAULT_LINE_STYLE;

        bool is_done = true;


};


#endif // CURVEITEM_H
