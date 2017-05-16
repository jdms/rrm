#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include <tuple>

#include "Core/Geometry/PolygonalCurve/PolygonalCurve2D.hpp"

//TODO: clear whatever variable before performing the set

class Object
{

    public:


        enum class TYPE{ Stratigraphy, Channel, None };

        Object()
        {
            initialize();
        }

        Object( Object::TYPE type_ )
        {
            initialize();
            setType( type_ );
        }


        inline std::size_t getId() const
        {
            return id;
        }

        inline void setType( const Object::TYPE _type )
        {
            type = _type;
        }

        inline Object::TYPE getType() const
        {
            return type;
        }


        inline void setName( const std::string& _name )
        {
            name = _name;
        }

        inline std::string getName() const
        {
            return name;
        }

        inline void setVisibility( bool option )
        {
            is_visible = option;
        }

        inline bool getVisibility() const
        {
            return is_visible;
        }

        inline void setColor( int red_, int green_, int blue_ )
        {
            color.red = red_;
            color.green = green_;
            color.blue = blue_;
        }


        inline void getColor( int& red_, int& green_, int& blue_ )
        {
            red_   = color.red ;
            green_ = color.green;
            blue_  = color.blue ;
        }


        inline void addInputCurve( double _depth, Curve2D const& _curve )
        {

            if( ( type == Object::TYPE::Channel ) && ( input_curves.size() > 1 ) )
                input_curves.clear();

            input_curves[ _depth ] = _curve;

        }

        inline void addInputEdges( double _depth, const std::vector< std::size_t >& edges_ )
        {
            if( ( type == Object::TYPE::Channel ) && ( input_edges.size() > 1 ) )
                input_edges.clear();

            input_edges[ _depth ] = edges_;
        }

        inline Curve2D& getInputCurveofCrossSection( double _depth )
        {
            return input_curves[ _depth ];
        }

        inline std::vector< std::size_t > getInputEdgesofCrossSection( double _depth )
        {
            return input_edges[ _depth ];
        }

        inline std::vector< double > getAllCrossSectionsRelatedtoObject() const
        {
            std::vector< double > depth_cross_sections_;
            for( auto const &it : input_curves ) {
                depth_cross_sections_.push_back( it.first );
            }
            return depth_cross_sections_;
        }

        inline std::vector< Curve2D > getAllInputCurves() const
        {
            std::vector< Curve2D > input_curves_;
            for( auto const &it : input_curves) {
                input_curves_.push_back( it.second );
            }
            return input_curves_;
        }


        inline std::vector< std::vector< std::size_t > > getAllInputEdges()
        {
            std::vector< std::vector< std::size_t > > input_edges_;
            for( auto const &it : input_edges ) {
                input_edges_.push_back( it.second );
            }
            return input_edges_;
        }

        inline std::vector< std::tuple< Curve2D, double > > getAllCurves() const
        {
            std::vector< std::tuple< Curve2D, double > > curves_;
            for( auto const &it : input_curves) {
                curves_.push_back( std::make_tuple( it.second, (std::size_t)it.first ) );
            }

            return curves_;
        }

        inline std::size_t getNumberofInputCurves() const
        {
            return input_curves.size();
        }


        inline void setPathCurve( const Curve2D& _path )
        {
            path_curve = _path;
        }

        inline const Curve2D& getPathCurve() const
        {
            return path_curve;
        }

        inline bool hasPathCurve(){ return ( !path_curve.isEmpty() );  }

        inline void updateSurface( const std::vector< double >& vertices_,
                                   const std::vector< std::size_t > faces_ )
        {
            surface_vertices.clear();
            surface_faces.clear();

            surface_vertices.assign( vertices_.begin(), vertices_.end() );
            surface_faces.assign( faces_.begin(), faces_.end() );
        }


        inline const std::vector< double >& getSurfaceVertices()
        {
            return surface_vertices;
        }

        inline const std::vector< std::size_t >& getSurfaceFaces()
        {
            return surface_faces;
        }


        inline void setDefaultValues()
        {
            type = TYPE::Stratigraphy;
            is_visible = true;
            name = "Stratigraphy";
            id = 0;
        }


        inline void clear()
        {

            type = TYPE::Stratigraphy;
            id = 0;


            for( auto &it: input_curves )
            {
                (it.second).clear();
            }
            input_curves.clear();

            for( auto &it: input_edges )
            {
                (it.second).clear();
            }
            input_edges.clear();


            path_curve.clear();

            surface_vertices.clear();
            surface_faces.clear();

            path_curve.clear();

            name.clear();
            is_visible = true;

        }


        inline void clearAllCurves()
        {
            for( auto &it: input_curves )
            {
                clearCurve( it.first );
            }

            if( path_curve.isEmpty() == false )
                path_curve.clear();

        }

        inline void clearCurve( double depth_ )
        {
            input_curves[ depth_ ].clear();
            input_edges[ depth_ ].clear();
        }

        inline void clearSurface()
        {
            surface_vertices.clear();
            surface_faces.clear();
        }

        inline void initialize()
        {

            setDefaultValues();
            id = index;

            name = std::string( "Surface " ) + std::to_string( (int) id );
            ++index;

        }


    protected:

        struct Color { int red, green, blue; } color;


        TYPE type;

        std::size_t id;
        static std::size_t index;
        bool is_visible;

        std::string name;

        std::map< double,  Curve2D > input_curves;
        std::map< double,  std::vector< std::size_t > > input_edges;
        Curve2D path_curve;

        std::vector< double > surface_vertices;
        std::vector< std::size_t > surface_faces;





};

#endif // OBJECT_H
