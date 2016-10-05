#include "ExtrusionRulesProcessor.hpp"
#include <cmath>

#define _RP_UNUSED(x) (void)(x) 

namespace RRM
{
    // 
    // Begin methods to interface with GUI
    // 
    void ExtrusionRulesProcessor::setOrigin( double opengl_x, double opengl_y, double opengl_z )
    {
        origin_.x = opengl_x; 
        origin_.y = opengl_z; 
        origin_.z = opengl_y; 

        got_origin_ = true; 
    }

    bool ExtrusionRulesProcessor::setLenght( double opengl_x, double opengl_y, double opengl_z )
    {
        if ( ( opengl_x > 0 ) && ( opengl_y > 0 ) && ( opengl_z > 0 ) ) { 
            lenght_.x = opengl_x; 
            lenght_.y = opengl_z; 
            lenght_.z = opengl_y; 
            got_lenght_ = true; 

            return true;
        }

        return false; 
    }

    size_t ExtrusionRulesProcessor::getMaxDiscretizationLevel() const
    {
        return max_discretization_level_; 
    }

    bool ExtrusionRulesProcessor::setMaxDiscretizationLevel( size_t level ) 
    {
        if ( level < 1 ) 
        {
            return false; 
        }

        if ( container_.size() != 0 )
        {
            return false; 
        }

        max_discretization_level_ = level;  
        return true; 
    } 

    bool ExtrusionRulesProcessor::requestChangeDiscretizationLevel( size_t level_width, size_t level_depth )
    {
        if ( (level_width > max_discretization_level_) || (level_depth > max_discretization_level_) ) 
        {
            return false; 
        }

        if ( level_width == 0 )
        {
            level_width = 1; 
        }

        if ( level_depth == 0 ) 
        {
            level_depth = 1; 
        }

        level_I_ = level_width; 
        numI_ = static_cast<size_t>( std::pow(static_cast<double>(2), static_cast<int>(level_I_)) ); 

        level_J_ = level_depth; 
        numJ_ = static_cast<size_t>( std::pow(static_cast<double>(2), static_cast<int>(level_J_)) ); 

        return true; 
    }

    void ExtrusionRulesProcessor::requestChangeDiscretization( size_t discretization_width, size_t discretization_depth ) 
    {
        numI_ = discretization_width; 
        numJ_ = discretization_depth; 

        PlanarSurface::requestChangeDiscretization(numI_, numJ_); 
    }

    bool ExtrusionRulesProcessor::init()
    {
        if ( got_origin_ == false ) { 
            setOrigin( 0., 0., 0. );
        }

        if ( got_lenght_ == false ) { 
            setLenght( 1., 1., 1. );
        }

        if ( max_discretization_level_ == 0 ) { 
            setMaxDiscretizationLevel(7); 
            requestChangeDiscretizationLevel( getMaxDiscretizationLevel(), getMaxDiscretizationLevel() ); 
        }

        PlanarSurface::setOutputCoordinatesOrdering( 
            PlanarSurface::Coordinate::WIDTH,
            PlanarSurface::Coordinate::DEPTH, 
            PlanarSurface::Coordinate::HEIGHT); 

        initialized_ = true; 

        return true;
    }

    void ExtrusionRulesProcessor::clear()
    {
        container_.clear(); 
        dictionary_.clear(); 
    }

    bool ExtrusionRulesProcessor::update(State s)
    {
        if ( isInitialized() == false ) { 
            return false; 
        }

        if ( s == State::UNDEFINED ) 
        {
            return false; 
        }

        current_state_ = s; 
        return true; 
    }

    bool ExtrusionRulesProcessor::canUndo() 
    {
        if ( container_.size() > 0 )
        {
            return true;
        }

        return false;
    }

    bool ExtrusionRulesProcessor::undo()
    {
        if ( canUndo() == false )
        {
            return false; 
        }

        PlanarSurface::Ptr last_sptr; 
        container_.popLastSurface(last_sptr);

        State last_state = applied_geologic_rules_.back(); 
        applied_geologic_rules_.pop_back(); 

        size_t last_surface_index = inserted_surfaces_indices_.back(); 
        inserted_surfaces_indices_.pop_back(); 

        undoed_surfaces_stack_.push_back(last_sptr); 
        undoed_surfaces_indices_.push_back(last_surface_index); 
        undoed_geologic_rules_.push_back(last_state); 

        auto iter = dictionary_.find(last_surface_index); 
        dictionary_.erase(iter); 

        return true;
    }

