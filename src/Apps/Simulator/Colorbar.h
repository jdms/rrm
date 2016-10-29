#ifndef COLORBAR_H
#define COLORBAR_H

#include <QWidget>
#include <QPainter>
#include <QVector3D>

class ColorBar: public QWidget
{
    public:

        ColorBar();
        void setSize( const int& l, const int& w );
        void updateColorMap( const std::vector < QVector3D >& c/*, const std::vector< double >& v*/, float m, float M );
        void clear();

    protected:

        void paintEvent(QPaintEvent*);

        std::vector< QVector3D > colors;
        std::vector< double > values;

        int x;
        int y;
        int width;
        int length;
        float min;
        float max;
};


#endif // COLORBAR_H
