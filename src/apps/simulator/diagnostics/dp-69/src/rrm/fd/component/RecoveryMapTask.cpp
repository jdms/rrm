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

#include "rrm/fd/component/RecoveryMapTask.h"
#include "rrm/fd/FDPropertyTreeItems.hpp"
#include "rrm/fd/FlowCalculations.hpp"

#include <vtkBandedPolyDataContourFilter.h>
#include <vtkContourFilter.h>

#include <vtkDoubleArray.h>

#include <vtkLabeledDataMapper.h>
#include <vtkPlaneSource.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkStripper.h>

#include <QPushButton>





namespace rrm::fd::tasks
{
  struct TaskTypeItem
  {
    RecoveryMapTask* task;
    
    using ComboItem = int;
  
    static auto Name() { return "Name"; }
  
    int Get() { return task->task_params_.type; }
  
    void Set(int prop) {      
      task->task_params_.type = prop;      
      task->HardRefreshTaskTree();
      task->UpdateRecoveryMap();
      // task->fd_widget_->RefreshTitleText();
    }
  
    constexpr int Count() {
      return 3;
    }
    
    static std::string Caption(int item) {
      return item == 0 ? "Recovery map, pvi" : item == 1 ? "Recovery map, time" : "Flow rate, m3/day";
    }
  };
  
  struct MapAtItem : BaseWidgetItem<MapAtItem, double>
  {
    RecoveryMapTask* task;
    
    auto Name() { return task->task_params_.type == 0 ? "At, pvi" : "At, years"; }
  
    double Get() { return task->task_params_.type == 0 ? task->task_params_.at_pvi : task->task_params_.at_time/units::Year(); }
  
    void Set(double prop) {      
      if (task->task_params_.type == 0)
        task->task_params_.at_pvi = prop;
      else
        task->task_params_.at_time = units::Year(prop);
      
      task->UpdateRecoveryMap();
      // task->fd_widget_->RefreshTitleText();
    }
  
    bool IsVisible() { return task->task_params_.type != 2; }
  };

  
  void RecoveryMapTask::EndInit() {
    using wptr = FDWidget*;
    
    auto& model = *task_view_->model();
    
    model.AddItem(FunctorPropertyItem{
      nullptr, "Resolution",
      [this](wptr) { return task_params_.steps; },
      [this](wptr, dpl::vector2i v) { task_params_.steps = v; }
    });
    
    // model.AddItem(TaskTypeItem{this});
    
    // model.AddItem(MapAtItem{this});
    
    model.AddItem(FunctorPropertyItem{
      nullptr, "Fill contours",
      [this](wptr) { return static_cast<bool>(surface_fill_->GetVisibility()); },
      [this](wptr, bool v) {
        surface_fill_->SetVisibility(v);
        isolines_->GetProperty()->SetColor(dpl::vector3d{v ? 0 : 1});
        isolines_->GetProperty()->SetLineWidth(v ? 1.25 : 1);
        fd_widget_->Render();
      }
    });
    
    
    model.AddItem(FunctorPropertyItem{
      nullptr, "Label format",
      [this](wptr) { return task_params_.format; },
      [this](wptr, std::string v) {
        task_params_.format = std::move(v);
        UpdateRecoveryMap();
      }
    });
    
    
    // auto* params_category = model.AddCategory("Parameters");
    
    task_view_->setColumnWidth(0, fd_widget_->GetPropertyTree().TreeView()->columnWidth(0));
    task_view_->expandAll();
  }

