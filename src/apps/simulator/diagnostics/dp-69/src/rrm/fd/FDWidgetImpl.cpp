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

#include "rrm/fd/FDWidgetImpl.h"
#include "rrm/fd/FDQVTKWidget.h"
#include "rrm/fd/FDPropertyTreeItems.hpp"
#include "rrm/fd/FlowCalculations.hpp"
#include "rrm/fd/ImportExport.hpp"
#include "rrm/fd/core/QLayoutBuilder.hpp"

#include <QDesktopServices>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcess>
#include <QValueAxis>
#include <QScreen>
#include <QWindow>

#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkPNGWriter.h>
#include <vtkWindowToImageFilter.h>

#include <stack>




namespace rrm::fd
{
  void FDWidgetImpl::BeginInit() {
    filter_ro_array_->ConstructFunctor(model_);
    filter_ro_array_->SetName("filter_array");
    
    // auto scaled_font = QApplication::font();
    // scaled_font.setPointSize(8);

    
    qvtk_widget_ = new FDQVTKWidget;

    auto* plot_tabs = new QTabWidget;

    using namespace core::QLayoutBuilder;
    using std::tuple;
    
    {
      plot_tabs->addTab(lorenz_plot_.Init(this), "Lorenz");
      plot_tabs->addTab(sweep_plot_.Init(this/*, [this]{ ResetLorenzSweepFD(); }*/), "Recovery");

      // FlowDiagnosticsChanged += [this] { ResetLorenzSweepFD(); };

      plot_tabs->addTab(domain_partitioning_.Init(this), "Domains");
      plot_tabs->addTab(well_partitioning_.Init(this), "Wells");
    }


    // auto* property_tabs = new QTabWidget;

    
    // {
      // property_tabs->frame(QFrame::NoFrame);

      // {
      //   fd_property_tree_.BeginInit(this);
      //   property_tabs->addTab(fd_property_tree_.AcquireTreeView().release(), "Properties");
      // }

      // {
      //   recovery_map_task_.BeginInit(this);
      //   property_tabs->addTab(recovery_map_task_.AcquireWidget().release(), "Well optimisation");
      // }
    // }

    vsplit_ = new QSplitter{Qt::Vertical};
    {
      fd_property_tree_.BeginInit(this);
      vsplit_->addWidget(fd_property_tree_.AcquireTreeView().release());
    }
    vsplit_->addWidget(plot_tabs);

    vsplit_->setStretchFactor(0, 1);
    vsplit_->setStretchFactor(1, 0);

    hsplit_ = new QSplitter{Qt::Horizontal};
    hsplit_->addWidget(vsplit_);
    hsplit_->addWidget(qvtk_widget_);
    hsplit_->setStretchFactor(0, 0);
    hsplit_->setStretchFactor(1, 1);

    // hsplit_->setSizes({100, 1});
    // vsplit_->setSizes({1, 100});

    setCentralWidget(hsplit_);


#if (VTK_MAJOR_VERSION == 8)
    static_cast<FDQVTKWidget*>(qvtk_widget_)->SetRenderWindow(render_window_);
#elif (VTK_MAJOR_VERSION == 9)
    // qvtk_widget_->SetRenderWindow(render_window_);
    static_cast<FDQVTKWidget*>(qvtk_widget_)->setRenderWindow(render_window_);
    // render_window_->SetMultiSamples(32);
#endif

    
    // renderer_->SetUseFXAA(true);
    // renderer_->SSAOBlurOn();
    
    render_window_->AddRenderer(renderer_);
    interactor_ = render_window_->GetInteractor();


    
    // 
    // //renderWindow->use
    // // renderWindow->SetMultiSamples(8);
    // // renderWindow->SetLineSmoothing(false);
    //
    // renderWindow->SetSize(300, 300);
    // renderWindow->AddRenderer(renderer_);
    // renderWindow->SetWindowName("Cylinder");
    // renderWindowInteractor->SetRenderWindow(renderWindow);

    // renderer_->UseHiddenLineRemovalOff();


    using namespace QtCharts;

    auto scaled_font = QApplication::font();
    scaled_font.setPointSize(8);

    QFont font = scaled_font;
    font.setBold(false);

    QBrush black_brush{Qt::black};

    {
      
      // property_tabs->tabBar()->setFont(scaled_font);
      
      plot_tabs->setFont(scaled_font);
      // plot_tabs->setFont(font_def);
      // property_tabs->setFont(font_def);
      // tree_view_->setFont(font_copy);
    }
  }


