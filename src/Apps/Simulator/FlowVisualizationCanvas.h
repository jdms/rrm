/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef FLOWVISUALIZATIONCANVAS_H
#define FLOWVISUALIZATIONCANVAS_H


#include <GL/glew.h>
#include <iostream>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QFileDialog>


#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"
//#include "Tucano/BoundingBox3.hpp"

#include "FlowVisualizationController.h"
#include "ColorMap.h"
#include "CoordinateAxes.h"
#include "FlowCrossSection.h"
//#include "PointMarkers.h"


class FlowVisualizationCanvas: public QOpenGLWidget
{
    Q_OBJECT

    public:

		FlowVisualizationCanvas(QWidget *parent, QString _current_dir);
        ~FlowVisualizationCanvas();
       
        void setController( FlowVisualizationController *c );      
        void clear();

    public slots:

        void updateMesh();
        void updateVolumetricMesh();
        void updateCornerPoint();
		
        void showBoundingBox( bool status );
        
        void disableCrossSection( bool status );
        void setCrossSectionNormalCoordinates( float X, float Y, float Z );

        inline void setCurrentDirectory( std::string current_dir ){ current_directory.clear(); current_directory = current_dir; }

        void setColors( const std::vector< float >& colors );
		void setDefaultColor();

        void showRegions();

		/// Debug Purpose
		void reloadShader();

		float getDepth() const;

		void updateWellsPosition(int _number_of_wells,const std::map<int, Eigen::Vector4d>& _positions, const std::map<int, int>& _types);

    signals:
        void applyCrossSection();
        void clearAll();


    protected:

        void initializeShader();
        void loadBackGround();

        void initializeGL() Q_DECL_OVERRIDE;
        void paintGL() Q_DECL_OVERRIDE;
		void resizeGL(int width, int height) Q_DECL_OVERRIDE;

		void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

		void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;


    private:

        FlowVisualizationController* controller ;

        QSurfaceFormat format;
        std::string current_directory;

        Tucano::Trackball camera;
        const float speed_zoom = 0.1f;

        Mesh mesh;

		std::map<int, Eigen::Vector4d> wells_positions_;
       
        bool show_axis;
//        CoordinateAxes axes;

        float scale;

        bool apply_crosssection;
        FlowCrossSection crosssection;

        Tucano::Shader*   background;
        GLuint va_background;
        GLuint vb_background;


        ColorMap::COLORMAP current_colormap;
        bool show_colorbar;

};

#endif // FLOWVISUALIZATIONCANVAS_H