  void RecoveryMapTask::BeginInit(FDWidget* fd_widget) {
    fd_widget_ = fd_widget;
    
    isolines_->GetProperty()->LightingOff();
    isolines_->PickableOff();
    isolines_->GetProperty()->SetColor(dpl::vector3d{0});
    isolines_->GetProperty()->SetLineWidth(1.25);
    isolabels_->PickableOff();
    surface_fill_->GetProperty()->LightingOff();

    task_view_ = new core::property_editor::QPropertyTreeView;

    auto* vbox = new QVBoxLayout;
    vbox->setSpacing(0);
    // vbox->addSpacing(6);
    vbox->setContentsMargins(0, 0, 0, 0);


    auto* run_btn = new QPushButton("Run");
    // btn->setStyleSheet("background-color:green;");
    QObject::connect(run_btn, &QPushButton::clicked, [this]() {
      EvaluateCurrentTask();
    });

    auto* hide_btn = new QPushButton("Hide");
    QObject::connect(hide_btn, &QPushButton::clicked, [this]() {
      auto* r = fd_widget_->Renderer();
      r->RemoveActor(isolines_);
      r->RemoveActor(isolabels_);
      r->RemoveActor(surface_fill_);

      fd_widget_->Render();
    });


    auto* hbox_buttons = new QHBoxLayout;
    hbox_buttons->setSpacing(0);
    hbox_buttons->setContentsMargins(0, 0, 0, 0);
    hbox_buttons->addWidget(run_btn);
    hbox_buttons->addWidget(hide_btn);

    vbox->addWidget(task_view_);
    vbox->addLayout(hbox_buttons);


    
    auto* w = new QWidget;
    w->setLayout(vbox);
    w->setContentsMargins(0, 0, 0, 0);

    recovery_map_widget_.reset(w);
    
    // rm_widget_ = std::make_unique<QWidget>(w);
    // return w;
  }



 
  
  void RecoveryMapTask::EvaluateCurrentTask() {
    TimestepRecords timesteps{"Placement Sensitivity"};

    auto& model = fd_widget_->Model();
    
    auto cell_count = model.CellCount();

    auto matrix = FlowSolver::PressureCellMatrix(model);

    auto wells = fd_widget_->Wells();
    
    auto working_well_iter = wells.begin();
    for (auto& w : fd_widget_->Wells()) {
      if (&w == fd_widget_->ActiveVisualWell()->input)
        break;
      ++working_well_iter;
    }

    auto& working_well = *working_well_iter;

    auto [i_count, j_count] = task_params_.steps;

    auto [wx, wy, wz] = model.PhysicalSize();
    auto [x0, y0, z0] = model.Location();

    auto dx = wx / (i_count - 1);
    auto dy = wy / (j_count - 1);

    auto domain_diag_saved = matrix.diagonal;

    auto& sim_results = task_params_.sim_results;
    sim_results.pore_volume = fd_widget_->GetFlowSummary().pore_volume;
    sim_results.steps = task_params_.steps;
    sim_results.points_data.resize(i_count * j_count);

    sim_results.x0 = x0;
    sim_results.y0 = y0;
    sim_results.wx = wx;
    sim_results.wy = wy;
    // sim_results.z = working_well.head_location.z();
    sim_results.z = z0 + wz;

    // std::vector<double> sweep_at_value_arr;
    for (auto j = 0; j < j_count; ++j)
      for (auto i = 0; i < i_count; ++i) {
        working_well.head_location.x() = x0 + i * dx;
        working_well.head_location.y() = y0 + j * dy;

        std::vector<double> constants(cell_count, 0);
        matrix.diagonal = domain_diag_saved;
        FlowSolver::WellPressureSourceTerms(model, wells, matrix.diagonal, constants);

        timesteps
          << "Pressure matrix";

        linear_solver::HypreInput input{matrix, std::move(constants)};
        auto pressure = input.Solve();

        timesteps
          << "Hypre solve";

        flow_diagnostics_soa fd;
        fd.resize(cell_count);
        for (auto IDX = 0; IDX < cell_count; ++IDX) {
          fd[TOF_FORWARD][IDX] = std::numeric_limits<double>::max();
          fd[TOF_BACKWARD][IDX] = std::numeric_limits<double>::max();
          fd[TRACER_FORWARD][IDX] = std::numeric_limits<double>::max();
          fd[TRACER_BACKWARD][IDX] = std::numeric_limits<double>::max();
          // model[DARCY_VEL][i] = fd_[DARCY_VEL][i];
        }
        
        // fd.assign(cell_count, std::numeric_limits<double>::max());


        
        FlowSolver::EvaluateTransport(model, wells, fd_widget_->GetBoundaryConditions(), pressure.data(), fd, timesteps);


        auto& point_data = sim_results.points_data[i + j * i_count];

        FlowSummary dc;
        FlowSolver::EvaluateDiagnosticsCurves(model, dc);
        point_data.sweep = dc.sweep;
        // auto fd_curves = FlowSolver::EvaluateDiagnosticsCurves(cartesian_model_, TofTotalMapper<flow_diagnostics_soa>{&fd});

        {
          auto [x, y, z] = model.Dims();

          dpl::vector3i idx_map{1, x, x * y};

          point_data.total_flow_rate = 0;

          for (auto& well : wells) {
            auto disc_well = model.Discretise(well);

            if (well.pressure > pressure[idx_map.dot({disc_well.x, disc_well.y, disc_well.z0})])
              point_data.total_flow_rate += WellFlowRate(model, well, pressure.data());
          }
        }


        // double sweep_at_value;
        //
        // if (task_params_.type == 0) {
        //   sweep_at_value = dpl::lerp(point_data.sweep, task_params_.at);
        //   
        //   std::cout << 
        //     boost::format("Recovery @ %d pvi(%d, %d) = %d\n")
        //       % task_params_.at % working_well.head_location.x() % working_well.head_location.y() % sweep_at_value;
        // }
        // else {                    
        //   auto time_coef = sim_results.pore_volume/point_data.total_flow_rate/units::Year();         
        //   
        //   sweep_at_value = dpl::lerp(point_data.sweep, task_params_.at/time_coef);
        //
        //   std::cout << 
        //     boost::format("Recovery @ %d yrs(%d, %d) = %d\n")
        //       % task_params_.at % working_well.head_location.x() % working_well.head_location.y() % sweep_at_value;
        // }

        // sweep_at_value_arr.push_back(sweep_at_value);
      }


    UpdateRecoveryMap();
  }
  
