#ifndef __SRULES_HPP__
#define __SRULES_HPP__

#include <vector>

/* #include <iostream> */ 
/* using namespace std; */ 

#include "planar_surface.hpp" 

class SRules
{
    public: 
        using ContainerType = std::vector<PlanarSurface::Ptr>; 

        size_t size() { 
            return container.size(); 
        }

        PlanarSurface::Ptr operator[]( size_t surface_index ) { 
            return container[surface_index]; 
        }

        bool isValidSurface( PlanarSurface::Ptr sptr ) { 
            return ( ( sptr != nullptr ) && ( sptr->surfaceIsSet() != false ) ); 
        }

        bool addSurface( PlanarSurface::Ptr sptr ) { 
            if ( isValidSurface(sptr) == false ) { 
                return false; 
            }
            /* cout << "Got a surface!\n"; */ 

            if ( define_above_ == true ) {
                /* cout << "Someone was defined above!\n"; */ 
                sptr->removeBelow(lower_bound_); 
            }

            if ( define_below_ == true ) { 
                /* cout << "Someone was defined below!\n"; */ 
                sptr->removeAbove(upper_bound_); 
            }

            container.push_back(sptr); 
            return true; 
        }

        bool removeAbove( PlanarSurface::Ptr sptr ) { 
            if ( isValidSurface(sptr) == false ) { 
                return false; 
            }

            if ( weakEntireSurfaceCheck(sptr) == false ) { 
                return false; 
            }

            bool status = false; 
            for ( auto s : container ) 
            {
                if ( s->surfaceIsSet() == true  ) {
                    status |= boundaryAwareRemoveAbove(sptr, s); 
                }
            }

            return status; 
        }

        bool removeAboveIntersection( PlanarSurface::Ptr sptr ) { 
            if ( isValidSurface(sptr) == false ) { 
                return false; 
            }

            if ( weakEntireSurfaceCheck(sptr) == false ) { 
                return false; 
            }

            bool status = false; 
            for ( auto s : container ) 
            {
                if ( s->surfaceIsSet() == true ) 
                    if ( s->weakIntersectionCheck(sptr) ) { 
                        status |= boundaryAwareRemoveAbove(sptr, s); 
                    }
            }

            return status; 
        }

        bool removeBelow( PlanarSurface::Ptr sptr ) { 
            if ( isValidSurface(sptr) == false ) { 
                return false; 
            }

            if ( weakEntireSurfaceCheck(sptr) == false ) { 
                return false; 
            }

            bool status = false; 
            for ( auto s : container ) 
            {
                if ( s->surfaceIsSet() == true ) { 
                    status |= boundaryAwareRemoveBelow(sptr, s); 
                }
            }

            return status; 
        }

        bool removeBelowIntersection( PlanarSurface::Ptr sptr ) { 
            if ( isValidSurface(sptr) == false ) { 
                return false; 
            }

            if ( weakEntireSurfaceCheck(sptr) == false ) { 
                return false; 
            }

            bool status = false; 
            for ( auto s : container ) 
            {
                if ( s->surfaceIsSet() ) 
                    if ( s->weakIntersectionCheck(sptr) ) { 
                        status |= boundaryAwareRemoveBelow(sptr, s); 
                    }
            }

            return status; 
        }

        bool defineAbove( PlanarSurface::Ptr sptr ) { 
            if ( isValidSurface(sptr) == false ) { 
                stopDefineAbove(); 
                return false; 
            }

            if ( weakEntireSurfaceCheck(sptr) == false ) { 
                stopDefineAbove(); 
                return false; 
            }

            lower_bound_ = sptr; 
            define_above_ = true; 
            return true; 
        }

        void stopDefineAbove() { 
            lower_bound_ = nullptr; 
            define_above_ = false; 
        }

