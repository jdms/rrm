/*
 * This file is part of Rapid Reservoir Modelling Software.
 *   | https://rapidreservoir.org/
 *   | https://bitbucket.org/rapidreservoirmodelling/rrm/
 *
 * Copyright (c) 2022
 *   | Dmytro Petrovskyy, PhD
 *   | dmytro.petrovsky@gmail.com
 *   | https://www.linkedin.com/in/dmytro-petrovskyy/
 *
 * RRM is free software: you can redistribute it and/or modify              
 * it under the terms of the GNU General Public License as published by     
 * the Free Software Foundation, either version 3 of the License, or        
 * (at your option) any later version.                                      
 *                                                                         
 * RRM is distributed in the hope that it will be useful,                   
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            
 * GNU General Public License for more details.                             
 *                                                                         
 * You should have received a copy of the GNU General Public License        
 * along with RRM. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "rrm/fd/component/Volumetrics.h"
#include "rrm/fd/FDPropertyTreeItems.hpp"

namespace rrm::fd::component
{
  struct FluidFluidDepthItem : BaseWidgetItem<FluidFluidDepthItem, double>
  {
    Volumetrics* volumetrics;
    
    constexpr auto Name() { return "Depth, m"; }

    Type Get() const {
      return volumetrics->PlaneSource()->GetOrigin()[2];
    }
    
    void Set(const Type& v) const { volumetrics->SetDepth(v); }

    static std::string Format(const double& x) {
      return (boost::format("%.2f") % x).str();
    }
  };
  
  struct DomainInFluidFluidItem : BaseWidgetItem<DomainInFluidFluidItem, bool>
  {
    Volumetrics* volumetrics;
    int domain_idx;

    auto Name() const { return widget->Model().Petrophysics().domains[domain_idx].name; }

    Type Get() const { return volumetrics->Domains().count(domain_idx); }

    void Set(const Type& v) const {
      if (v)
        volumetrics->Domains().insert(domain_idx);
      else
        volumetrics->Domains().erase(domain_idx);
      
      volumetrics->RefreshVolumes();
      widget->HardRefreshPropertyTree();
    }
  };

  
  void Volumetrics::InitFluidFluidContact() {
    for (auto& [idx, settings] : fd_widget_->Petrophysics().domains) {
      if (settings.name != "<Outer>")
        domains_.insert(idx);
    }

    using namespace dpl;

    static constexpr auto margin = 10.0;

    auto origin = fd_widget_->Model().Location() - vector3d{margin};

    plane_source_->SetOrigin(origin);
    plane_source_->SetPoint1(origin + vector3d{fd_widget_->Model().PhysicalSize().x() + 2*margin, 0.0, 0.0});
    plane_source_->SetPoint2(origin + vector3d{0.0, fd_widget_->Model().PhysicalSize().y() + 2*margin, 0.0});
    plane_source_->Update();


    vtkNew<vtkPolyDataMapper> plane_mapper;
    plane_mapper->SetInputConnection(plane_source_->GetOutputPort());

    plane_actor_->SetMapper(plane_mapper);
    plane_actor_->GetProperty()->LightingOff();
    plane_actor_->GetProperty()->EdgeVisibilityOn();
    plane_actor_->GetProperty()->SetColor(vector3d{0.75});
    plane_actor_->GetProperty()->SetEdgeColor(vector3d{0.0});
    plane_actor_->SetVisibility(false);

      
    fd_widget_->ScaleChanged += [this] { plane_actor_->SetScale(fd_widget_->Scale()); };
      
    fd_widget_->Renderer()->AddActor(plane_actor_);

    RefreshVolumes();
  }

  void Volumetrics::SetDepth(double value) {
    dpl::vector3d origin{plane_source_->GetOrigin()};
    dpl::vector3d p1{plane_source_->GetPoint1()};
    dpl::vector3d p2{plane_source_->GetPoint2()};
    
    origin.z() = value;
    p1.z() = value;
    p2.z() = value;
    
    plane_source_->SetOrigin(origin);
    plane_source_->SetPoint1(p1);
    plane_source_->SetPoint2(p2);
    
    plane_source_->Modified();

    RefreshVolumes();

    fd_widget_->HardRefreshPropertyTree();
    
    if (plane_actor_->GetVisibility()) {
      fd_widget_->RefreshCubeAxesBounds();
      fd_widget_->Render();
    }
  }


  void Volumetrics::RefreshVolumes() {
    const auto& model = fd_widget_->Model();
      
    auto [x, y, z] = model.Dims();

    const auto cell_size = model.CellSize();
    const auto cell_volume = cell_size.prod();
    const auto z_cell_size = cell_size.z();
    auto cell_z = model.Location().z() + z_cell_size/2;

    const auto fluid_fluid_depth = plane_source_->GetOrigin()[2];

    volume_.rock_above = 0;
    volume_.pore_above = 0;

    volume_.rock_below = 0;
    volume_.pore_below = 0;

    // auto& domain_map = cartesian_model_.Petrophysics().domains;

    for (int k = 0, idx = 0; k < z; ++k, cell_z += z_cell_size)
      for (int j = 0; j < y; ++j)
        for (int i = 0; i < x; ++i, ++idx)
          if (domains_.count(model.Domain(idx))) {
            if (fluid_fluid_depth <= cell_z) {
              volume_.rock_above += cell_volume;
              volume_.pore_above += cell_volume*model[POROSITY][idx];
            }
            else {
              volume_.rock_below += cell_volume;
              volume_.pore_below += cell_volume*model[POROSITY][idx];
            }
          }
  }

  void Volumetrics::InitPropertyTree() {
    auto& tree_view = *fd_widget_->GetPropertyTree().TreeView();
    auto& tree_model = *tree_view.model();

    using wptr = FDWidget*;

    auto* category = tree_model.AddCategory(
      fd_widget_->GetPropertyTree().PetrophysicsFluidCategory(), "Fluid-fluid contact");

    tree_model.AddItem(category,
      FunctorPropertyItem(fd_widget_, "Show",
        [this](wptr) {
          return static_cast<bool>(plane_actor_->GetVisibility());
        },
        [this](wptr, bool v) {
          plane_actor_->SetVisibility(v);
          fd_widget_->RefreshCubeAxesBounds();
          fd_widget_->Render();
        }
    ));
  
    tree_model.AddItem(category, FluidFluidDepthItem{fd_widget_, this});
  
    auto* domains_cat = tree_model.AddCategory(category, "Domains");

    for (auto& [idx, settings] : fd_widget_->Petrophysics().domains)
      tree_model.AddItem(domains_cat, DomainInFluidFluidItem{fd_widget_, this, idx})->color = settings.color;

    auto* volumes_cat = tree_model.AddCategory(category, "Volumes, m3", "[above, below]");
  
    tree_model.AddItem(volumes_cat,
      FunctorPropertyItem(fd_widget_, "Pore",
        [this](wptr) {
          return (boost::format("[%.2e, %.2e]")
            % dpl::units::metre3{volume_.pore_above}
            % dpl::units::metre3{volume_.pore_below}).str();
        }));
  
    tree_model.AddItem(volumes_cat,
      FunctorPropertyItem(fd_widget_, "Rock",
        [this](wptr) {
          return (boost::format("[%.2e, %.2e]")
            % dpl::units::metre3{volume_.rock_above}
            % dpl::units::metre3{volume_.rock_below}).str();
        }));
    
    tree_view.expand(tree_model.index(volumes_cat));
  }
}