  void FDWidgetImpl::EndInit() {
    InitMainMenu();
    
    setWindowTitle(
      rrm_project_path.empty()
        ? "Flow Diagnostics"
        : QString::fromStdString("Flow Diagnostics - " + rrm_project_path.filename().string()));
    
    setWindowIcon(QIcon(
      QString::fromStdString((std::filesystem::current_path() / "_config/droplet.png").string())
    )); 


    // { // title
    //   
    //   // title_mapper_->SetInput("Title text");
    //   title_mapper_->GetTextProperty()->SetFontFamily(VTK_FONT_FILE);
    //   title_mapper_->GetTextProperty()->SetFontFile(rendering_settings_.font_path.string().c_str());
    //
    //   auto* text_property = title_mapper_->GetTextProperty();
    //   text_property->SetColor(dpl::vector3d{0});
    //   text_property->SetFontFamily(VTK_FONT_FILE);
    //   text_property->SetFontFile(rendering_settings_.font_path.string().c_str());
    //   text_property->SetFontSize(1.25*rendering_settings_.visual.font_size);
    //   text_property->SetJustificationToCentered();
    //   text_property->SetVerticalJustificationToTop();
    //
    //   if (rendering_settings_.publishing) {
    //     // text_property->SetBackgroundColor(dpl::vector3d{0.9});
    //     // text_property->SetBackgroundOpacity(1.0);
    //   }
    //   else {
    //     text_property->SetBackgroundColor(dpl::vector3d{1});
    //     text_property->SetBackgroundOpacity(VisualVerticalWell::DefaultTextOpacity);
    //   }
    //
    //   vtkNew<vtkActor2D> caption;
    //   caption->SetMapper(title_mapper_);
    //   caption->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
    //   caption->GetPositionCoordinate()->SetValue(0.5, 0.95);
    //
    //   renderer_->AddActor2D(caption);
    // }

    
    renderer_->SetBackground(dpl::vector3d{1.0});

    hsplit_->setSizes({static_cast<int>(/*275*/300*rendering_settings_.visual.ScaleFactor()), 1});
    vsplit_->setSizes({1, 300});

    InitModel();

    InitImageData();

    volumetrics_.Init(this);

    InitDisplayProperties();

    ScaleChanged += [this] {
      for (auto& w : VisualWells())
        w.SetScale(scale_, VisualVerticalWell::VisualRadius(model_, scale_));
    };
    
    AddVisualWells();
    SetActiveVisualWell(*visual_wells_.begin());

    boundary_cond_.values = rendering_settings_.bond_cond;
    if (rendering_settings_.wells)
      LoadWells(*rendering_settings_.wells);

    InitScalarBar();
    
    {
      axes_.tidy.Init(renderer_.Get(), rendering_settings_, scale_);
      ScaleChanged += [this] { Axes().tidy.SetScale(Scale()); };
    }

    #if (VTK_MAJOR_VERSION == 8)
      renderer_->RemoveActor2D(scalar_bar_);
      renderer_->AddActor2D(scalar_bar_);    
    #endif
    
    InitCompass();

    
    fd_property_tree_.EndInit();

    volumetrics_.InitPropertyTree();


    // recovery_map_task_.EndInit();

    connect(
      static_cast<FDQVTKWidget*>(qvtk_widget_),
      &FDQVTKWidget::resized, this, &FDWidgetImpl::ViewportResized);


    static_cast<FDQVTKWidget*>(qvtk_widget_)->Init(this, &volumetrics_);
    
    EvaluateFlowDiagnostics(true);
    // UpdateImageDataAndThreshold();
    // image_data_->Modified();
    // threshold_->Modified();
    // threshold_->Update();

    // domain_partitioning_.Update();
    // well_partitioning_.Update();    
    
    


    {
      auto& dp_name = rendering_settings_.visual.display_property;
      
      if (!dp_name.empty()) {
        if (auto* found =
          cell_info_.first<DisplayCellInfo>([dp_name](const DisplayCellInfo& cd) {
            return cd.vtk_field_name == dp_name;
          }))
          found->SetActive();
      }
      else
        cell_info_[PRESSURE].SetActive();
    }
    
    

    if (surfaces_actor_) {
      for (auto* a : dpl::range<vtkActor>(surfaces_actor_)) {
        a->SetScale(scale_);
        // prop->PickableOff();
        a->VisibilityOff();
      }

      renderer_->AddActor(surfaces_actor_);
      surfaces_actor_->VisibilityOff();
      // surfaces_actor_->PickableOff();

      ScaleChanged += [this] {
        for (auto* a : dpl::range<vtkActor>(surfaces_actor_))
          a->SetScale(scale_);
      };
    }


    if (rendering_settings_.camera) {
      *rendering_settings_.camera >> renderer_->GetActiveCamera();
      renderer_->ResetCameraClippingRange();
    }
    else {
      renderer_->ResetCamera();
      auto* camera = renderer_->GetActiveCamera();
      camera->Elevation(-90);
      camera->OrthogonalizeViewUp();
      renderer_->ResetCamera();
      camera->SetParallelProjection(false);
      renderer_->GetActiveCamera()->SetParallelScale(model_.PhysicalSize().z()*0.9);
      // renderer_->SetNearClippingPlaneTolerance(0.1);
      // renderer_->ResetCameraClippingRange();
    }




    ScaleChanged();
    

    RefreshCubeAxesBounds();

    ViewportResized();


    PetrophysicsChanged.add([this] {
      UpdateModelPoroPerm();
    }, -1);

    PetrophysicsChanged.add([this](model_event_args e) {
      if (e.porosity) {
        cell_info_[POROSITY].DataArray()->Modified();
        cell_info_[POROSITY].TryUpdateDefaultLutRange();
      }

      if (e.permeability) {
        cell_info_[PERM_X].DataArray()->Modified();
        cell_info_[PERM_X].TryUpdateDefaultLutRange();

        cell_info_[PERM_Y].DataArray()->Modified();
        cell_info_[PERM_Y].TryUpdateDefaultLutRange();
        
        cell_info_[PERM_Z].DataArray()->Modified();
        cell_info_[PERM_Z].TryUpdateDefaultLutRange();
      }
    }, 0);

    PetrophysicsChanged.add([this](model_event_args e) {
      if (e.porosity)
        volumetrics_.RefreshVolumes();
    }, 0);

    PetrophysicsChanged.add([this](model_event_args e) {
      UpdateFull(e.permeability);
    }, 1);
    
    
    // UpdateModelPoroPerm();
    //         
    //         poro_property_.DataArray()->Modified();
    //         if (poro_property_.use_default_range)
    //           poro_property_.Lut()->SetTableRange(poro_property_.GetDefaultRange());
    //
    //         RefreshFFContactVolumes();
    //         
    //         UpdateFull(false);
    
    // Render();
  }

  
  