    bool ExtrusionRulesProcessor::canRedo() 
    {
        if ( undoed_geologic_rules_.size() > 0 )
        {
            return true;
        }

        return false;
    }

    bool ExtrusionRulesProcessor::redo()
    {
        return false; 

        PlanarSurface::Ptr undoed_sptr = undoed_surfaces_stack_.back(); 
        undoed_surfaces_stack_.pop_back(); 

        current_state_ = undoed_geologic_rules_.back();
        undoed_geologic_rules_.pop_back(); 

        return false;
    }


    //
    // End methods to interface with GUI
    // 

    bool ExtrusionRulesProcessor::isInitialized() const
    {
        return initialized_; 
    }

    bool ExtrusionRulesProcessor::getSurfaceIndex( const size_t controller_index, size_t &index ) const
    {
        auto iter = dictionary_.find(controller_index);  

        if ( iter == dictionary_.end() ) { 
            return false; 
        }
        else { 
            index = iter->second;
        }

        return true;
    }

    bool ExtrusionRulesProcessor::getSurfaceID( const size_t controller_index, PlanarSurface::SurfaceId &surface_id )
    {
        size_t index; 

        if ( getSurfaceIndex(controller_index, index) == false ) {
            return false; 
        }

        PlanarSurface::Ptr sptr(container_[index]);
        surface_id = sptr->getID();

		return false; 
    }

    bool ExtrusionRulesProcessor::addSurface( const std::vector<Point3> &curve, size_t given_index, 
            std::vector<size_t> lbound_indices, std::vector<size_t> ubound_indices )
    {
        /* Check is ExtrusionRulesProcessor state and inputs are valid */

        if ( isInitialized() == false ) { 
            return false; 
        }

        bool status = true; 
        std::vector<size_t> lbounds, ubounds;

        for ( auto &i : lbound_indices ) 
        {
            size_t j;
            status &= getSurfaceIndex(i, j);

            if ( status ) { 
                lbounds.push_back(j); 
            }
        }

        for ( auto &i : ubound_indices )
        {
            size_t j;
            status &= getSurfaceIndex(i, j);

            if ( status ) { 
                ubounds.push_back(j); 
            }
        }

        if ( status == false )
        {
            return false; 
        }


        /* Create a surface */
        PlanarSurface::Ptr sptr = std::make_shared<PlanarSurface>(true);

        sptr->setOrigin(origin_); 
        sptr->setLenght(lenght_); 
        sptr->addPoints(curve); 
        sptr->generateSurface(); 


        /* Execute selected Geologic Rule */
        return executeAction(sptr, given_index, lbounds, ubounds); 
    }


    bool ExtrusionRulesProcessor::executeAction( 
            PlanarSurface::Ptr &sptr, 
            size_t given_index, 
            std::vector<size_t> lbounds, 
            std::vector<size_t> ubounds
            )
    {
        size_t index;
        bool status; 

        if ( current_state_ == State::SKETCHING )
        {
            status = container_.addSurface(sptr, index, ubounds, lbounds);
        }
        else
        {
            status = container_.addSurface(sptr, index); 
        }

        switch ( current_state_ ) 
        { 
            case State::SKETCHING: 
                break; 

            case State::RA_SKETCHING: 
                if ( status == true )
                    container_.removeAbove();
                break; 

            case State::RAI_SKETCHING:
                if ( status == true )
                    container_.removeAboveIntersection(); 
                break;

            case State::RB_SKETCHING:
                if ( status == true )
                    container_.removeBelow(); 
                break;

            case State::RBI_SKETCHING:
                if ( status == true )
                    container_.removeBelowIntersection(); 
                break;

            default:
                status = false; 
                break;
        }

        if ( status == true ) { 
            dictionary_[given_index] = index; 
            inserted_surfaces_indices_.push_back(given_index); 
            applied_geologic_rules_.push_back(current_state_); 
        }

        return status;
    }

}; /* namespace RRM */
