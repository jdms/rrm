#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <exception>

//FIXME: remove this typedef later
typedef int Curve2d;


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
            return index;
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

/*

        inline void addCrossSectionofDepth( double _depth )
        {
            depth_of_cross_sections.push_back( _depth );
        }

        inline double getCrossSectionofId( std::size_t _id ) const
        {
            try
            {
                return depth_of_cross_sections.at( _id );
            }
            catch ( const std::exception& e )
            {
                std::cout << "a standard exception was caught as trying to access cross-section of "
                          << _id << ", with message '"
                          << e.what() << "'\n";
            }

        }

        inline void getDepthofAllCrossSections( std::vector< double >& _cross_sections ) const
        {

            _cross_sections = depth_of_cross_sections;
        }


        inline void addInputCurve( Curve2d* const& _curve )
        {
            input_curves.push_back( _curve );
        }

        inline Curve2d* getInputCurveofId( std::size_t _id ) const
        {
            try
            {
                input_curves.at( _id );
            }
            catch ( const std::exception& e )
            {
                std::cout << " a standard exception was caught as trying to access input_curve of "
                          << _id << ", with message '"
                          << e.what() << "'\n";
            }

        }

        inline const std::vector< Curve2d* >&  getAllInputCurves() const
        {
            return input_curves;
        }
*/

        inline void addInputCurve( double _depth, Curve2d* const& _curve )
        {
            input_curves[ _depth ] = _curve;
        }

        inline Curve2d* const& getInputCurveofCrossSection( double _depth )
        {
            return input_curves[ _depth ];
        }

        inline std::vector< double > getAllCrossSectionsRelatedtoObject() const
        {
            std::vector< double > depth_cross_sections_;
            for( auto const &it : input_curves ) {
                depth_cross_sections_.push_back( it.first );
            }
            return depth_cross_sections_;
        }

        inline std::vector< Curve2d* > getAllInputCurves() const
        {
            std::vector< Curve2d* > input_curves_;
            for( auto const &it : input_curves) {
                input_curves_.push_back( it.second );
            }
            return input_curves_;
        }

        inline std::size_t getNumberofInputCurves() const
        {
            return input_curves.size();
        }


        inline void setPathCurve( const Curve2d& _path )
        {
            path_curve = _path;
        }

        inline const Curve2d& getPathCurve() const
        {
            return path_curve;
        }


        inline void setDefaultValues()
        {
            type = TYPE::Stratigraphy;
            is_visible = true;
            name = "Stratigraphy";
        }


        inline void clear()
        {
//            depth_of_cross_sections.clear();
            input_curves.clear();

            //FIXME: clean curve2d
            path_curve = 0;

        }

        inline void initialize()
        {

            //TODO: incremental id using static variable
            index = 0;

            clear();
            setDefaultValues();

        }


    protected:

        TYPE type;

        std::size_t index;
        bool is_visible;

        std::string name;
//        std::vector< double > depth_of_cross_sections;

//        std::vector< Curve2d* > input_curves;
        std::map< double,  Curve2d* > input_curves;
        Curve2d path_curve;

};

#endif // OBJECT_H