  // TODO - convoluted method, needs refactoring
  // void FDWidgetImpl::ResetLorenzSweepFD() {
  //   FlowSolver::EvaluateDiagnosticsCurves(model_, flow_summary_);
  //
  //   // 1 m3/s = 543439.65056533 bbl/day 
  //
  //   flow_summary_.total_inj_flow_rate = 0;
  //   for (auto [_, wr] : well_results_)
  //     if (!wr.IsProducer())
  //       flow_summary_.total_inj_flow_rate += wr.flow_rate;
  //
  //   for (auto flow_rate : face_flow_rate_)
  //     if (flow_rate > 0)
  //       flow_summary_.total_inj_flow_rate += flow_rate;
  //
  //
  //   { // Plots
  //     const auto time_coef = flow_summary_.TimeInPVI()/units::Year();
  //     const auto x_coef = sweep_plot_.CheckedPVI() ? 1.0 : time_coef;
  //
  //     constexpr int steps = 100;
  //
  //     lorenz_plot_.LineSeries()->clear();
  //     lorenz_plot_.LineSeries()->append(0, 0);
  //
  //     sweep_plot_.LineSeries()->clear();
  //     sweep_plot_.LineSeries()->append(0, 0);
  //
  //     auto cell_count = model_.CellCount();
  //
  //     sweep_plot_.LineSeries()->append(x_coef*flow_summary_.sweep[1].x(), flow_summary_.sweep[1].y());
  //     for (auto i = 0; i < steps; ++i) {
  //       const int j = (i + 1.0)/steps*(cell_count - 1);
  //       lorenz_plot_.LineSeries()->append(flow_summary_.lorenz[j + 1].x(), flow_summary_.lorenz[j + 1].y());
  //
  //       if (!std::isinf(flow_summary_.sweep[j + 1].x()))
  //         sweep_plot_.LineSeries()->append(x_coef*flow_summary_.sweep[j + 1].x(), flow_summary_.sweep[j + 1].y());
  //     }
  //   }
  // }

  
  
