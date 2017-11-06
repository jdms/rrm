/********************************************************************************/
/*                                                                              */
/* This file is part of the "Stratigraphy Modeller Library" (StratModLib)       */
/* Copyright (C) 2017, Julio Daniel Machado Silva.                              */
/*                                                                              */
/* StratModLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                   */
/* License as published by the Free Software Foundation; either                 */
/* version 3 of the License, or (at your option) any later version.             */
/*                                                                              */
/* StratModLib is distributed in the hope that it will be useful,               */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU            */
/* Lesser General Public License for more details.                              */
/*                                                                              */
/* You should have received a copy of the GNU Lesser General Public             */
/* License along with StratModLib.  If not, see <http://www.gnu.org/licenses/>, */
/* or write to the Free Software Foundation, Inc., 51 Franklin Street,          */
/* Fifth Floor, Boston, MA  02110-1301  USA.                                    */
/*                                                                              */
/********************************************************************************/

#include <numeric>

#include "sutilities.hpp"

#include "smodeller_impl.hpp"

#include "planin/interpolated_graph.hpp"



SUtilities::SUtilities( SModeller &m ) : model_(m) {}

size_t SUtilities::getNumVerticesWidth()
{
    return 2*model_.getWidthDiscretization() + 1;
}

size_t SUtilities::getNumVerticesDepth()
{
    return 2*model_.getLengthDiscretization()+ 1;
}

bool SUtilities::getExtrusionPath( size_t surface_id, std::vector<double> &path_vertex_list )
{
    size_t index; 
    if ( model_.pimpl_->getSurfaceIndex(surface_id, index) == false )
    {
        return false; 
    }

    return model_.pimpl_->container_[index]->getPathVertexList(path_vertex_list);

    return true;
}

bool SUtilities::getNormalList( size_t surface_id, std::vector<double> &normals )
{
    size_t index; 
    if ( model_.pimpl_->getSurfaceIndex(surface_id, index) == false )
    {
        /* std::cout << "\nGot wrong surface index for normals: " << index << std::endl; */
        return false; 
    }

    bool status = model_.pimpl_->container_[index]->getNormalList(normals);

    /* std::cout << "The result of getting normals from stratmod was: " << (status ? "true" : "false") << ", and got " << normals.size()/3 << " normals" <<  std::endl; */

    return status;

}

bool SUtilities::getWidthCrossSectionCurveVertices( std::size_t,  std::size_t, std::vector< std::vector<double> > )
{
    return false;
}

bool SUtilities::getLengthCrossSectionCurveVertices( std::size_t, std::size_t, std::vector< std::vector<double> > )
{
    return false;
}


