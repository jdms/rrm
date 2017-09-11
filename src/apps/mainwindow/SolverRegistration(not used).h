#ifndef SOLVERREGISTRATION_H
#define SOLVERREGISTRATION_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <unordered_map>


#include "SolverInterface.h"


//TODO:review nomenclatues and  data structures


enum class ManyfoldType { VERTEX, EDGE, FACE, CELL };
enum class MeshPattern { UNSTRUCTURED, REGULAR };
enum class CellShape { TRIANGULAR, QUADRILATERAL, TETRAHEDRAL, HEXAHEDRAL };

struct PropertyData
{
    std::string name;
    std::vector< std::string> labels;
    std::vector<std::string> values;
    std::pair< double, double> boundaries;
};


struct MeshData
{
    unsigned int dimension;
    MeshPattern pattern;
    CellShape cell_shape;
};

using property_getter = std::function< bool( std::vector< double >& values, std::string& msg ) >;
using map_properties_getter = std::unordered_map< std::string, property_getter >;


using property_setter = std::function< bool( const double x, const double y, const double z,
                                         const std::vector< double >& values, std::string& msg ) >;
using map_properties_setter = std::unordered_map< std::string, property_setter >;

using map_input_property = std::unordered_map< std::string, PropertyData >;




class SolverRegistration
{

    public:

        SolverRegistration( )
        {
            definingOutputPropertyGetters();
            definingInputPropertySetters();

        }


        bool runGetter( const std::string name_, std::vector< double >& values_, std::string& msg_ )
        {
            auto iter = output_property_dictionary.find( name_ );
            if ( iter == output_property_dictionary.end() )
            {
                msg_ = name_ + std::string( ": property not recognized" );
                return false;
            }

            bool status = iter->second( values_, msg_ );
            return status;
        }

        bool runSetter( const std::string name_, const double x_, const double y_, const double z_,
                        const std::vector< double >& values_, std::string& msg_ )
        {
            auto iter = input_property_dictionary.find( name_ );
            if ( iter == input_property_dictionary.end() )
            {
                msg_ = name_ + std::string( ": property not recognized" );
                return false;
            }

            bool status = iter->second(  x_, y_, z_, values_, msg_ );
            return status;
        }



        bool registerSolverData( map_input_property& input_data_, map_input_property& boundary_data_,
                             map_input_property& output_data_ )
        {
            registerInputProperties();
            registerBoundaryProperties();
            registerOutputProperties();
            registerMeshes();

            input_data_ = input_data_vector;
            boundary_data_ = boundary_data_vector;
            output_data_ = output_data_vector;

            return true;
        }

        bool registerSolverMeshes( MeshData& input_mesh_, MeshData& output_mesh_ )
        {

            registerMeshes();

            input_mesh_ = input_mesh;
            output_mesh_ = output_mesh;

            return true;
        }



        bool registerInputProperties()
        {

            std::vector<std::string> labels_ = { "param0", "param1" };
            std::vector<std::string> values_ = { "value0", "value1" };
            std::pair< double, double > boundaries_ = std::make_pair( 0.2f, -0.5f );
            std::string msg_ ;

            return addInputProperty( std::string( "input1" ), labels_, values_, boundaries_, msg_ );
        }

        bool registerBoundaryProperties()
        {
            std::vector<std::string> labels_ = { "param0", "param1" };
            std::vector<std::string> values_ = { "value0", "value1" };
            std::pair< double, double > boundaries_ = std::make_pair( 0.2f, -0.5f );
            std::string msg_ ;

            bool bd0_added = addBoundaryProperty( std::string( "boundary0" ), labels_, values_, boundaries_, msg_ );
            bool bd1_added = addBoundaryProperty( std::string( "boundary1" ), labels_, values_, boundaries_, msg_ );

            return ( bd0_added && bd1_added );
        }

        bool registerOutputProperties()
        {
            std::vector<std::string> labels_ = { "param0", "param1" };
            std::vector<std::string> values_ = { "value0", "value1" };
            std::pair< double, double > boundaries_ = std::make_pair( 0.2f, -0.5f );
            std::string msg_ ;

            bool bd0_added = addOutputProperty( std::string( "boundary0" ), labels_, values_, boundaries_, msg_ );
            bool bd1_added = addOutputProperty( std::string( "boundary0" ), labels_, values_, boundaries_, msg_ );

            return ( bd0_added && bd1_added );
            return true;
        }

