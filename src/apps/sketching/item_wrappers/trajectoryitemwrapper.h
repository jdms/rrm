#ifndef TRAJECTORYITEMWRAPPER_H
#define TRAJECTORYITEMWRAPPER_H


#include <QGraphicsPathItem>
#include <QColor>
#include <QPainter>


#include "./core/base/models/object.h"


class TrajectoryItemWrapper: public QGraphicsPathItem
{
    public:



    TrajectoryItemWrapper() = default;
    TrajectoryItemWrapper( Object* const& obj_ );


    void setRawObject( Object* const& obj_ );
    Object* getRawObject() const ;

    std::size_t getIndex() const;

    void updateState();
    void updateCurve();
    void updateTrajectory();
    void updateDepth( double depth_ );


    void setActive( bool status_ ){}
    bool isActive() const{}


    bool isVisible() const;
    bool isEditable() const;


    QRectF boundingRect() const;


    void clear();
    void clearData();
    void clearCurve();


    static QPainterPath polycurveToQPainterPath( const PolyCurve& pol_ );
    static QPolygonF vectorToQPolygonF( const std::vector< double >& vertices_ );
    static QPainterPath vectorToPainterPath( const std::vector< double >& vertices_,
                                             const std::vector< std::size_t >& edges_ );



protected:


    void setupPen();
    void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );



private:

    Object* raw;
    QPainterPath curve;
    QPen current_pen;

    double current_csection;


};

#endif // TRAJECTORYITEMWRAPPER_H
