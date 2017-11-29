#ifndef INPUTSKETCH_H
#define INPUTSKETCH_H

#include <QGraphicsPathItem>

#include "./core/base/constants/constants.hpp"
#include "./libs/PolygonalCurve/CurveN.hpp"
#include "./core/base/models/polycurve.h"

class SketchLibrary
{

    public:


        static Curve2D xmonotonicSketch( const Curve2D& sketch_ )
        {

            if( sketch_.size() < SKETCH_MIN_LENGHT ) return sketch_;

            Curve2D ms_;

            int start = 0;

            ms_.add( sketch_[ start ] );

            for( std::size_t i = start; i < sketch_.size() - 1; ++i )
            {
                if( sketch_[ i ].x() != sketch_[ i + 1 ].x() ) break;

                ms_.add( sketch_[ i + 1 ] );
                start = i+1;
            }

            if( sketch_[ start ].x() <= sketch_[ start + 1 ].x() )
            {
                std::size_t temp = start;
                for( std::size_t i = start; i < sketch_.size() - 1; ++i )
                {
                    if( sketch_[ temp ].x() >= sketch_[ i + 1 ].x() ) continue;

                    ms_.add( sketch_[ i + 1 ] );
                    temp = i + 1;
                }

            }

            else if( sketch_[ start ].x() >= sketch_[ start + 1 ].x() )
            {
                std::size_t temp = start;
                for( std::size_t i = start; i < sketch_.size() - 1; ++i )
                {
                    if( sketch_[ temp ].x() <= sketch_[ i + 1 ].x() ) continue;
                    ms_.add( sketch_[ i + 1 ] );
                    temp = i + 1;
                }

            }

            return ms_;

        }

        static Curve2D ymonotonicSketch( const Curve2D& sketch_ )
        {

            if( sketch_.size() < SKETCH_MIN_LENGHT ) return sketch_;

            Curve2D ms_;

            int start = 0;

            ms_.add( sketch_[ start ] );

            for( std::size_t i = start; i < sketch_.size() - 1; ++i )
            {
                if( sketch_[ i ].y() != sketch_[ i + 1 ].y() ) break;

                ms_.add( sketch_[ i + 1 ] );
                start = i+1;
            }

            if( sketch_[ start ].y() <= sketch_[ start + 1 ].y() )
            {
                std::size_t temp = start;
                for( std::size_t i = start; i < sketch_.size() - 1; ++i )
                {
                    if( sketch_[ temp ].y() >= sketch_[ i + 1 ].y() ) continue;

                    ms_.add( sketch_[ i + 1 ] );
                    temp = i + 1;
                }

            }

            else if( sketch_[ start ].y() >= sketch_[ start + 1 ].y() )
            {
                std::size_t temp = start;
                for( std::size_t i = start; i < sketch_.size() - 1; ++i )
                {
                    if( sketch_[ temp ].y() <= sketch_[ i + 1 ].y() ) continue;
                    ms_.add( sketch_[ i + 1 ] );
                    temp = i + 1;
                }

            }

            return ms_;

        }

        static bool overSketch( Curve2D& sketch_, const Curve2D& subpath_ )
        {
            if( subpath_.size() < SKETCH_MIN_LENGHT ) return false;

            Curve2D cpy_subpath_ = subpath_;
            Curve2D rest_;

            double err_ = 1.0;
            double aprox_factor_ = 36.0;

            sketch_ = cpy_subpath_.overSketch( sketch_, rest_, err_, aprox_factor_ );
            return true;

        }

        static Curve2D smooth( Curve2D& sketch_ )
        {
            Curve2D cpy_ = sketch_;
            cpy_.lineFilter();
            cpy_.meanFilter();
            cpy_.meanFilter();
            return cpy_;
        }

        static void fromCurve2DToVector( const Curve2D& curve_, std::vector< double >& vertices_,
                                         std::vector< std::size_t >& edges_ )
        {
            if( curve_.isEmpty() == true ) return;

            std::size_t index_ = 0;

            for( std::size_t i = 0; i < curve_.size(); ++i )
            {
                const Point2D& p_ = curve_.at( i );

                vertices_.push_back( p_.x() );
                vertices_.push_back( p_.y() );

                edges_.push_back( index_ );
                edges_.push_back( ++index_ );
            }
        }

        static PolyCurve fromCurve2DToPolyCurve( const Curve2D& curve_ )
        {
            std::vector< double > vertices_;
            std::vector< std::size_t > edges_;

            fromCurve2DToVector( curve_, vertices_, edges_ );
            return PolyCurve( vertices_, edges_ );

        }

    public:

        static const std::size_t SKETCH_MIN_LENGHT = 7;

};


class QPainter;


class InputSketch: public QGraphicsPathItem
{

    public:

        enum class Direction{ X, Y, Z };

        InputSketch( QGraphicsItem * parent = 0 );
        virtual QRectF boundingRect() const;

        void create( const QPointF& p_ );
        void add( const QPointF& p_ );
        void process();
        PolyCurve done( const InputSketch::Direction& dir_ );
        void clear();
        bool isEmpty() const;

        void setCurrentColor( const QColor& color_ );
        QColor getCurrentColor() const;

        void setCurrentWidth( int width_ );
        int getCurrentWidth() const;

        void setSketch( const QPolygonF& sketch_ );
        QPolygonF getSketch() const;

        bool getSubpaths( QPolygonF& path0_, QPolygonF& path1_ ) const;
        Curve2D overSketch( const QPolygonF& path0_, const QPolygonF& path1_ );


        static void fromPainterPathToVector( const QPainterPath& curve_,
                                             std::vector< double >& vertices_,
                                             std::vector< std::size_t >& edges_ );

        static Curve2D fromQtToCurve2D( const QPolygonF& pol_ );
        static QPolygonF fromCurve2DToQt( const Curve2D& curve_ );

        static QPointF Point2DToQPointF( const Point2D & p );
        static Point2D QPointFToPoint2D( const QPointF &p );



    protected:

        virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                            QWidget* widget = 0 );


    private:


        const int MIN_SUBPATHS_OVERSKETCH = 2;

        int current_width;
        QColor current_color;

        QPainterPath curve;


};


#endif // INPUTSKETCH_H