size_t SUtilities::getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps, size_t active_cross_section )
{
    if ( model_.pimpl_->container_.size() == 0 ) 
    {
        return 0;
    }

    size_t depth_discretization = (num_extrusion_steps > 0) ? num_extrusion_steps + 1 : 2;

    using Curve = std::vector<Point3>; 
    using CurveContainer = std::vector<Curve>;
    CurveContainer curves; 

    model_.pimpl_->container_[0]->updateDiscretization();
    size_t size = model_.pimpl_->container_[0]->getNumX();

    // TODO: remember to check whether the active_cross_section number is valid

    // Process curves ... 

    Point3 p;
    double unused; 
    bool vertex_is_valid, prev_vertex_is_valid, next_vertex_is_valid; 
    bool has_curve = false; 
    PlanarSurface::Natural index, prev_index, next_index;

    size_t container_size = 0;

    for ( size_t k = 0; k < model_.pimpl_->container_.size(); ++k )
    {
        Curve curve;
        auto &sptr = model_.pimpl_->container_[k]; 

        for ( PlanarSurface::Natural i = 0; i < size; ++ i )
        {
            prev_index = sptr->getVertexIndex( (static_cast<long int>(i)-1 > 0 ? i-1 : i), active_cross_section );
            index = sptr->getVertexIndex( i, active_cross_section ); 
            next_index = sptr->getVertexIndex( (i+1 < size ? i+1 : i), active_cross_section ); 

            prev_vertex_is_valid = sptr->getHeight(prev_index, unused); 
            vertex_is_valid = sptr->getVertex3D(index, p); 
            next_vertex_is_valid = sptr->getHeight(next_index, unused); 

            if ( prev_vertex_is_valid || vertex_is_valid || next_vertex_is_valid )
            {
                curve.push_back(p);
                has_curve = true; 
            }

            if ( vertex_is_valid == false )
            {
                if ( has_curve && prev_vertex_is_valid ) 
                {
                    nu.push_back(curve.size());
                    nv.push_back(depth_discretization);
                    container_size += 3*curve.size()*(depth_discretization);
                    has_curve = false;

                    curves.push_back( std::move(curve) );
                    curve = Curve();
                }
            }
        }

        if ( has_curve )
        {
            container_size += 3*curve.size()*(depth_discretization);
            nu.push_back(curve.size());
            nv.push_back(depth_discretization);

            curves.push_back( std::move(curve) );
            has_curve = false;
        }
    }


    // Build Zhao's data structure

    size_t num_surfaces = curves.size(); 
    double extrusion_step = model_.pimpl_->lenght_.z/(static_cast<double>(depth_discretization) - 1.0);
    size_t offset = 0; 

    points.resize(container_size);

    /* std::cout << num_surfaces << std::endl; */ 
    for ( size_t k = 0; k < num_surfaces; ++k )
    {
        // Get surface/curve number k
        auto &curve = curves[k]; 
        /* std::cout << nu[k] << " " << nv[k] << std::endl << std::flush; */ 

        for ( size_t j = 0; j < depth_discretization; ++j )
        {
            for ( size_t i = 0; i < curve.size() ; ++i )
            {
                index = static_cast<PlanarSurface::Natural>( j*curve.size() + i + offset ); 

                points[ 3*index + 0 ] = curve[i].x; 
                points[ 3*index + 1 ] = static_cast<double>(j)*extrusion_step; 
                points[ 3*index + 2 ] = curve[i].z;

                /* std::cout << points[ 3*index + 0 ]  << " " << points[ 3*index + 1 ] << " " << points[ 3*index + 2 ] << std::endl << std::flush; */
            }
            }
            offset += curve.size() * depth_discretization;
        }

        return num_surfaces;
}

bool SUtilities::getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<double> &vlist, std::vector<size_t> &elist )
{
    size_t index; 
    if ( model_.pimpl_->getSurfaceIndex(surface_id, index) == false )
    {
        return false; 
    }

    PlanarSurface::Ptr sptr( model_.pimpl_->container_[index] ); 

    PlanarSurface::Natural Nwidth = 2 * static_cast<PlanarSurface::Natural>(width);

    if ( Nwidth >= sptr->getNumX() )
    {
        return false;
    }

    /* vlist.resize( 2 * sptr->getNumY() ); */
    vlist.resize( 3 * sptr->getNumY() );
    elist.clear(); 

    /* double height, previous_height; */ 
    bool status, previous_status; 

    /* TODO: have a look at those types and their conversions */
    using OutRealType = double;
    using OutNaturalType = size_t;

    /* sptr->getHeight( Nwidth, 0, height ); */
    /* vlist[0] = static_cast<OutRealType>( model_.pimpl_->origin_.x ); */
    /* vlist[1] = static_cast<OutRealType>( height ); */
    Point3 p, previous;
    sptr->getVertex3D( sptr->getVertexIndex(Nwidth,0), p );
    vlist[0] = static_cast<OutRealType>(p.x);
    vlist[1] = static_cast<OutRealType>(p.y);
    vlist[2] = static_cast<OutRealType>(p.z);

    bool has_curve = false;

    for ( PlanarSurface::Natural i = 1; i < sptr->getNumY(); ++i )
    {
        /* status          = sptr->getHeight(Nwidth,     i, height); */
        /* previous_status = sptr->getHeight(Nwidth, i - 1, previous_height); */
        status          = sptr->getVertex3D( sptr->getVertexIndex(Nwidth, i    ), p );
        previous_status = sptr->getVertex3D( sptr->getVertexIndex(Nwidth, i - 1), previous );

        /* vlist[2*i + 0] = static_cast<OutRealType>( model_.pimpl_->origin_.x + (double)(i) * model_.pimpl_->lenght_.x / ( (double)(sptr->getNumY() - 1) ) ); */
        /* vlist[2*i + 1] = static_cast<OutRealType>( height ); */
        vlist[3*i + 0] = static_cast<OutRealType>(p.x);
        vlist[3*i + 1] = static_cast<OutRealType>(p.y);
        vlist[3*i + 2] = static_cast<OutRealType>(p.z);

        if ( status || previous_status )
        {
            elist.push_back( static_cast<OutNaturalType>(i - 1) );
            elist.push_back( static_cast<OutNaturalType>(i) );
            has_curve |= true;
        }
    }

    return has_curve;
}

