#ifndef OBJECTITEMWRAPPER_H
#define OBJECTITEMWRAPPER_H

#include <set>
#include <vector>

#include <QGraphicsPathItem>
#include <QPainter>

class Object;

class ObjectItemWrapper: public QGraphicsPathItem
{
    public:

        ObjectItemWrapper();


        QRectF boundingRect() const;
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w );

        void setRawObject( Object* const& obj, double depth );
        std::size_t getId() const;

        void updateDepth( double depth );

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

        Object* object;

        double csection_depth;
        QPainterPath curve;
        const QColor selectable_color =  Qt::yellow;
        QColor selected_color = Qt::red;

        std::set<QPointF> intersection_points;
        std::vector<bool> hidden_subpaths;
        QPen hidden_subpaths_pen;
        QPen visible_subpaths_pen;

};

#endif // OBJECTITEMWRAPPER_H
