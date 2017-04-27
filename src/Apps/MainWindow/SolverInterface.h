#ifndef SOLVERINTERFACE_H
#define SOLVERINTERFACE_H

#include <vector>
#include <map>



class SolverInterface
{
    public:

        SolverInterface(){}


        void getPressure( std::vector< double >& pressure_ ) const { pressure_ = pressure; }


        void getForwardTOF( std::vector< double >& forward_tof_ ) const { forward_tof_ =
                                                                           forward_tof; }

        void getBackwardTOF( std::vector< double >& backward_tof_ ) const { backward_tof_ =
                                                                            backward_tof; }

        void getTotalTOF( std::vector< double >& total_tof_ ) const { total_tof_ = total_tof; }


        void getMaxTracer( std::vector< double >& max_tracer_ ) const { max_tracer_ = max_tracer; }


        void getPermeability( std::vector< double >& permeability_ ) const { permeability_ =
                                                                             permeability; }

        void getPorosity( std::vector< double >& porosity_ ) const { porosity_ = porosity; }



        void setPermeability( const double x_, const double y_, const double z_,
                              const double permeability_ )
        {
            point p = std::make_tuple( x_, y_, z_ );

            auto iter = map_point_to_region.find( p );
            if ( iter == map_point_to_region.end() )
            {
                   map_point_to_region[ p ].id = map_point_to_region.size();

            }

            map_point_to_region[ p ].permeability = permeability_;

        }

        void setViscosity( const double x_, const double y_, const double z_,
                              const double viscosity_ )
        {
            point p = std::make_tuple( x_, y_, z_ );

            auto iter = map_point_to_region.find( p );
            if ( iter == map_point_to_region.end() )
            {
                   map_point_to_region[ p ].id = map_point_to_region.size();

            }

            map_point_to_region[ p ].viscosity = viscosity_;

        }

        void setPorosity( const double x_, const double y_, const double z_,
                              const double porosity_ )
        {
            point p = std::make_tuple( x_, y_, z_ );



            map_point_to_region[ p ].porosity = porosity_;

        }


    protected:




    protected:


        using point = std::tuple< double, double, double >;
        struct RegionFields
        {
            std::size_t id;
            double permeability;
            double viscosity;
            double porosity;
        };


        const size_t num_mesh_vertices = 10;
        const size_t num_mesh_cells = 7;


        std::vector< double > pressure = std::vector< double >( num_mesh_vertices, 1.0f );
        std::vector< double > forward_tof = std::vector< double >( num_mesh_vertices, 2.0f );
        std::vector< double > backward_tof = std::vector< double >( num_mesh_vertices, 3.0f );
        std::vector< double > total_tof = std::vector< double >( num_mesh_vertices, 4.0f );
        std::vector< double > max_tracer = std::vector< double >( num_mesh_vertices, 5.0f );

        std::vector< double > permeability = std::vector< double >( num_mesh_cells, 1.5f );
        std::vector< double > porosity  = std::vector< double >( num_mesh_cells, 2.5f );


        std::map< point, RegionFields > map_point_to_region;


        void defineIdRegionfromPoint( const point& p_ )
        {
            auto iter = map_point_to_region.find( p_ );
            if ( iter != map_point_to_region.end() )
                return;

            map_point_to_region[ p_ ].id = map_point_to_region.size();
        }

};

#endif // SOLVERINTERFACE_H
