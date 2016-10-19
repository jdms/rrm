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
        current_.bounded_below_ = false; 
        current_.bounded_above_ = false; 
        current_.state_ = State::UNDEFINED; 
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

        current_.state_ = s;
        return true; 
    }


    bool ExtrusionRulesProcessor::defineAbove( size_t surface_index )
    {
        ContainerSurfaceIndex index;
        if ( getSurfaceIndex(surface_index, index) == false )
        {
            return false; 
        }

        // Do not accept surface as boundary if it was alread
        // used as an upper boundary
        if ( current_.bounded_above_ == true )
        {
            if ( current_.upper_boundary_ == surface_index )
            {
                return false;
            }
        }

        current_.bounded_below_ = container_.defineAbove(index); 
        current_.lower_boundary_ = surface_index;

        return current_.bounded_below_; 
    }


    void ExtrusionRulesProcessor::stopDefineAbove()
    {
        current_.bounded_above_ = false;
        container_.stopDefineAbove(); 
    }


    bool ExtrusionRulesProcessor::defineBelow( size_t surface_index )
    {
        ContainerSurfaceIndex index;
        if ( getSurfaceIndex(surface_index, index) == false )
        {
            return false; 
        }

        // Do not accept surface as boundary if it was alread
        // used as a lower boundary
        if ( current_.bounded_below_ == true )
        {
            if ( current_.lower_boundary_ == surface_index )
            {
                return false;
            }
        }
        current_.bounded_above_ = container_.defineBelow(index);
        current_.upper_boundary_ = surface_index;

        return current_.bounded_above_; 
    }


    void ExtrusionRulesProcessor::stopDefineBelow()
    {
        current_.bounded_below_ = false;
        container_.stopDefineBelow(); 
    }


    void ExtrusionRulesProcessor::removeAbove()
    {
        current_.state_ = State::RA_SKETCHING;
    }

    void ExtrusionRulesProcessor::removeAboveIntersection()
    {
        current_.state_ = State::RAI_SKETCHING; 
    }

    void ExtrusionRulesProcessor::removeBelow()
    {
        current_.state_ = State::RB_SKETCHING;
    }

    void ExtrusionRulesProcessor::removeBelowIntersection()
    {
        current_.state_ = State::RBI_SKETCHING; 
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

        size_t last_surface_index = inserted_surfaces_indices_.back(); 
        inserted_surfaces_indices_.pop_back(); 

        StateDescriptor last = past_states_.back();
        past_states_.pop_back();

        current_.bounded_above_ = last.bounded_above_;
        current_.bounded_below_ = last.bounded_below_;
        enforceDefineRegion();

        undoed_surfaces_stack_.push_back(last_sptr); 
        undoed_surfaces_indices_.push_back(last_surface_index); 

        undoed_states_.push_back(last);

        auto iter = dictionary_.find(last_surface_index); 
        dictionary_.erase(iter); 

        return true;
    }

    bool ExtrusionRulesProcessor::canRedo() 
    {
        if ( undoed_surfaces_stack_.size() > 0 )
        {
            return true;
        }

        return false;
    }

    bool ExtrusionRulesProcessor::redo()
    {
        if ( canRedo() == false )
        {
            return false;
        }

        PlanarSurface::Ptr undoed_sptr = undoed_surfaces_stack_.back(); 
        undoed_surfaces_stack_.pop_back(); 

        size_t surface_index = undoed_surfaces_indices_.back();
        undoed_surfaces_indices_.pop_back();

        StateDescriptor state_before_redo_ = current_;
        current_ = undoed_states_.back();
        undoed_states_.pop_back();

        enforceDefineRegion(); 

        bool status = commitSurface(undoed_sptr, surface_index, std::vector<size_t>(), std::vector<size_t>());

        current_ = state_before_redo_;
        return status;
    }


    //
    // End methods to interface with GUI
    // 

    bool ExtrusionRulesProcessor::isInitialized() const
    {
        return initialized_; 
    }

    std::vector<size_t> ExtrusionRulesProcessor::getActiveSurfaces()
    {
        return inserted_surfaces_indices_; 
    }


    bool ExtrusionRulesProcessor::requestDefineRegion( std::vector<size_t> &eligible_surfaces )
    {
        if ( container_.empty() )
        {
            return false; 
        }

        eligible_surfaces.clear(); 
        ControllerSurfaceIndex output_index; 
        ContainerSurfaceIndex index; 

        for ( size_t i = 0; i < inserted_surfaces_indices_.size(); ++i )
        {
            output_index = inserted_surfaces_indices_[i]; 
            if ( getSurfaceIndex(output_index, index) == false )
                continue; 

            if ( container_.weakEntireSurfaceCheck(index) )
            {
                eligible_surfaces.push_back(output_index); 
            }
        }

        return !eligible_surfaces.empty(); 
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

        if ( undoed_surfaces_stack_.empty() == false )
        {
            undoed_surfaces_stack_.clear();
            undoed_surfaces_indices_.clear();
            undoed_states_.clear(); 
        }

        /* Execute selected Geologic Rule */
        return commitSurface(sptr, given_index, lbounds, ubounds); 
    }

    void ExtrusionRulesProcessor::registerState(ControllerSurfaceIndex given_index, ContainerSurfaceIndex index)
    {
            dictionary_[given_index] = index; 
            inserted_surfaces_indices_.push_back(given_index); 

            undoed_states_.push_back(current_);
    }

    bool ExtrusionRulesProcessor::enforceDefineRegion()
    {
        bool status = true; 

        if ( current_.bounded_above_ )
        {
            status &= defineBelow(current_.upper_boundary_);
        }

        if ( current_.bounded_below_)
        {
            status &= defineAbove(current_.lower_boundary_);
        }

        return status; 
    }

    bool ExtrusionRulesProcessor::commitSurface( 
            PlanarSurface::Ptr &sptr, 
            size_t given_index, 
            std::vector<size_t> lbounds, 
            std::vector<size_t> ubounds
            )
    {
        size_t index;
        bool status; 

        if ( current_.state_ == State::SKETCHING )
        {
            status = container_.addSurface(sptr, index, ubounds, lbounds);
        }
        else
        {
            status = container_.addSurface(sptr, index); 
        }

        switch ( current_.state_ ) 
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

            past_states_.push_back(current_);
        }

        return status;
    }

} /* namespace RRM */