        bool defineBelow( PlanarSurface::Ptr sptr ) { 
            if ( isValidSurface(sptr) == false ) { 
                stopDefineBelow(); 
                return false; 
            }

            if ( weakEntireSurfaceCheck(sptr) == false ) { 
                stopDefineBelow(); 
                return false; 
            }

            upper_bound_ = sptr; 
            define_below_ = true; 
            return true; 
        }

        void stopDefineBelow() { 
            upper_bound_ = nullptr;
            define_below_ = false; 
        }


    private: 
        ContainerType container; 

        PlanarSurface::Ptr lower_bound_; 
        bool define_above_ = false; 

        PlanarSurface::Ptr upper_bound_; 
        bool define_below_ = false; 

        bool weakEntireSurfaceCheck( PlanarSurface::Ptr &s ) 
        {
            bool status = false; 
            /* cout << "Check for entire surface: "; */ 

            if ( define_above_ && define_below_ ) 
            {
                status |= s->weakBoundedEntireSurfaceCheck( lower_bound_, upper_bound_ ); 
                /* cout << " defined above and below: " << status << endl; */ 

            }
            else if ( define_above_ ) 
            {
                status |= s->weakBoundedEntireSurfaceCheck( lower_bound_, PlanarSurface::Ptr() ); 
                /* cout << " defined above: " << status << endl; */ 
            }
            else if ( define_below_ ) 
            {
                status |= s->weakBoundedEntireSurfaceCheck( PlanarSurface::Ptr(), upper_bound_ ); 
                /* cout << " defined below: " << status << endl; */ 
            }
            else 
            {
                status |= s->weakEntireSurfaceCheck(); 
            }

            return status; 
        }

        bool boundaryAwareRemoveAbove( PlanarSurface::Ptr base_surface, PlanarSurface::Ptr to_remove_surface )  
        {
            bool status = false; 

            if ( define_above_ && define_below_ ) 
            { 
                if ( to_remove_surface->weakLiesAboveCheck(lower_bound_) )  
                    if ( to_remove_surface->weakLiesBelowCheck(upper_bound_) ) { 
                        to_remove_surface->removeAbove(base_surface); 
                        status |= true;
                    }
            }
            else if ( define_above_ ) 
            {
                if ( to_remove_surface->weakLiesAboveCheck(lower_bound_) ) { 
                    to_remove_surface->removeAbove(base_surface); 
                    status |= true;
                }
            }
            else if ( define_below_ ) 
            { 
                if ( to_remove_surface->weakLiesBelowCheck(upper_bound_) ) { 
                    to_remove_surface->removeAbove(base_surface); 
                    status |= true; 
                }
            }
            else 
            { // if ( !define_above_ && !define_below_ ) { 
                to_remove_surface->removeAbove(base_surface); 
                status |= true; 
            }

            return status; 
            }

            bool boundaryAwareRemoveBelow( PlanarSurface::Ptr base_surface, PlanarSurface::Ptr to_remove_surface )  
            {
                bool status = false; 

                if ( define_above_ && define_below_ ) 
                { 
                    if ( to_remove_surface->weakLiesAboveCheck(lower_bound_) )  
                        if ( to_remove_surface->weakLiesBelowCheck(upper_bound_) ) { 
                            to_remove_surface->removeBelow(base_surface); 
                            status |= true;
                        }
                }

                else if ( define_above_ ) 
                { 
                    if ( to_remove_surface->weakLiesAboveCheck(lower_bound_) ) { 
                        to_remove_surface->removeBelow(base_surface); 
                        status |= true;
                    }
                }

                else if ( define_below_ ) 
                { 
                    if ( to_remove_surface->weakLiesBelowCheck(upper_bound_) ) { 
                        to_remove_surface->removeBelow(base_surface); 
                        status |= true; 
                    }
                }

                else 
                { // if ( !define_above_ && !define_below_ ) { 
                    to_remove_surface->removeBelow(base_surface); 
                    status |= true; 
                }

                return status; 
                }
            }; 

#endif 