  void FDWidgetImpl::SaveScreenshot() {
    std::filesystem::create_directories("_screenshots");

    if (screenshot_number_ == 0) {
      std::regex reg("screenshot_(\\d+)\\.png", std::regex_constants::icase);
      std::smatch match;

      
      for (auto& entry : std::filesystem::directory_iterator("_screenshots")) {
        auto stem = entry.path().filename().string();

        if (std::regex_search(stem, match, reg))
          screenshot_number_ = std::max(screenshot_number_, std::stoi(match.str(1)));
      }
    }

    vtkNew<vtkWindowToImageFilter> filter;

    filter->SetInput(render_window_);
    filter->SetInputBufferTypeToRGB(); //also record the alpha (transparency) channel
    filter->ReadFrontBufferOff(); // read from the back buffer
    filter->Update();

    vtkNew<vtkPNGWriter> writer;

    writer->SetFileName((boost::format("_screenshots\\screenshot_%03i.png") % ++screenshot_number_).str().c_str());
    writer->SetInputConnection(filter->GetOutputPort());
    writer->Write();
  }
  
  void FDWidgetImpl::ExportWellsDialog() {
    auto filename = QFileDialog::getSaveFileName(
      this, "Export Wells", "", "JSON (*.json);;JSON (*.wells.json);;All Files (*.*)");

    if (!filename.isEmpty()) {
      nlohmann::json j;

      
      auto& wells = j["wells"];
      
      for (auto& vw : VisualWells())
        wells.push_back(vw.input->to_json());

      auto& bc = j["boundary_conditions"];

      for (auto& v : boundary_cond_.values)
        bc.push_back(v ? *v : std::numeric_limits<double>::quiet_NaN());

      std::ofstream(filename.toStdString()) << j.dump(2);
    }
  }

  void FDWidgetImpl::ExportGridDialog() {
    auto filename = QFileDialog::getSaveFileName(this, "Save Eclipse Grid", "",
        "Eclipse ASCII Grid (*.grdecl);;All files (*.*)");

    if (!filename.isEmpty())
      ExportGRDECL(model_, filename.toStdString());
  }