  void RecoveryMapTask::UpdateRecoveryMap() {
    auto& sim_results = task_params_.sim_results;
    if (sim_results.steps.x() == 0)
      return;

    auto [i_count, j_count] = sim_results.steps;

    std::vector<double> sweep_at_value_arr;
    for (auto j = 0; j < j_count; ++j)
      for (auto i = 0; i < i_count; ++i) {
        auto& point_data = sim_results.points_data[i + j * i_count];

        double sweep_at_value;

        if (task_params_.type == 0) {
          sweep_at_value = dpl::lerp_solve(point_data.sweep, task_params_.at_pvi); // TODO. sweep does not start with (0, 0)
          sweep_at_value = std::clamp<double>(sweep_at_value, 0, 1)*100.0;
        }
        else if (task_params_.type == 1) {
          auto time_coef = sim_results.pore_volume / point_data.total_flow_rate;
          sweep_at_value = dpl::lerp_solve(point_data.sweep, task_params_.at_time/time_coef); // TODO. sweep does not start with (0, 0)
          sweep_at_value = std::clamp<double>(sweep_at_value, 0, 1)*100.0;
        }
        else
          sweep_at_value = point_data.total_flow_rate /*/ units::Barrel(1e3)*/ * units::Day();

        sweep_at_value_arr.push_back(sweep_at_value);
      }


    double fill_contour_height = 0.1;
    double isoline_height = 0.5;

    {
      vtkNew<vtkContourFilter> contours;
      vtkNew<vtkPlaneSource> plane;

      plane->SetOrigin(sim_results.x0, sim_results.y0, sim_results.z);
      plane->SetPoint1(sim_results.x0 + sim_results.wx, sim_results.y0, sim_results.z);
      plane->SetPoint2(sim_results.x0, sim_results.y0 + sim_results.wy, sim_results.z);

      plane->SetXResolution(i_count - 1);
      plane->SetYResolution(j_count - 1);
      plane->Update();

      vtkNew<vtkDoubleArray> plane_scalars;
      plane_scalars->SetArray(sweep_at_value_arr.data(), sweep_at_value_arr.size(), true);
      plane->GetOutput()->GetPointData()->SetScalars(plane_scalars);

      int number_of_contours = 10;
      auto [min_iter, max_iter] = std::minmax_element(sweep_at_value_arr.begin(), sweep_at_value_arr.end());
      auto min = *min_iter, max = *max_iter;
      if (task_params_.type != 2) {
        min = std::clamp<double>(min, 0, 1 * 100.0);
        max = std::clamp<double>(max, 0, 1 * 100.0);
      }

      contours->SetInputData(plane->GetOutput());
      contours->GenerateValues(number_of_contours, min, max);

      // contours->GenerateValues(35, 0.0, 1.0);
      // auto pointThreshold = 0;

      vtkNew<vtkStripper> contourStripper;
      contourStripper->SetInputConnection(contours->GetOutputPort());
      contourStripper->Update();

      auto points = contourStripper->GetOutput()->GetPoints();
      auto cells = contourStripper->GetOutput()->GetLines();
      auto scalars = contourStripper->GetOutput()->GetPointData()->GetScalars();


      vtkNew<vtkPolyData> labelPolyData;
      vtkNew<vtkPoints> labelPoints;
      vtkNew<vtkDoubleArray> labelScalars;
      labelScalars->SetNumberOfComponents(1);
      labelScalars->SetName("Isovalues");

#if (VTK_MAJOR_VERSION == 8)
      vtkIdType* indices;
#elif (VTK_MAJOR_VERSION == 9)
      const vtkIdType *cindices;
      const vtkIdType *& indices = cindices;
#endif
      
      vtkIdType numberOfPoints;
      for (cells->InitTraversal(); cells->GetNextCell(numberOfPoints, indices);) {

        vtkIdType midPointId = indices[numberOfPoints / 2];

        double midPoint[3];
        points->GetPoint(midPointId, midPoint);
        midPoint[2] += isoline_height;
        labelPoints->InsertNextPoint(midPoint);
        labelScalars->InsertNextTuple1(scalars->GetTuple1(midPointId));
      }

      labelPolyData->SetPoints(labelPoints);
      labelPolyData->GetPointData()->SetScalars(labelScalars);

      vtkNew<vtkPolyDataMapper> contourMapper;
      contourMapper->SetInputConnection(contourStripper->GetOutputPort());
      contourMapper->ScalarVisibilityOff();


      isolines_->SetMapper(contourMapper);


      {
        vtkNew<vtkBandedPolyDataContourFilter> filled_contour_filter;
        filled_contour_filter->SetInputData(plane->GetOutput());
        filled_contour_filter->GenerateValues(number_of_contours, min, max);

        vtkNew<vtkLookupTable> lut;
        PopulateLutRedWhiteBlue(lut);

        for (vtkIdType i = 0, count = lut->GetNumberOfTableValues(); i < count; ++i) {
          auto* ptr = lut->GetTableValue(i);
          auto color = QColor::fromRgbF(ptr[0], ptr[1], ptr[2]).darker(110);
          lut->SetTableValue(i, color.redF(), color.greenF(), color.blueF());
        }

        vtkNew<vtkPolyDataMapper> filled_contour_map;
        filled_contour_map->SetInputConnection(filled_contour_filter->GetOutputPort());
        filled_contour_map->SetScalarModeToUseCellData();
        filled_contour_map->SetScalarRange(0, number_of_contours - 1);
        filled_contour_map->SetLookupTable(lut);

        surface_fill_->SetMapper(filled_contour_map);
        fd_widget_->Renderer()->AddActor(surface_fill_);
      }


      vtkNew<vtkLabeledDataMapper> labelMapper;
      labelMapper->SetFieldDataName("Isovalues");

      labelMapper->SetInputData(labelPolyData);

      labelMapper->SetLabelModeToLabelScalars();
      auto format = "%" + task_params_.format;
      if (task_params_.type != 2)
        format += "%%";
      labelMapper->SetLabelFormat(format.c_str());
      
      labelMapper->GetLabelTextProperty()->SetFontSize(fd_widget_->RenderingSettings().visual.font_size*3.0/4);
      labelMapper->GetLabelTextProperty()->SetJustificationToCentered();
      labelMapper->GetLabelTextProperty()->SetVerticalJustificationToCentered();
      // labelMapper->
      // vtkNew<vtkActor2D> isolabels;
      isolabels_->SetMapper(labelMapper);


      //   cube_axes_actor_->SetScreenSize(rendering_settings_.font_size*3.0/4);
      // cube_axes_actor_->SetLabelOffset(rendering_settings_.font_size*6.0/4);

      // isolines_->GetProperty()->SetEdgeColor(dpl::vector3d{0});

      
      fd_widget_->Renderer()->AddActor(isolines_);
      fd_widget_->Renderer()->AddActor(isolabels_);


      surface_fill_->SetPosition(0, 0, fill_contour_height);
      isolines_->SetPosition(0, 0, isoline_height);

      // 0.01

      fd_widget_->Render();
    }
  }
}