bool SUtilities::getLengthCrossSectionCurve( size_t surface_id, size_t lenght, std::vector<double> &vlist, std::vector<size_t> &elist )
{
    size_t index; 
    if ( model_.pimpl_->getSurfaceIndex(surface_id, index) == false )
    {
        return false; 
    }

    PlanarSurface::Ptr sptr( model_.pimpl_->container_[index] ); 

    PlanarSurface::Natural Ndepth = 2 * static_cast<PlanarSurface::Natural>(lenght);

    if ( Ndepth >= sptr->getNumY() )
    {
        return false;
    }

    /* vlist.resize( 2 * sptr->getNumX() ); */
    vlist.resize( 3 * sptr->getNumX() );
    elist.clear(); 

    /* double height, previous_height; */ 
    bool status, previous_status; 

    /* TODO: have a look at those types and their conversions */
    using OutRealType = double;
    using OutNaturalType = size_t;

    /* sptr->getHeight(0, Ndepth, height); */
    /* vlist[0] = static_cast<OutRealType>( model_.pimpl_->origin_.x ); */
    /* vlist[1] = static_cast<OutRealType>( height ); */
    Point3 p, previous;
    sptr->getVertex3D( sptr->getVertexIndex(0, Ndepth), p );
    vlist[0] = static_cast<OutRealType>(p.x);
    vlist[1] = static_cast<OutRealType>(p.y);
    vlist[2] = static_cast<OutRealType>(p.z);


    bool has_curve = false;

    for ( PlanarSurface::Natural i = 1; i < sptr->getNumX(); ++i )
    {
        /* status          = sptr->getHeight(    i, Ndepth, height); */
        /* previous_status = sptr->getHeight(i - 1, Ndepth, previous_height); */
        status          = sptr->getVertex3D( sptr->getVertexIndex(    i, Ndepth), p );
        previous_status = sptr->getVertex3D( sptr->getVertexIndex(i - 1, Ndepth), previous );

        /* vlist[2*i + 0] = static_cast<OutRealType>( model_.pimpl_->origin_.x + (double)(i) * model_.pimpl_->lenght_.x / ( (double)(sptr->getNumX() - 1) ) ); */
        /* vlist[2*i + 1] = static_cast<OutRealType>( height ); */
        vlist[3*i + 0] = static_cast<OutRealType>(p.x);
        vlist[3*i + 1] = static_cast<OutRealType>(p.y);
        vlist[3*i + 2] = static_cast<OutRealType>(p.z);


        if ( status || previous_status )
        {
            elist.push_back( static_cast<OutNaturalType>( i - 1 ) );
            elist.push_back( static_cast<OutNaturalType>( i ) );
            has_curve |= true;
        }
    }

    return has_curve;
}

bool SUtilities::getFrontBoundary3DCurves( std::vector<std::vector<double>> &vlists, std::vector<std::vector<size_t>> &elists)
{
    std::cout << "\nFront boundary, getting surface ids: ";
    auto surfaces_ids = model_.getSurfacesIndices();
    bool status = surfaces_ids.size() > 0;

    if ( status == false )
    {
        std::cout << "false -- got 0 surfaces\n";
        return false;
    }
    std::cout << "true -- got " << surfaces_ids.size() << " surfaces\n";

    vlists.clear();
    elists.clear();

    std::vector<double> vlist;
    std::vector<size_t> elist;

    for ( auto id : surfaces_ids )
    {
        std::cout << "Getting vlist & elist to surface: " << id << std::endl;
        status &= getLengthCrossSectionCurve(id, 0, vlist, elist);
        vlists.push_back(vlist);
        elists.push_back(elist);
    }

    computeLateralBoundingCurves(vlists, elists);

    return true;
}

