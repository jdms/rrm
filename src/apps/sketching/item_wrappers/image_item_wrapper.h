#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QString>

class ImageItemWrapper: public QGraphicsPixmapItem
{

    enum class Interaction{ MOVING, RESIZING, NONE };

    public:

        ImageItemWrapper();

        void setImage( const QPixmap& im, QPointF origin_, QPointF top_right_ );

        void setImagePath( const QString& file_ );
        QString getImagePath() const;

        QPointF getOrigin() const;
        QPointF getTopRight() const;

        void moveRectangle( QPointF origin_ );
        void resizeRectangle(const QPointF &p );


        QRectF boundingRect() const;


    protected:

        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                        QWidget* widget = 0 );





    private:

        QRectF m_imageRect;
        QPixmap m_image;
        QString path;

        Interaction default_action = Interaction::NONE;

};

#endif // ITEM_H
