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

#include "rrm/fd/FDWidget.h"
#include "rrm/fd/FDPropertyTree.h"
#include "rrm/fd/FDPropertyTreeItems.hpp"
#include "rrm/fd/ImportExport.hpp"

namespace rrm::fd
{
  template <typename T, typename... Rest>
  void AddPropertyEditorItems(
    core::property_editor::PropertyModel& model,
    core::property_editor::PropertyCategory* category, FDWidget* fd_widget) {
    model.AddItem(category, T{fd_widget});
  
    if constexpr (dpl::has_any_v<Rest...>)
      AddPropertyEditorItems<Rest...>(model, category, fd_widget);
  }
  

  // void FDPropertyTree::InitContextMenu() {
    // tree_view_->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    // QObject::connect(tree_view_, &QTreeView::customContextMenuRequested, [this](const QPoint& pos) {
    //   context_menu_tree_.popup(tree_view_->viewport()->mapToGlobal(pos));
    // });


    // connect(tree_view_->header(), &QHeaderView::sectionResized, this, [this](int idx, int old_size, int new_size) {
    //   if (idx == 0 && new_size > 13*rendering_settings_.font_size)
    //     tree_view_->setColumnWidth(0, 13*rendering_settings_.font_size);
    // });


    // load_wells_json_ = context_menu_tree_.addAction("Load Wells...");
    // save_wells_json_ = context_menu_tree_.addAction("Save Wells...");

    // context_menu_tree_.addSeparator();
    // action_export_report_ = context_menu_tree_.addAction("Export Report...");
    // action_export_petrel_wells_ = context_menu_tree_.addAction("Export PETREL Well Heads...");

    // if constexpr (false) {
    //   context_menu_tree_.addSeparator();
    //   import_permeability_ = context_menu_tree_.addAction("Import Permeability...");
    //
    //   QObject::connect(import_permeability_, &QAction::triggered, [this]() {
    //     auto filename = QFileDialog::getOpenFileName(
    //       fd_widget_, "Import Permeability", "", "All Files (*.*)"); // JSON Wells (*.wells.json);;JSON (*.json);;
    //
    //     if (!filename.isEmpty()) {
    //       fd_widget_->GetPetrophysics().imported_permeability.clear();
    //       fd_widget_->GetPetrophysics().imported_porosity.clear();
    //
    //       std::fstream myfile(filename.toStdString(), std::ios_base::in);
    //
    //       double value;
    //       while (myfile >> value) {
    //         fd_widget_->GetPetrophysics().imported_permeability.push_back(units::MilliDarcy(value));
    //
    //         // petrophysics_.imported_porosity.push_back(value > 5 ? 0.25 : 0.1);
    //       }
    //
    //       fd_widget_->Model().SetPoroPermByDomains(fd_widget_->GetPetrophysics());
    //
    //       auto update_property = [](ContiniousProperty& p) {
    //         p.DataArray()->Modified();
    //
    //         if (p.use_default_range)
    //           p.Lut()->SetTableRange(p.GetDefaultRange());
    //       };
    //
    //       update_property(fd_widget_->PermeabilityXYProperty());
    //       update_property(fd_widget_->PermeabilityZProperty());
    //       update_property(fd_widget_->PorosityProperty());
    //
    //
    //       fd_widget_->UpdateFull(true);
    //     }
    //   });
    // }


    // QObject::connect(load_wells_json_, &QAction::triggered, [this] { fd_widget_->LoadWellsDialog(); });
    // QObject::connect(save_wells_json_, &QAction::triggered, [this] { fd_widget_->ExportWellsDialog(); });

    // QObject::connect(action_export_petrel_wells_, &QAction::triggered, [this]() {
    //   auto filename = QFileDialog::getSaveFileName(
    //     fd_widget, "Export PETREL Well Heads", "", "Text Files (*.txt);;All Files (*.*)");
    //
    //   if (!filename.isEmpty()) {
    //     ExportPetrelWellHeads(wells_, filename.toStdString());
    //   }
    // });
    