bool SUtilities::getBackBoundary3DCurves( std::vector<std::vector<double>> &vlists, std::vector<std::vector<size_t>> &elists)
{
    std::cout << "\nBack Boundary, getting surface ids: ";
    auto surfaces_ids = model_.getSurfacesIndices();
    bool status = surfaces_ids.size() > 0;

    if ( status == false )
    {
        std::cout << "false -- got 0 surfaces\n";
        return false;
    }
    std::cout << "true -- got " << surfaces_ids.size() << " surfaces\n";

    vlists.clear();
    elists.clear();

    std::vector<double> vlist;
    std::vector<size_t> elist;

    size_t max_lenght = model_.getLengthDiscretization();

    for ( auto id : surfaces_ids )
    {
        std::cout << "Getting vlist & elist to surface: " << id << std::endl;
        status &= getLengthCrossSectionCurve(id, max_lenght, vlist, elist);
        vlists.push_back(vlist);
        elists.push_back(elist);
    }

    computeLateralBoundingCurves(vlists, elists);

    return true;
}

bool SUtilities::getLeftBoundary3DCurves( std::vector<std::vector<double>> &vlists, std::vector<std::vector<size_t>> &elists)
{
    std::cout << "\nLeft Boundary, getting surface ids: ";
    auto surfaces_ids = model_.getSurfacesIndices();
    bool status = surfaces_ids.size() > 0;

    if ( status == false )
    {
        std::cout << "false -- got 0 surfaces\n";
        return false;
    }
    std::cout << "true -- got " << surfaces_ids.size() << " surfaces\n";

    vlists.clear();
    elists.clear();

    std::vector<double> vlist;
    std::vector<size_t> elist;

    for ( auto id : surfaces_ids )
    {
        std::cout << "Getting vlist & elist to surface: " << id << std::endl;
        status &= getWidthCrossSectionCurve(id, 0, vlist, elist);
        vlists.push_back(vlist);
        elists.push_back(elist);
    }

    computeLateralBoundingCurves(vlists, elists);

    return true;
}

bool SUtilities::getRightBoundary3DCurves( std::vector<std::vector<double>> &vlists, std::vector<std::vector<size_t>> &elists)
{
    std::cout << "\nLeft Boundary, getting surface ids: ";
    auto surfaces_ids = model_.getSurfacesIndices();
    bool status = surfaces_ids.size() > 0;

    if ( status == false )
    {
        std::cout << "false -- got 0 surfaces\n";
        return false;
    }
    std::cout << "true -- got " << surfaces_ids.size() << " surfaces\n";

    vlists.clear();
    elists.clear();

    std::vector<double> vlist;
    std::vector<size_t> elist;

    size_t max_width = model_.getWidthDiscretization();

    for ( auto id : surfaces_ids )
    {
        std::cout << "Getting vlist & elist to surface: " << id << std::endl;
        status &= getWidthCrossSectionCurve(id, max_width, vlist, elist);
        vlists.push_back(vlist);
        elists.push_back(elist);
    }

    computeLateralBoundingCurves(vlists, elists);

    return true;
}

