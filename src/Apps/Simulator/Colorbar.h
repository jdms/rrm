#ifndef COLORBAR_H
#define COLORBAR_H

#include <QWidget>
#include <QPainter>
#include <QVector3D>
#include <iostream>

class ColorBar: public QWidget
{
	Q_OBJECT

    public:

        ColorBar();
        void setSize( const int& l, const int& w );
        void updateColorMap(const std::vector < QVector3D >& c, float _min, float _max, int label_step = 10);
        void updateColorMap( const std::vector < QVector3D >& c, const std::vector< int >& ids_, const std::vector< double >& values_ );
        void clear();

    protected:

        void paintEvent(QPaintEvent*);

        std::vector< QVector3D > colors;
        std::vector< int > ids;
        std::vector< double > values;

	private:
        int x;
        int y;
        int width;
        int length;
        float min;
        float max;
		float label_step;

        bool is_gradient;
};


#endif // COLORBAR_H