        bool registerMeshes()
        {
            setInputMesh( 2, MeshPattern::REGULAR, CellShape::TRIANGULAR );
            setOutputMesh( 3, MeshPattern::UNSTRUCTURED, CellShape::TETRAHEDRAL );

            return true;
        }



    protected:

        map_properties_getter output_property_dictionary;
        map_properties_setter input_property_dictionary;
        map_input_property input_data_vector;
        map_input_property boundary_data_vector;
        map_input_property output_data_vector;

        MeshData input_mesh;
        MeshData output_mesh;


    private:

        SolverInterface interface;


    private:


        void definingOutputPropertyGetters()
        {
            output_property_dictionary[ "pressure" ] =  [this]( std::vector< double >& values,
                                                           std::string& msg ) ->
                                                    bool {
                                                        return this->getPressure( values, msg );
                                                    };


            output_property_dictionary[ "forward_tof" ] =  [this]( std::vector< double >& values,
                                                           std::string& msg ) ->
                                                    bool {
                                                        return this->getForwardTOF( values, msg );
                                                    };

            output_property_dictionary[ "backward_tof" ] =  [this]( std::vector< double >& values,
                                                           std::string& msg ) ->
                                                    bool {
                                                        return this->getBackwardTOF( values, msg );
                                                    };

            output_property_dictionary[ "total_tof" ] =  [this]( std::vector< double >& values,
                                                           std::string& msg ) ->
                                                    bool {
                                                        return this->getTotalTOF( values, msg );
                                                    };

            output_property_dictionary[ "max_tracer" ] =  [this]( std::vector< double >& values,
                                                           std::string& msg ) ->
                                                    bool {
                                                        return this->getMaxTracer( values, msg );
                                                    };

            output_property_dictionary[ "permeability" ] =  [this]( std::vector< double >& values,
                                                           std::string& msg ) ->
                                                    bool {
                                                        return this->getPermeability( values, msg );
                                                    };

            output_property_dictionary[ "porosity" ] =  [this]( std::vector< double >& values,
                                                           std::string& msg ) ->
                                                    bool {
                                                        return this->getPorosity( values, msg );
                                                    };
        }

        bool getPressure( std::vector< double >& values_, std::string& msg_ ) const
        {
            interface.getPressure( values_ );

            if( values_.empty() == true ){
                msg_ = "property's values empty";
                std::cout << msg_.c_str() << std::endl;

                return false;
            }

            return true;

        }

        bool getForwardTOF( std::vector< double >& values_, std::string& msg_  )
        {
            interface.getForwardTOF( values_ );

            if( values_.empty() == true ){
                msg_ = "property's values empty";
                std::cout << msg_.c_str() << std::endl;

                return false;
            }

            return true;

        }

        bool getBackwardTOF( std::vector< double >& values_, std::string& msg_  )
        {
            interface.getBackwardTOF( values_ );

            if( values_.empty() == true ){
                msg_ = "property's values empty";
                std::cout << msg_.c_str() << std::endl;

                return false;
            }

            return true;

        }

        bool getTotalTOF( std::vector< double >& values_, std::string& msg_ )
        {
            interface.getTotalTOF( values_ );

            if( values_.empty() == true ){
                msg_ = "property's values empty";
                std::cout << msg_.c_str() << std::endl;

                return false;
            }

            return true;

        }

        bool getMaxTracer( std::vector< double >& values_, std::string& msg_  )
        {
            interface.getMaxTracer( values_ );

            if( values_.empty() == true ){
                msg_ = "property's values empty";
                std::cout << msg_.c_str() << std::endl;

                return false;
            }
            return true;

        }

        bool getPermeability( std::vector< double >& values_, std::string& msg_  )
        {
            interface.getPermeability( values_ );

            if( values_.empty() == true ){
                msg_ = "property's values empty";
                std::cout << msg_.c_str() << std::endl;

                return false;
            }
            return true;

        }

        bool getPorosity( std::vector< double >& values_, std::string& msg_  )
        {
            interface.getPorosity( values_ );

            if( values_.empty() == true )
            {
                msg_ = "property's values empty";
                std::cout << msg_.c_str() << std::endl;

                return false;
            }

            return true;

        }