    // QObject::connect(action_export_report_, &QAction::triggered, [this] {
    //   auto filename = QFileDialog::getSaveFileName(
    //     fd_widget_, "Export Report", "", "CSV (Comma delimited) (*.csv);;All Files (*.*)");
    //
    //   if (!filename.isEmpty()) {
    //     QtCharts::QPieSeries* inj;
    //     QtCharts::QPieSeries* prod;
    //
    //     std::unique_ptr<QtCharts::QPieSeries> other;
    //
    //     if (fd_widget_->well_part_.prod_->isChecked()) {
    //       prod = static_cast<QtCharts::QPieSeries*>(fd_widget_->tracer_pie_chart_->series()[0]);
    //       other.reset(fd_widget_->CreateTracerPieSeries(false));
    //       inj = other.get();
    //     }
    //     else {
    //       inj = static_cast<QtCharts::QPieSeries*>(fd_widget_->tracer_pie_chart_->series()[0]);
    //       other.reset(fd_widget_->CreateTracerPieSeries(true));
    //       prod = other.get();
    //     }
    //
    //     ExportReport(
    //       fd_widget_->wells_,
    //       fd_widget_->well_results_,
    //       inj, prod,
    //       fd_widget_->lorenz_series_, fd_widget_->sweep_series_,
    //       fd_widget_->fd_property_tree_.SummaryCategory(),
    //       fd_widget_->fd_summary_.lorenz_coef, fd_widget_->fd_summary_.upscaled_perm_, filename.toStdString());
    //   }
    // });

    // context_menu_tree_.addSeparator();
    
    // if (!fd_widget_->rrm_project_path.empty()) {
    //   fd_widget_->action_save_petrophysics_local_ = context_menu_tree_.addAction("Save Petrophysics (Project)");
    //   QObject::connect(fd_widget_->action_save_petrophysics_local_, &QAction::triggered, [this]() {
    //     nlohmann::json j;
    //
    //     for (auto& [_, d] : fd_widget_->petrophysics_.domains)
    //       j.push_back(d);
    //
    //     std::ofstream(
    //       std::filesystem::path(fd_widget_->rrm_project_path).replace_extension("fdsettings.json")) << j.dump(2);
    //   });
    // }

    // fd_widget_->action_save_petrophysics_global_ = context_menu_tree_.addAction("Save Petrophysics (Default)");
    // QObject::connect(fd_widget_->action_save_petrophysics_global_, &QAction::triggered, [this]() {
    //   nlohmann::json j;
    //
    //   std::map<std::string, DomainSettings> global_map;
    //
    //   try {
    //     LoadSettings(global_map, nlohmann::json::parse(std::ifstream("_config/fdsettings.json")));
    //   }
    //   catch (...) {}
    //   
    //   for (auto& [_, d] : fd_widget_->petrophysics_.domains) {
    //     std::string name = d.name;
    //     boost::algorithm::to_lower(name);
    //     global_map[name] = d;
    //   }
    //
    //   for (auto& [_, d] : global_map)
    //     j.push_back(d);
    //
    //   std::ofstream("_config/fdsettings.json") << j.dump(2);
    // });
  // }