  void FDWidgetImpl::LoadWellsDialog() {
    auto filename = QFileDialog::getOpenFileName(
        this, "Load Wells", "", "JSON (*.json);;JSON (*.wells.json);;All Files (*.*)");

    if (!filename.isEmpty()) {
      LoadWells(nlohmann::json::parse(std::ifstream(filename.toStdString())));
      UpdateFull(false);
    }
  }

  void FDWidgetImpl::LoadWells(const nlohmann::json& j) {
    // auto j = nlohmann::json::parse(std::ifstream(filename));

    for (auto& vw : visual_wells_) {
      renderer_->RemoveActor2D(vw.caption_actor);
      renderer_->RemoveActor(vw.above_perf_actor);
      renderer_->RemoveActor(vw.perf_actor);
    }

    wells_.clear();
    visual_wells_.clear();

    active_well_ = nullptr;


    if (j.is_array()) {
      for (auto& x : j) 
      wells_.emplace_back().from_json(x);
    }
    else {
      for (auto& x : j["wells"]) 
        wells_.emplace_back().from_json(x);

      if (j.contains("boundary_conditions")) {
        auto& bc = j["boundary_conditions"];
        
        for (int i = 0; i < 6; ++i)
          if (bc[i].is_null())
            boundary_cond_.values[i].reset();
          else
            boundary_cond_.values[i] = bc[i].get<dpl::units::pascal>();
      }
    }

    

    cell_info_[PRESSURE].TryUpdateDefaultLutRange();

    AddVisualWells();
    SetActiveVisualWell(*visual_wells_.begin());
  }
  
  void FDWidgetImpl::InitModel() {
    model_.SetLocation({100, 150, 200});
    model_.SetPhysicalSize({750, 600, 500}); //.Set(/*400, 200, 160*/500,500,500, units::Metres());


    if (rendering_settings_.grid.resolution)
      model_.SetDimensions(*rendering_settings_.grid.resolution);
    else
      model_.SetDimensions({/*16, 8, 16*/ /*35,35,35*/ /*11,11,11*/ 21, 21, 21});

    // if constexpr (direction_pressure_field) {
    //   pressure_directional_prop_[0].data.assign(model_.CellCount(), 0);
    //   pressure_directional_prop_[1].data.assign(model_.CellCount(), 0);
    //   pressure_directional_prop_[2].data.assign(model_.CellCount(), 0);
    // }


    model_.viscosity = rendering_settings_.viscosity;

    if (stratigraphy_model_) {
      dpl::vector3d julio_origin;
      dpl::vector3d julio_size;
      stratigraphy_model_->useDefaultCoordinateSystem();
      stratigraphy_model_->getOrigin(julio_origin.x(), julio_origin.y(), julio_origin.z());
      stratigraphy_model_->getSize(julio_size.x(), julio_size.y(), julio_size.z());
      julio_size = julio_size*rendering_settings_.stratmod.scale;
      // julio_size.z() *= rendering_settings_.stratmod.scale.z();
      stratigraphy_model_->useOpenGLCoordinateSystem();

      if (surfaces_actor_/*  min_z_ != max_z_*/) {
        auto* bounds = surfaces_actor_->GetBounds();
        model_.SetLocation({julio_origin.x(), julio_origin.y(), bounds[4]});
        model_.SetPhysicalSize({julio_size.x(), julio_size.y(), bounds[5] - bounds[4]});
      }
      else {
        model_.SetLocation(julio_origin);
        model_.SetPhysicalSize(julio_size);
      }
    }


    if (rendering_settings_.grid.location)
      model_.SetLocation(*rendering_settings_.grid.location);

    if (rendering_settings_.grid.size)
      model_.SetPhysicalSize(*rendering_settings_.grid.size);


    RequestRegions();

    UpdateModelPoroPerm();

    {
      auto& w = wells_.emplace_back();

      w.head_location = {
        model_.MinX() + 250,
        model_.MinY() + 250,
        model_.MaxZ() + Overhead
      };

      w.perf_top = Overhead + 0.01;
      w.perf_thickness = 1.0 * model_.PhysicalSize().z() - 0.02;

      // w.name = "�����";
      w.name = "Well 1";
      w.pressure = units::Bar(500);
      w.wellbore_radius = units::Feet(0.3);
    }

    {
      auto& w = wells_.emplace_back();

      w.head_location = {
        model_.MinX(),
        model_.MinY(),
        model_.MaxZ() + Overhead
      };

      w.perf_top = Overhead + 0.01;
      w.perf_thickness = 0.8 * model_.PhysicalSize().z() - 0.02;

      w.name = "Well 2";
      w.pressure = units::Bar(300);
      w.wellbore_radius = units::Feet(0.3);
    }

    {
      auto& w = wells_.emplace_back();

      w.head_location = {
        model_.MaxX(),
        model_.MinY(),
        model_.MaxZ() + Overhead
      };

      w.perf_top = Overhead + 0.01;
      w.perf_thickness = 1.0 * model_.PhysicalSize().z() - 0.02;

      w.name = "Well 3";
      w.pressure = units::Bar(300);
      w.wellbore_radius = units::Feet(0.3);
    }

    {
      auto& w = wells_.emplace_back();

      w.head_location = {
        model_.MaxX(),
        model_.MaxY(),
        model_.MaxZ() + Overhead
      };

      w.perf_top = Overhead + 0.01;
      w.perf_thickness = 1.0 * model_.PhysicalSize().z() - 0.02;

      w.name = "Well 4";
      w.pressure = units::Bar(300);
      w.wellbore_radius = units::Feet(0.3);
    }
  }
  
