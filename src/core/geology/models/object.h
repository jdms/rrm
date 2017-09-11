#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <map>
#include <tuple>

#include <Eigen/Dense>

#include "PolygonalCurve/CurveN.hpp"
//#include "./core/Geometry/PolygonalCurve/polygonal_curve_2d.hpp"


class Object
{
    public:

        enum class Type { STRATIGRAPHY, NONE };

        Object();
        Object( const Object::Type& t );


        void setId( std::size_t id );
        std::size_t getId() const;

        void setType( const Object::Type& t );
        Object::Type getType() const ;

        void setName( const std::string& n );
        std::string getName() const;


        void setColor( int r, int g, int b );
        void getColor( int& r, int& g, int& b ) const ;


        void setEditable( bool status );
        bool getEditable() const ;

        void setSelectable( bool status );
        bool getSelectable() const ;


        void setSelected( bool status );
        bool getSelected() const ;


        void setVisibility( bool status );
        bool getVisibility() const ;


        bool isEmpty() const ;

        void setTesting( bool status );
        bool isTesting() const;


        void setCrossSectionCurve( double depth, const Curve2D& curve,
                                   std::vector< std::size_t > edges = std::vector< std::size_t >() );
        Curve2D getCrossSectionCurve( double depth ) ;
        std::vector< std::size_t > getCrossSectionCurveEdges( double depth ) ;

        std::vector< std::tuple< Curve2D, double > > getCrossSectionCurves();

        void removeCrossSectionCurve( double depth );
        void removeCrossSections();


        bool isCurveAdmissible();
        bool isTrajectoryAdmissible();


        bool hasCurve( double depth );

        bool hasTrajectoryCurve();
        void setTrajectoryCurve( const Curve2D& path );
        Curve2D getTrajectoryCurve() const ;
        void removeTrajectoryCurve();


        void setSurface( const std::vector< double >& vertices,
                         const std::vector< std::size_t >& faces, bool test = false );


        void setSurfaceNormals( const std::vector< double >& normals );
        std::vector< double > computeNormals();
        void removeSurface();


        std::vector< double > getSurfaceVertices() const ;
        std::vector< std::size_t > getSurfaceFaces() const ;
        std::vector< double > getSurfaceNormals();

        void clear();
        void setDefaultValues();




    public:

        static std::size_t count_objects;


    private:

        std::size_t index;
        std::string name;
        std::tuple< int, int, int > color;
        Object::Type type;

        bool is_selectable;
        bool is_selected;
        bool is_visible;
        bool is_editable;
        bool is_test;


        bool has_trajectory;
        std::map< double, Curve2D > csections_curves;
        std::map< double, std::vector< std::size_t > > csections_edges;
        Curve2D trajectory_curve;


        std::vector< double > surface_vertices;
        std::vector< std::size_t > surface_faces;
        std::vector< double > surface_normals;





};

#endif // OBJECT_H