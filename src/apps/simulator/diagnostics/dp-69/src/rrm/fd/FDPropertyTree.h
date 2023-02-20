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

#pragma once

#include "rrm/fd/core/property_editor/QPropertyTreeView.hpp"

namespace rrm::fd
{
  class FDWidget;

  class FDPropertyTree
  {
    FDWidget* fd_widget_;

    using QPropertyTreeView = core::property_editor::QPropertyTreeView;
    
    std::unique_ptr<QPropertyTreeView> tree_view_factory_;
    QPropertyTreeView* tree_view_ = nullptr; 
    
    core::property_editor::PropertyCategory* summary_category_;
    core::property_editor::PropertyCategory* petrophysics_fluid_category_;

    void InitProperties();
  public:
    FDPropertyTree() = default;
    
    void BeginInit(FDWidget* fd_widget) {
      fd_widget_ = fd_widget;
      tree_view_factory_ = std::make_unique<QPropertyTreeView>();
      tree_view_ = tree_view_factory_.get();
    }
    
    void EndInit() {
      InitProperties();
    }

    std::unique_ptr<QPropertyTreeView> AcquireTreeView() {
      return std::move(tree_view_factory_);
    }

    QPropertyTreeView* TreeView() {
      return tree_view_;
    }

    auto* SummaryCategory() const { return summary_category_; }
    auto* PetrophysicsFluidCategory() const { return petrophysics_fluid_category_; }
  };
}