bool SUtilities::computeLateralBoundingCurves( std::vector<std::vector<double>> &vlists, std::vector<std::vector<size_t>> &elists )
{
    double zj, zi;
    size_t temp, size = model_.getSurfacesIndices().size();

    std::vector<size_t> ordered_surfaces(size);

    std::vector<double> vlist( 3*size );
    std::vector<size_t> elist( 2*(size-1) );


    // For the left
    std::iota(ordered_surfaces.begin(), ordered_surfaces.end(), 0);
    std::cout << "Ordering curves by height on the left, result is: ";
    for ( size_t i = 0; i < size; i++ )
    {
        for ( size_t j = i+1; j < size; j++ )
        {
            zi = vlists[ordered_surfaces[i]].operator[](2);
            zj = vlists[ordered_surfaces[j]].operator[](2);

            if ( zj < zi )
            {
                temp = ordered_surfaces[i];
                ordered_surfaces[i] = ordered_surfaces[j];
                ordered_surfaces[j] = temp;
            }
        }
        std::cout << ordered_surfaces[i] << " ";
    }
    std::cout << "\n";

    std::cout << "Creating vertical curve on the left.\n";
    vlist[0] = vlists[ordered_surfaces[0]].operator[](0);
    vlist[1] = vlists[ordered_surfaces[0]].operator[](1);
    vlist[2] = vlists[ordered_surfaces[0]].operator[](2);

    std::cout << "Vertical curve on the left is: ";
    std:: cout <<  "( " << vlist[0] << ", " << vlist[1] << ", " << vlist[2] << "), ";
    /* std::cout << "Connectivity is : "; */


    for ( size_t i = 1; i < ordered_surfaces.size(); ++i )
    {
        vlist[3*i + 0] = vlists[ordered_surfaces[i]].operator[](0);
        vlist[3*i + 1] = vlists[ordered_surfaces[i]].operator[](1);
        vlist[3*i + 2] = vlists[ordered_surfaces[i]].operator[](2);
        std:: cout <<  "( " << vlist[3*i + 0] << ", " << vlist[3*i + 1] << ", " << vlist[3*i + 2] << "), ";

        elist[2*(i-1) + 0] = i-1;
        elist[2*(i-1) + 1] = i;
        /* std::cout << elist[2*(i-1) + 0] << ", " << elist[2*(i-1) + 1] << "; "; */
    }
    std::cout << "\n";

    vlists.push_back(vlist);
    elists.push_back(elist);


    // For the right
    std::iota(ordered_surfaces.begin(), ordered_surfaces.end(), 0);
    /* std::cout << "Ordering curves by height on the right, result is:\n"; */
    std::cout << "Ordering curves by height on the right, result is: ";
    // waiting for a bug
    size_t last_entry_index = vlists[0].size() -3;

    for ( size_t i = 0; i < size; i++ )
    {
        for ( size_t j = i+1; j < size; j++ )
        {
            zi = vlists[ordered_surfaces[i]].operator[](last_entry_index + 2);
            zj = vlists[ordered_surfaces[j]].operator[](last_entry_index + 2);
            /* std::cout << "Surface " << i << "'s height is: " << zi << "\n"; */
            /* std::cout << "Surface " << j << "'s height is: " << zj << "\n"; */

            if ( zj < zi )
            {
                temp = ordered_surfaces[i];
                ordered_surfaces[i] = ordered_surfaces[j];
                ordered_surfaces[j] = temp;
            }
        }
        /* std::cout << "Smallest surface is: " << ordered_surfaces[i] << "\n"; */
        std::cout << ordered_surfaces[i] << " ";
    }
    std::cout << "\n";

    std::cout << "Creating vertical curve on the right.\n";
    vlist[0] = vlists[ordered_surfaces[0]].operator[](last_entry_index + 0);
    vlist[1] = vlists[ordered_surfaces[0]].operator[](last_entry_index + 1);
    vlist[2] = vlists[ordered_surfaces[0]].operator[](last_entry_index + 2);

    std::cout << "Vertical curve on the right is: ";
    std:: cout <<  "( " << vlist[0] << ", " << vlist[1] << ", " << vlist[2] << "), ";

    for ( size_t i = 1; i < ordered_surfaces.size(); ++i )
    {
        vlist[3*i + 0] = vlists[ordered_surfaces[i]].operator[](last_entry_index + 0);
        vlist[3*i + 1] = vlists[ordered_surfaces[i]].operator[](last_entry_index + 1);
        vlist[3*i + 2] = vlists[ordered_surfaces[i]].operator[](last_entry_index + 2);
        std:: cout <<  "( " << vlist[3*i + 0] << ", " << vlist[3*i + 1] << ", " << vlist[3*i + 2] << "), ";

        elist[2*(i-1) + 0] = i-1;
        elist[2*(i-1) + 1] = i;
    }
    std::cout << "\n\n";

    vlists.push_back(vlist);
    elists.push_back(elist);


    return true;
}

