#ifndef TRAJECTORYITEMWRAPPER_H
#define TRAJECTORYITEMWRAPPER_H

#include <set>
#include <vector>

#include <QGraphicsPathItem>
#include <QPainter>

class Object_Refactored;

class TrajectoryItemWrapper: public QGraphicsPathItem
{
    public:

        TrajectoryItemWrapper();


        QRectF boundingRect() const;
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w );

        void setRawObject( Object_Refactored* const& obj );
        std::size_t getId() const;

        void updateState();
        void updateCurve();
        void updateObject();

        bool isVisible() const;
        void enableEditing();

        void clear();
        void clearData();


    protected:

        void setupPen();
        void clearCurve();



    private:

        Object_Refactored* object;

        QPainterPath curve;
        const QColor selectable_color =  Qt::yellow;
        QColor selected_color = Qt::red;

        std::set<QPointF> intersection_points;
        std::vector<bool> hidden_subpaths;
        QPen hidden_subpaths_pen;
        QPen visible_subpaths_pen;




};

#endif // TRAJECTORYITEMWRAPPER_H