  void FDWidgetImpl::InitCompass() {
    class vtkAxesActorExt : public vtkAxesActor
    {
      const double def_bound_ = 0.6;
      double bounds_[6] = {-def_bound_, def_bound_, -def_bound_, def_bound_, -def_bound_, def_bound_};

    public:
      static vtkAxesActorExt* New() { return new vtkAxesActorExt; }

      double* GetBounds() override {
        return bounds_;
      }
    };

    vtkNew<vtkAxesActorExt> display_axes_actor;

    dpl::vector3d black{0};
    display_axes_actor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(black);
    display_axes_actor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(black);
    display_axes_actor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(black);


    display_axes_actor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
    display_axes_actor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
    display_axes_actor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();

    vtkNew<vtkNamedColors> colors;
    orientation_market_widget_->SetOrientationMarker(display_axes_actor);

    orientation_market_widget_->SetInteractor(interactor_);

    orientation_market_widget_->SetEnabled(true);
    orientation_market_widget_->InteractiveOff();

    display_axes_actor->VisibilityOff();
  }
  
  void FDWidgetImpl::InitMainMenu() {
    // menuBar()->hide();
    
    {
      auto* cat = menuBar()->addMenu("&File");

      connect(cat->addAction("&Exit"), &QAction::triggered, [this] { close(); });
    }

    {
      auto* cat = menuBar()->addMenu("&Edit");

      connect(cat->addAction("&Copy Camera"), &QAction::triggered, [this] {
        QApplication::clipboard()->setText(QString::fromStdString(
          nlohmann::json(CameraSettings{renderer_->GetActiveCamera()}).dump(2)));
      });

      connect(cat->addAction("&Paste Camera"), &QAction::triggered, [this] {
        try {
          CameraSettings{
            nlohmann::json::parse(QApplication::clipboard()->text().toStdString())
          } >> renderer_->GetActiveCamera();
          renderer_->ResetCameraClippingRange();
          Render();
        }
        catch (...) {}
      });

      cat->addSeparator();
      
      connect(cat->addAction("&Save Screenshot"), &QAction::triggered, [this] { SaveScreenshot(); });

      connect(cat->addAction("&Open Screenshot Folder"), &QAction::triggered, [this] {
        QDesktopServices::openUrl(QUrl::fromLocalFile("_screenshots"));
      });

      cat->addSeparator();
      
      connect(cat->addAction("Default Settings File"), &QAction::triggered, [this] {
        QDesktopServices::openUrl(QUrl::fromLocalFile("_config/startup.json"));
      });
      
      if (!rrm_project_path.empty()) {
        connect(cat->addAction("Project Settings File"), &QAction::triggered, [this] {
          auto path = rrm_project_path;
          path.replace_extension("startup.json");

          // ReSharper disable once CppRedundantQualifier
          if (!std::filesystem::exists(path))
            std::ofstream(path) << nlohmann::json{}.dump(2);

          QDesktopServices::openUrl(
            QUrl::fromLocalFile(
              QString::fromStdString(path.string())
            ));
          
        });
      }
    }
    

    {
      auto* cat = menuBar()->addMenu("&Wells");

      connect(cat->addAction("&Load..."), &QAction::triggered, [this] { LoadWellsDialog(); });
      connect(cat->addAction("&Export..."), &QAction::triggered, [this] { ExportWellsDialog(); });
    }

    {
      auto* cat = menuBar()->addMenu("&Grid");

      auto* widget = static_cast<FDQVTKWidget*>(qvtk_widget_);
      
      auto* action_cell_display = cat->addAction("Cell detective");

      action_cell_display->setCheckable(true);
      action_cell_display->setChecked(widget->MouseOverShow());
      
      connect(action_cell_display, &QAction::triggered, [widget, action_cell_display, this] {
        widget->SetMouseOverShow(!widget->MouseOverShow());
        action_cell_display->setChecked(widget->MouseOverShow());
      });

      connect(cat->addAction("&Export GRDECL..."), &QAction::triggered, [this] { ExportGridDialog(); });
    }

    {
      auto* cat = menuBar()->addMenu("&Petrophysics");

      connect(cat->addAction("Default File"), &QAction::triggered, [this] {
        QDesktopServices::openUrl(QUrl::fromLocalFile("_config/fdsettings.json"));
      });

      if (!rrm_project_path.empty()) {
        connect(cat->addAction("Project File"), &QAction::triggered, [this] {
          auto path = rrm_project_path;
          path.replace_extension("fdsettings.json");

          // ReSharper disable once CppRedundantQualifier
          if (!std::filesystem::exists(path))
            std::ofstream(path) << nlohmann::json{
              DomainSettings{"_YOUR_DOMAIN_NAME_", QColor::fromRgb(120, 100, 80), 0.1, units::MilliDarcy(100)}
            }.dump(2);

          QDesktopServices::openUrl(
            QUrl::fromLocalFile(
              QString::fromStdString(path.string())
            ));
          
        });
      }

      cat->addSeparator();
      
      
      connect(cat->addAction("Save Default"), &QAction::triggered, [this] {
        nlohmann::json j;

        std::map<std::string, DomainSettings> global_map;

        try {
          LoadSettings(global_map, nlohmann::json::parse(std::ifstream("_config/fdsettings.json")));
        }
        catch (...) {}
        
        for (auto& [_, d] : petrophysics_.domains) {
          std::string name = d.name;
          boost::algorithm::to_lower(name);
          global_map[name] = d;
        }

        for (auto& [_, d] : global_map)
          j.push_back(d);

        std::ofstream("_config/fdsettings.json") << j.dump(2);
      });


      if (!rrm_project_path.empty()) {
        connect(cat->addAction("Save Project"), &QAction::triggered, [this] {
          nlohmann::json j;
        
          for (auto& [_, d] : petrophysics_.domains)
            j.push_back(d);
          
          std::ofstream(
            std::filesystem::path(rrm_project_path).replace_extension("fdsettings.json")) << j.dump(2);
        });
      }
    }

    {
      auto* cat = menuBar()->addMenu("&Report");

      connect(cat->addAction("&Export..."), &QAction::triggered, [this] {
        auto filename = QFileDialog::getSaveFileName(
          this, "Export Report", "", "CSV (Comma delimited) (*.csv);;All Files (*.*)");

        if (!filename.isEmpty()) {
          QtCharts::QPieSeries* inj;
          QtCharts::QPieSeries* prod;

          std::unique_ptr<QtCharts::QPieSeries> other;

          if (well_partitioning_.Producers()) {
            prod = well_partitioning_.PieSeries();
            other.reset(component::CreateTracerPieSeries(this, false));
            inj = other.get();
          }
          else {
            inj = well_partitioning_.PieSeries();
            other.reset(component::CreateTracerPieSeries(this, true));
            prod = other.get();
          }

          ExportReport(
            wells_,
            flow_summary_.well_results,
            inj, prod,
            lorenz_plot_.LineSeries(), sweep_plot_.LineSeries(),
            fd_property_tree_.SummaryCategory(),
            flow_summary_.lorenz_coef, flow_summary_.upscaled_perm, filename.toStdString());
        }
      });
      
    }
    
    {
      auto* cat = menuBar()->addMenu("&Help");

      connect(cat->addAction("&About"), &QAction::triggered, [this] {
        QMessageBox msg{this};

        msg.setWindowTitle("About Flow Diagnostics");
        
        msg.setTextFormat(Qt::RichText);
        msg.setText(QString(R"(
          <style>
            p {
              margin: 0;
              padding: 0;
            }
            
            p.author {
              text-indent: %1;
            }
          </style>

          <p>Flow Diagnostics is part of Rapid Reservoir Modelling Software</p>
          <p>Version %2</p>  

          <br/>
          <center>
            <p><a href="https://rapidreservoir.org/">Website</a></p>
            <p><a href="https://bitbucket.org/rapidreservoirmodelling/rrm/">Source code</a></p>
          </center>
          <br/>
          <p>Delft University of Technology, Netherlands</p>
          <p>Heriot-Watt University, United Kingdom</p>
          <p class="author">Dmytro Petrovskyy, Sebastian Geiger</p>
          <br/>
          <p>Imperial College London, United Kingdom</p>
          <p class="author">Carl Jacquemyn, Matthew Jackson, Gary Hampson</p>
          <br/>
          <p>University of Calgary, Canada</p>
          <p class="author">Julio Machado Silva, Sicilia Judice, Fazilatur Rahman, Mario Costa Sousa</p>)")
        .arg("14px")
        .arg("2.1.20822"));

        
        msg.setIconPixmap(this->windowIcon().pixmap(this->windowIcon().availableSizes().first()));
        msg.setWindowFlags(
          Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint/* | Qt::WindowCloseButtonHint*/);

        msg.exec();
      });
    }
  }

  void FDWidgetImpl::changeEvent(QEvent* event) {
    if (!initialised_after_shown_ && event->type() == QEvent::ActivationChange/* && event->spontaneous()*/) {
    // vsplit_->setSizes({1, 100});
      initialised_after_shown_ = true;
      
      if (rendering_settings_.visual.viewport) {
        hsplit_->setSizes({200, 1});
        
        ViewportItem{this}.Set(*rendering_settings_.visual.viewport);
        setGeometry(QStyle::alignedRect(
          Qt::LeftToRight, Qt::AlignCenter, size(), windowHandle()->screen()->availableGeometry()));
        move(std::max(0, x()), std::max(0, y()));
      }
      else {
        hsplit_->setSizes({300, 1});
      }
    }
  }
  
  void FDWidgetImpl::keyPressEvent(QKeyEvent* e) {
    if (e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) && e->key() == Qt::Key_P)
      SaveScreenshot();
    else if (e->key() == Qt::Key_Delete && active_well_ && visual_wells_.size() > 1)
      RemoveActiveWell();
  }
}
