#ifndef OBJECT_REFACTORED_H
#define OBJECT_REFACTORED_H

#include <vector>
#include <string>
#include <map>
#include <tuple>

class Curve2D;

class Object_Refactored
{
    public:

        enum class Type { STRATIGRAPHY };

        Object_Refactored();


        std::size_t getId() const{}

        void setType( const Object_Refactored::Type& t );
        Object_Refactored::Type getType() const ;

        void setName( const std::string& n );
        std::string getName() const ;


        void setColor( int r, int g, int b );
        void getColor( int& r, int& g, int& b ) const ;


        void setSelectable( bool status );
        bool getSelectable() const ;


        void setSelected( bool status );
        bool getSelected() const ;


        void setVisibility( bool status );
        bool getVisibility() const ;


        bool isEmpty() const { return true; }


        void setCrossSectionCurve( double depth, Curve2D* const& curve,
                                   std::vector< std::size_t > edges = std::vector< std::size_t >() );
        void getCrossSectionCurve( double depth ) const ;
        void getCrossSectionCurveEdges( double depth ) const ;
        void removeCrossSectionCurve( double depth );
        void removeCrossSections();



        bool hasTrajectoryCurve();
        void setTrajectoryCurve( Curve2D* const& path );
        Curve2D* getTrajectoryCurve() const ;
        void removeTrajectoryCurve();


        void setSurface( const std::vector< double >& vertices,
                         const std::vector< std::size_t >& faces );
        void setSurfaceNormals( const std::vector< double >& normals );
        void removeSurface();



        void clear(){}
        void setDefaultValues(){}







    private:

        std::size_t index;
        std::string name;
        std::tuple< int, int, int > color;
        Object_Refactored::Type type;

        bool is_selectable;
        bool is_selected;
        bool is_visible;


        bool has_trajectory;
        std::map< double, Curve2D* > csections_curves;
        std::map< double, std::vector< std::size_t > > csections_edges;
        Curve2D* trajectory_curve;


        std::vector< double > surface_vertices;
        std::vector< std::size_t > surface_faces;
        std::vector< double > surface_normals;


        static std::size_t count_objects;


};

#endif // OBJECT_REFACTORED_H
