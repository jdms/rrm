#ifndef FLOWVISUALIZATIONCONTROLLER_H
#define FLOWVISUALIZATIONCONTROLLER_H

//#include "Model/CrossSection.hpp"

#include <vector>
#include <random>

#include <QWidget>
#include <QColor>
#include <QFileDialog>

#include "Mesh.h"
#include "ProgressCounter.h"
#include "FlowDiagnosticsInterface.hpp"

#include <Eigen/Dense>

class FlowVisualizationController: public QWidget
{
    Q_OBJECT

    public:

        typedef qreal		 	 	Real;
//        typedef RRM::CrossSection<Real>  	CrossSection;


        enum class  MESHING_METHOD{ CORNERPOINT, UNSTRUCTURED };


        FlowVisualizationController( QWidget* parent = 0 );



        void readSkeletonFiles( const std::string& filename );
        void setSkeletonData( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );

        void buildSurfaceSkeleton( std::vector< double >& positions, std::vector< unsigned int >& faces );

        void readInputParameters( const std::string& input_file );

        void generateCornerPoint();
        void generateUnstructured();

        void updateSurfaces( std::vector< float >& vertices, std::vector< unsigned int >& faces );
        void updateCornerPoint( std::vector< float >& vertices, std::vector< unsigned int >& edges, std::vector< unsigned int >& faces );
        void updateVolumetricMesh( std::vector< float >& vertices, std::vector< unsigned int >& edges, std::vector< unsigned int >& faces );
        void getSurfacesFromCrossSection(/*const CrossSection &_cross_section*/);

        void computeFlowProperties();

        std::vector< double > getVerticesPropertyValues( std::string name_of_property, std::string method, double& min, double& max );
        std::vector< double > getFacesPropertyValues( std::string name_of_property, std::string method, double& min, double& max );
        std::vector< double > vectorToScalarProperties(const std::vector<double> &values, std::string type, double& min , double& max  );

        void setPropertyArea( const int np, const std::vector< double >& values , const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc );
        void getPropertyArea( int& np, std::vector< double >& values , std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc );

        void setWellsValues( const int nw,  const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign );
        void getWellsValues( int& nw,  std::vector< unsigned int >& type, std::vector< double >& value, std::vector< int >& sign );


        void clear();


        bool isVolumetricOk(){ return volumetric_ok; }
        bool arePropertiesComputed() { return properties_computed; }

        void getRegionsColor( std::vector< QColor >& color_by_cells );


    public slots:


        inline void setCurrentMethod( const FlowVisualizationController::MESHING_METHOD& t ){ current_method = t; }
        inline  FlowVisualizationController::MESHING_METHOD setCurrentMethod(){ return current_method; }


        void exportSurfacetoVTK();
        void exportVolumetoVTK();
        void exportCornerPointtoVTK();
        void exportCornerPointtoGRDECL();
        void exportResultstoVTK();


		void setScene2Dto3D(const Eigen::Affine3f& m);
		void setScene3Dto2D(const Eigen::Affine3f& m);

		Eigen::Vector3f scene2Dto3D(const Eigen::Vector2f& p);
		Eigen::Vector3f scene2Dto3D(const Eigen::Vector3f& p);
		Eigen::Vector3f scene3Dto2D(const Eigen::Vector3f& p);



    signals:

        void showToolbar( bool );

        void propertybyVertexComputed( std::string, std::string );
        void propertybyFaceComputed( std::string, std::string );

        void setColorMap();

        void clearPropertiesMenu();
        void clearAll();



    private:


        ProgressCounter counter;

        bool mesh_ok;
        bool volumetric_ok;
        bool user_input_ok;
        bool properties_computed;

        MESHING_METHOD current_method;
        FlowDiagnosticsInterface code_interface;

		Eigen::Affine3f m_2dto3d;
		Eigen::Affine3f m_3dto2d;

};

#endif // FLOWVISUALIZATIONCONTROLLER_H