  void FDPropertyTree::InitProperties() {
    using wptr = FDWidget*;
  
    auto& tree_model = *tree_view_->model();
  
    auto* visual_category = tree_model.AddCategory("Visualisation");
  
  
    auto add_bool = [this, &tree_model](
      core::property_editor::PropertyCategory* cat, std::string_view name, const auto& get, const auto& set) {
      tree_model.AddItem(cat, FunctorPropertyItem<
        decltype(get), bool, decltype(set), FinaliserRender>{fd_widget_, name, get, set});
    };
  
    auto add_bool_visual = [add_bool, visual_category](
      std::string_view name, const auto& get, const auto& set) {
      add_bool(visual_category, name, get, set);
    };
  
  
    add_bool_visual(
      "Grid",
      [](wptr w) { return w->ImageDataActor()->GetVisibility(); },
      [](wptr w, bool v) {
        w->ImageDataActor()->SetVisibility(v);
        w->RefreshCubeAxesBounds();
      }
    );
  
    add_bool_visual(
      "Edges",
      [](wptr w) { return w->ImageDataActor()->GetProperty()->GetEdgeVisibility(); },
      [](wptr w, bool v) { w->ImageDataActor()->GetProperty()->SetEdgeVisibility(v); }
    );
    
    add_bool_visual(
      "Colour bar",
      [](wptr w) { return w->ScalarBar()->GetVisibility(); },
      [](wptr w, bool v) { w->ScalarBar()->SetVisibility(v); }
    );
    
    add_bool_visual(
      "Compass",
      [](wptr w) { return w->CompassWidget()->GetOrientationMarker()->GetVisibility(); },
      [](wptr w, bool v) { w->CompassWidget()->GetOrientationMarker()->SetVisibility(v); }
    );
    
    {
      add_bool_visual(
        "Wells",
        [](wptr w) {
          bool visible = true;
          if (!w->VisualWells().empty())
            visible = w->VisualWells().front().caption_actor->GetVisibility();
          return visible;
        },
        [](wptr w, bool v) {
          // auto value = !wells_visibility(w);
          
          for (auto& well : w->VisualWells())
            well.SetVisibility(v);
          w->RefreshCubeAxesBounds();
        }
      );
    }
  
    if (fd_widget_->SurfacesActor())
      add_bool_visual(
        "Surfaces",
        [](wptr w) { return w->SurfacesActor()->GetVisibility(); },
        [](wptr w, bool v) {
          for (auto* a : dpl::range<vtkActor>(w->SurfacesActor()))
            a->SetVisibility(v);
          w->SurfacesActor()->SetVisibility(v);
          w->RefreshCubeAxesBounds();
        }
      );
  
    auto* camera_category = tree_model.AddCategory(visual_category, "Camera");
    {
      tree_model.AddItem(camera_category, ViewportItem{fd_widget_});
      
      add_bool(camera_category,
        "Perspective",
        [](wptr w) { return !w->Renderer()->GetActiveCamera()->GetParallelProjection(); },
        [](wptr w, bool v) { w->Renderer()->GetActiveCamera()->SetParallelProjection(!v); });
  
  
      auto* light_category = tree_model.AddCategory(camera_category, "Lighting");
      
      tree_model.AddItem(light_category,
        FunctorPropertyItem{
          fd_widget_, "Ambient",
          [](wptr w) { return w->ImageDataActor()->GetProperty()->GetAmbient(); },
          [](wptr w, double v) {
            w->ImageDataActor()->GetProperty()->SetAmbient(v);
            w->Render();
          }
        }
      );
  
      tree_model.AddItem(light_category,
        FunctorPropertyItem{
          fd_widget_, "Diffuse",
          [](wptr w) { return w->ImageDataActor()->GetProperty()->GetDiffuse(); },
          [](wptr w, double v) {
            w->ImageDataActor()->GetProperty()->SetDiffuse(v);
            w->Render();
          }
        }
      );
  
      tree_model.AddItem(light_category,
        FunctorPropertyItem{
          fd_widget_, "Specular",
          [](wptr w) { return w->ImageDataActor()->GetProperty()->GetSpecular(); },
          [](wptr w, double v) {
            w->ImageDataActor()->GetProperty()->SetSpecular(v);
            w->Render();
          }
        }
      );
  
    //   PureFunctionPropertyItem{
    //   this, "Resolution",
    //   [](wptr w) { return w->task_params_.steps; },
    //   [](wptr w, dpl::vector2i v) { w->task_params_.steps = v; }
    // }
    }
  
    auto* axes_cat = tree_model.AddCategory(visual_category, "Axes");
    {
      tree_model.AddItem(axes_cat, ExaggerationItem{fd_widget_});
      tree_model.AddItem(axes_cat, BoundingBoxItem{fd_widget_, false});
      tree_model.AddItem(axes_cat, BoundingBoxItem{fd_widget_, true});
    }
    
    // {
    // {
    //   auto* pie_chart_cat = tree_model.AddCategory(visual_category, "Pie charts");
    //
    //   struct PieCategoryBoolItem : BaseWidgetItem<PieCategoryBoolItem, bool> {
    //     std::string name;
    //     bool* value;
    //     
    //     auto Name() { return name; }
    //
    //     Type Get() { return *value; }
    //
    //     void Set(bool v) {
    //       *value = v;
    //       widget->ResetPieCharts();
    //     }
    //   };
    //   
    //   tree_model.AddItem(pie_chart_cat, PieCategoryBoolItem{fd_widget_, "Filtered", &fd_widget_->DomainPieSettings().filtered_});
    //   tree_model.AddItem(pie_chart_cat, PieCategoryBoolItem{fd_widget_, "Pore volume", &fd_widget_->DomainPieSettings().pore_volume_});
    //
    //   struct PiePrecentageItem : BaseWidgetItem<PiePrecentageItem, bool> {
    //     auto Name() { return "Percentage"; }
    //
    //     Type Get() { return widget->DomainPieSettings().view_->show_percentage; }
    //
    //     void Set(bool v) {
    //       widget->DomainPieSettings().view_->show_percentage = v;
    //       widget->DomainPieSettings().view_->viewport()->update();
    //     }
    //   };
    //   
    //   tree_model.AddItem(pie_chart_cat, PiePrecentageItem{fd_widget_});
    // }
    
    auto* thresholds_cat = tree_model.AddCategory(visual_category, "Filters");
  
    auto* thresholds_domains_cat = tree_model.AddCategory(thresholds_cat, "Domains");
  
    
    // tree_model.AddItem(thresholds_cat, DomainRangeItem{this}); // DomainInThresholdItem
    tree_model.AddItem(thresholds_cat, TofThresholdItem{fd_widget_, "ToF from, yrs", fd_widget_->CellFilter().max_tof_forward});
    tree_model.AddItem(thresholds_cat, TofThresholdItem{fd_widget_, "ToF to, yrs", fd_widget_->CellFilter().max_tof_backward});
    tree_model.AddItem(thresholds_cat, SliceTypeItem{fd_widget_});
    /*title_prop_item_ = */
    tree_model.AddItem(thresholds_cat, SliceValueItem{fd_widget_});
    // SliceTypeItem
    // }
  
    tree_model.AddItem(thresholds_cat, CutoffTypeItem{fd_widget_});
    
    tree_model.AddItem(thresholds_cat, CutoffItem{fd_widget_});
  
  
    auto* grid_cat = tree_model.AddCategory("Grid");
  
    AddPropertyEditorItems<
      DisplayPropertyItem,
      ResolutionItem,
      GridLocationItem,
      GridSizeItem,
      ScalarRangeItem,
      LogScaleItem
    >(tree_model, grid_cat, fd_widget_);
  
    {
      auto* bound_cond_cat = tree_model.AddCategory(grid_cat, "Boundary conditions, MPa");
  
      auto& bound_cond = fd_widget_->GetBoundaryConditions();
  
      constexpr auto coef = dpl::units::coef<dpl::units::megapascal>;
      tree_model.AddItem(bound_cond_cat, BoundaryConditionFaceItem{fd_widget_, "X min", &bound_cond.values[0], coef});
      tree_model.AddItem(bound_cond_cat, BoundaryConditionFaceItem{fd_widget_, "X max", &bound_cond.values[1], coef});
      tree_model.AddItem(bound_cond_cat, BoundaryConditionFaceItem{fd_widget_, "Y min", &bound_cond.values[2], coef});
      tree_model.AddItem(bound_cond_cat, BoundaryConditionFaceItem{fd_widget_, "Y max", &bound_cond.values[3], coef});
      tree_model.AddItem(bound_cond_cat, BoundaryConditionFaceItem{fd_widget_, "Z min", &bound_cond.values[4], coef});
      tree_model.AddItem(bound_cond_cat, BoundaryConditionFaceItem{fd_widget_, "Z max", &bound_cond.values[5], coef});
  
      // auto& outer = fd_widget_->Petrophysics().domains[fd_widget_->Model().OuterDomain()];
      
      // tree_model.AddItem(bound_cond_cat, BoolValueItem{fd_widget_, outer.name, &bound_cond.include_outer})->color =
      //   outer.color;
    }
  
  
  
  
  
  
  
  
    {
      auto* well_cat = tree_model.AddCategory("Well");
  
      AddPropertyEditorItems<
        WellNameItem,
        PressureItem,
        WellLocationItem,
        PerforationItem,
        WellboreRadiusItem
      >(tree_model, well_cat, fd_widget_);
  
  
      using namespace dpl::units;
      
      tree_model.AddItem(well_cat,
        FunctorPropertyItem(fd_widget_, "Flow rate, m3/day",
          [](wptr w) {
            auto& well_result = w->GetFlowSummary().well_results[w->ActiveVisualWell()->input];
            return (boost::format("%s : %s")
              % dpl::ft_format(std::abs(/*1e-3**/well_result.flow_rate/*/coef<foot3>*/*coef<day>)/*, 2*/)
              % (well_result.IsProducer() ? "prod" : "inj")).str();
          }));
  
      tree_model.AddItem(well_cat,
        FunctorPropertyItem(fd_widget_, "Breakthrough, yrs",
          [](wptr w) {
            return (boost::format("%.2f")
              % (w->GetFlowSummary().well_results[w->ActiveVisualWell()->input].breakthrough_time/units::Year())).str();
          }));
    }
  
    petrophysics_fluid_category_ = tree_model.AddCategory("Petrophysics & fluid");
  
    {
      tree_model.AddItem(petrophysics_fluid_category_, ViscosityItem{fd_widget_});
  
      auto* perm_category = tree_model.AddCategory(petrophysics_fluid_category_, "Permeability, mD"/*, "lateral, vertical"*/);
      auto* poro_category = tree_model.AddCategory(petrophysics_fluid_category_, "Porosity, %"/*, petrophysics*/);


      
  
      for (auto& [d_idx, d] : fd_widget_->Petrophysics().domains) {
        tree_model.AddItem(perm_category, PermeabilityItem{fd_widget_, &d})/*->color = d.color*/;
        tree_model.AddItem(poro_category, PorosityItem{fd_widget_, &d})/*->color = d.color*/;
  
        {
          tree_model.AddItem(
            thresholds_domains_cat, DomainInThresholdItem{fd_widget_, d_idx})->color = d.color;
        }
      }
    }
  
  
    summary_category_ = tree_model.AddCategory("Summary");
  
  
  
    using namespace dpl::units;
  
  
  
    struct separate_thousands : std::numpunct<char> {
        char_type do_thousands_sep() const override { return ','; }  // separate with commas
        string_type do_grouping() const override { return "\3"; } // groups of 3 digit
    };


    
    {
      struct PoreVolumeItem : BaseWidgetItem<PoreVolumeItem, std::string> {
        constexpr auto Name() { return "Pore volume, m3"; }
      
        Type Get() { return
          dpl::ft_format(widget->GetFlowSummary().pore_volume_excluding_outer);
          // (boost::format("%s (%s)")
          //   % dpl::ft_format(widget->GetFlowSummary().pore_volume_filter)
          //   % dpl::ft_format(widget->GetFlowSummary().pore_volume_excluding_outer)).str();
        }

        constexpr bool IsReadOnly() { return true; }

        constexpr auto Tooltip() { return "*excludes <Outer>"; }
      };
      
      tree_model.AddItem(summary_category_, PoreVolumeItem{fd_widget_}

      //   PureFunctionPropertyItem(fd_widget_, "Pore volume, m3",
      //     // [](wptr w) { return (boost::format("%.2f MMbbl") % (barrel{cubic_metre{w->fd_summary_.pore_volume_}}/1e6)).str(); }
      // [](wptr w) {
      //     return dpl::ft_format(w->GetFlowSummary().pore_volume_excluding_outer);
      //   // (boost::format("%s") %
      //     // dpl::ft_format(/*foot3{*/metre3{/*1e-6**/w->fd_summary_.pore_volume_}/*}, 2*/)).str();
      // }
      // )
      );
    }
    
  
    
    
    tree_model.AddItem(summary_category_,
      FunctorPropertyItem(fd_widget_, "Flow rate, m3/day",
        [](wptr w) {
          return dpl::ft_format(w->GetFlowSummary().total_inj_flow_rate*coef<day>);
          // return (boost::format("%.2f Mcf/d") % (1e-3*w->fd_summary_.total_inj_flow_rate_/coef<cubic_foot>*coef<day>)).str();
          // return (boost::format("%s ") %
          //   dpl::ft_format(/*1e-3**/w->fd_summary_.total_inj_flow_rate_/*/coef<foot3>*/*coef<day>)).str();
        }));
  
    tree_model.AddItem(summary_category_,
      FunctorPropertyItem(fd_widget_, "yrs @ 1 pvi",
        [](wptr w) { return (boost::format("%.1f") % (w->GetFlowSummary().TimeInPVI()/units::Year())).str(); }));
  
    /*title_prop_item_ = */
    tree_model.AddItem(summary_category_, UpscaledPermItem{fd_widget_});
  
    tree_model.AddItem(summary_category_,
      FunctorPropertyItem(fd_widget_, "Lorenz coefficient",
        [](wptr w) { return (boost::format("%.3f") % w->GetFlowSummary().lorenz_coef).str(); }));
  
  
    auto* recovery_category = tree_model.AddCategory(summary_category_, "Recovery at");
    
    
    tree_model.AddItem(recovery_category,
      FunctorPropertyItem(fd_widget_, "0.5 pvi",
        [](wptr w) { return (boost::format("%.1f %%") % (w->GetFlowSummary().SweepAtPV(0.5)*100.0)).str(); }));
  
    tree_model.AddItem(recovery_category,
      FunctorPropertyItem(fd_widget_, "1 pvi",
        [](wptr w) { return (boost::format("%.1f %%") % (w->GetFlowSummary().SweepAtPV(1.0)*100.0)).str(); }));
  
    tree_model.AddItem(recovery_category,
      FunctorPropertyItem(fd_widget_, "2 pvi",
        [](wptr w) { return (boost::format("%.1f %%") % (w->GetFlowSummary().SweepAtPV(2.0)*100.0)).str(); }));
    
    /*title_prop_item_ = */
    tree_model.AddItem(recovery_category,
      FunctorPropertyItem(fd_widget_, "10 yrs",
        [](wptr w) { return (boost::format("%.1f %%") % (w->GetFlowSummary().SweepAtYears(1e1)*100.0)).str(); }));
  
    tree_model.AddItem(recovery_category,
      FunctorPropertyItem(fd_widget_, "100 yrs",
        [](wptr w) { return (boost::format("%.1f %%") % (w->GetFlowSummary().SweepAtYears(1e2)*100.0)).str(); }));
  
    tree_model.AddItem(recovery_category,
      FunctorPropertyItem(fd_widget_, "1,000 yrs",
        [](wptr w) { return (boost::format("%.1f %%") % (w->GetFlowSummary().SweepAtYears(1e3)*100.0)).str(); }));
  
    tree_view_->expandAll();
    tree_view_->collapse(tree_model.index(thresholds_cat));
    // tree_view_->collapse(tree_model.index(bound_cond_cat));
    tree_view_->collapse(tree_model.index(axes_cat));
    tree_view_->collapse(tree_model.index(camera_category));
    
    tree_view_->resizeColumnToContents(0);
  }
}