        void definingInputPropertySetters()
        {
            input_property_dictionary[ "permeability" ] =  [this]( const double x, const double y,
                                                            const double z,
                                                            const std::vector< double >& values,
                                                            std::string& msg ) ->
                                                            bool {
                                                                return this->setPermeability( x, y,
                                                                                  z, values, msg );
                                                            };

            input_property_dictionary[ "viscosity" ] =  [this]( const double x, const double y,
                                                            const double z,
                                                            const std::vector< double >& values,
                                                            std::string& msg ) ->
                                                            bool {
                                                                return this->setViscosity( x, y,
                                                                                  z, values, msg );
                                                            };


            input_property_dictionary[ "porosity" ] =  [this]( const double x, const double y,
                                                            const double z,
                                                            const std::vector< double >& values,
                                                            std::string& msg ) ->
                                                            bool {
                                                                return this->setPorosity( x, y,
                                                                                  z, values, msg );
                                                            };

        }

        bool setPermeability( const double x_, const double y_, const double z_,
                              const std::vector< double >& values_,  std::string& msg_ )
        {

            if( values_.empty() == true ){
                msg_ = "property's values empty";
                std::cout << msg_.c_str() << std::endl;

                return false;
            }

            interface.setPermeability( x_, y_, z_, values_[ 0 ] );
            return true;
        }

        bool setViscosity( const double x_, const double y_, const double z_,
                              const std::vector< double >& values_,  std::string& msg_ )
        {

            if( values_.empty() == true ){
                msg_ = "property's values empty";
                std::cout << msg_.c_str() << std::endl;

                return false;
            }

            interface.setViscosity( x_, y_, z_, values_[ 0 ] );
            return true;
        }

        bool setPorosity( const double x_, const double y_, const double z_,
                              const std::vector< double >& values_,  std::string& msg_ )
        {

            if( values_.empty() == true ){
                msg_ = "property's values empty";
                std::cout << msg_.c_str() << std::endl;

                return false;
            }

            interface.setPorosity( x_, y_, z_, values_[ 0 ] );
            return true;
        }



        /* Registering properties */


        bool addInputProperty( std::string& name_, std::vector<std::string>& labels_,
                              std::vector<std::string>& values_, std::pair<double, double> boundaries_,
                              std::string& msg_ )
        {


            auto iter = input_data_vector.find( name_ );
            if ( iter != input_data_vector.end() )
            {
                msg_ = name_ + std::string( ": property already existent" );
                return false;
            }


            PropertyData id_;
            id_.name = name_;
            id_.labels = labels_;
            id_.values = values_;
            id_.boundaries = boundaries_;

            input_data_vector[ name_ ] = id_;

            return true;

        }


        bool addBoundaryProperty( std::string& name_, std::vector<std::string>& labels_,
                              std::vector<std::string>& values_, std::pair<double, double> boundaries_,
                              std::string& msg_ )
        {

            auto iter = boundary_data_vector.find( name_ );
            if ( iter != boundary_data_vector.end() )
            {
                msg_ = name_ + std::string( ": property already existent" );
                return false;
            }


            PropertyData bd_;
            bd_.name = name_;
            bd_.labels = labels_;
            bd_.values = values_;
            bd_.boundaries = boundaries_;

            boundary_data_vector[ name_ ] = bd_;

            return true;
        }


        bool addOutputProperty( std::string& name_, std::vector<std::string>& labels_,
                              std::vector<std::string>& values_, std::pair<double, double> boundaries_,
                              std::string& msg_ )
        {

            auto iter = output_data_vector.find( name_ );
            if ( iter != output_data_vector.end() )
            {
                msg_ = name_ + std::string( ": property already existent" );
                return false;
            }


            PropertyData od_;
            od_.name = name_;
            od_.labels = labels_;
            od_.values = values_;
            od_.boundaries = boundaries_;

            output_data_vector[ name_ ] = od_;

            return true;
        }


        bool setInputMesh( unsigned int dimension_, const MeshPattern& pattern_,
                           const CellShape& shape_ )
        {
            input_mesh.dimension = dimension_;
            input_mesh.pattern = pattern_;
            input_mesh.cell_shape = shape_;
            return true;
        }


        bool setOutputMesh( unsigned int dimension_, const MeshPattern& pattern_,
                           const CellShape& shape_ )
        {
            output_mesh.dimension = dimension_;
            output_mesh.pattern = pattern_;
            output_mesh.cell_shape = shape_;
            return true;
        }
};

#endif // SOLVERREGISTRATION_H
