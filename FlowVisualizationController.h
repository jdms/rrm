#ifndef FLOWVISUALIZATIONCONTROLLER_H
#define FLOWVISUALIZATIONCONTROLLER_H

#include <vector>

#include <QString>

using namespace std;

class FlowVisualizationController
{

    public:

        FlowVisualizationController();


        void setVertices( vector< float > vertices );
        void getVertices(vector< float >& vertices ) const;

        void setTriangles( vector< unsigned int > triangles );
        void getTriangles( vector< unsigned int >& triangles ) const;

        void setColors( vector< float > colors );
        void getColors( vector< float >& colors ) const;

        void setColorMap();
        void getColorMap() const;

        void setProperties( QString property, int dim, vector< float > values );
        void getProperties( QString& property, int& dim, vector< float >& values ) const;



};

#endif // FLOWVISUALIZATIONCONTROLLER_H
