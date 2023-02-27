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

#include "rrm/fd/FDQVTKWidget.h"
#include "rrm/fd/FDWidgetImpl.h"

#include <QApplication>
#include <QScreen>
#include <QSurfaceFormat>

#include <nlohmann/json.hpp>

#include <stratmod/sutilities.hpp>

// auto boo(auto a, auto b) { return a + b; }

vtkSmartPointer<vtkPropAssembly> CreateSurfacesActor(SModeller& strat_model, dpl::vector3d scale);

int main(int argc, char* argv[]) {
  using namespace rrm::fd;
  using nlohmann::json;
  using std::filesystem::path;
  
  path rrm_filename = argc > 1
    ? path(std::string(argv[1]))
    : path(argv[0]).replace_filename("_config/test_model.rrm");

  // rrm_filename = R"(C:\_TEST\shallow_marine_parasequences\SM1\SM1_initialinterp.rrm)";
  // rrm_filename = R"(C:\_TEST\cube\model.rrm)";

  StartupSettings startup_sets;
  
  startup_sets.font_path = GetWindowsUnicodeArialFontPath();

  startup_sets.TryParseConfig();  
  startup_sets.TryParseConfig(path(rrm_filename).replace_extension("startup.json"));
  
  TimestepRecords time_steps{"stratmod model"};
  std::unique_ptr<SModeller> stratmod;
  stratmod = std::make_unique<SModeller>();


  if (stratmod) {
    if (stratmod->loadJSON(rrm_filename.string()))
      time_steps << "JSON load" >> std::cout << endl;
    else {
      std::cerr << "::: Error ::: Could not find .rrm.json file.\n\n" << std::flush;
      return 1;
    }
  }


  PetrophysicalSettings petrophysics;
  std::map<int, std::vector<int>> domain_to_regions;

  int region_count = -1;
  {
    json j;
    try {
      j = json::parse(std::ifstream(path(rrm_filename).replace_extension("rrm.json")));
    }
    catch (...) {
      std::cout << "::: Warning ::: Could not find .rrm.json file.\n\n" << std::flush;
    }

    auto dcount = j["domains"].size();
    dcount = dcount > 0 ? dcount : 1;
    for (auto& domain : j["domains"]) {
      int didx = domain["index"];
      
      for (int ridx : domain["regions"]) {
        petrophysics.region_to_domain[ridx] = didx;
        domain_to_regions[didx].push_back(ridx);
      }

      auto& dset = petrophysics.domains[didx];
      dset.name = domain.contains("name") ? domain["name"].get<std::string>() : "Domain" + std::to_string(didx);
      dset.color = QColor::fromHslF(0.7*didx/dcount, 0.65, 0.45);  // NOLINT(clang-diagnostic-implicit-int-float-conversion, cppcoreguidelines-narrowing-conversions, bugprone-narrowing-conversions)
      dset.perm = units::MilliDarcy(999.99);
      dset.poro = 0.2999;
    }

    if (!j["regions"].empty()) {
      region_count = j["regions"].size() - 1;  // NOLINT(clang-diagnostic-shorten-64-to-32)
      for (auto& region : j["regions"])
        try {
          if (region.at("index").get<int>() > region_count)
            region_count += 1;
        }
        catch (...) {}
      region_count += 1;
    }
    else {
      stratmod::SUtilities u(*stratmod);
      std::vector<double> v;
      std::vector<std::vector<std::size_t>> regions;
      u.getTetrahedralMesh(v, regions);
      region_count = regions.size();  // NOLINT(clang-diagnostic-shorten-64-to-32, bugprone-narrowing-conversions)
    }
  }

  
  
  {


    
    try {
      LoadSettings(startup_sets.domains, json::parse(std::ifstream(path(rrm_filename).replace_extension("fdsettings.json"))));
    }
    catch (...) {
      std::cout << "::: Status ::: Could not find _config/test_model.fdsettings.json file.\n\n" << std::flush;
    }

    for (auto& [_, dset] : petrophysics.domains) {
      auto original_name = dset.name;
      if (auto iter = startup_sets.domains.find(boost::algorithm::to_lower_copy(original_name));
        iter != startup_sets.domains.end()) {
        dset = iter->second;
        dset.name = original_name;
      }
    }
    
    int outer_domain_idx = petrophysics.domains.size();  // NOLINT(clang-diagnostic-shorten-64-to-32)
    auto& outer_settings = petrophysics.domains[outer_domain_idx];
    outer_settings.poro = 0.001;
    outer_settings.name = "<Outer>";
    outer_settings.perm = units::MilliDarcy(0.001);
    outer_settings.color = QColor::fromRgbF(0.4, 0.4, 0.4);

    petrophysics.region_to_domain[petrophysics.region_to_domain.size()] = outer_domain_idx;  // NOLINT(clang-diagnostic-shorten-64-to-32, bugprone-narrowing-conversions)
  }

  {
    /* int region_count = petrophysics.region_to_domain.size() - 1; */
    
    const auto start = 0.0;
    const auto step = 0.85/(region_count - 1);

    for (int i = 0; i < region_count; ++i) {
      petrophysics.regions[i] = {"Region " + std::to_string(i), QColor::fromHslF(start + step*i, 0.5, 0.5)};

      auto& region = petrophysics.regions[i];
      int reg_idx = i;
        // Won't save metadata for regions ignored by the user
        if (petrophysics.region_to_domain.find(reg_idx) == petrophysics.region_to_domain.end())
        {
            auto domain_idx = petrophysics.domains.size();
            petrophysics.region_to_domain[reg_idx] = domain_idx;

            auto& domain_settings = petrophysics.domains[domain_idx];

            domain_settings.name = region.name;
            domain_settings.perm = units::MilliDarcy(999.99);
            domain_settings.poro = 0.2999;
            domain_settings.color = region.color;
        }
    }
  }

  {
    json j;
    try {
      j = json::parse(std::ifstream(path(rrm_filename).replace_extension("rrm.json")));
    }
    catch (...) {
    }

    for (auto& region : j["regions"])
    {
      int r, g, b;
      try {
        int i = region.at("index");
        r = region.at("red");
        g = region.at("green");
        b = region.at("blue");
        auto& region_settings = petrophysics.regions.at(i);
        region_settings.color = QColor::fromRgb(r, g, b);
      }
      catch (...) {
          std::cout << "\nCould not proccess region: " << region.dump() << std::endl;
      }
    }
  }

  
  // for (auto& [d_idx, regions] : domain_to_regions)
  //   if (!regions.empty()) {
  //     auto& d_sets = petrophysics.domains[d_idx];
  //     int r_idx = regions.front();
  //     auto& r_sets = petrophysics.regions[r_idx];
  //     d_sets.color = r_sets.color;
  //   }






  auto format = FDQVTKWidget::defaultFormat();

#ifdef _WIN32
  format.setProfile(QSurfaceFormat::CompatibilityProfile);
#else
  format.setProfile(QSurfaceFormat::CoreProfile);
#endif
  

  QSurfaceFormat::setDefaultFormat(format);
  
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, false);
  
  QApplication app(argc, argv);  

  



  startup_sets.stratmod.y_invert = true;
  
  {
    stratmod->useDefaultCoordinateSystem();
    
    dpl::vector3d stratmod_size;
    stratmod->getSize(stratmod_size.x(), stratmod_size.y(), stratmod_size.z());
      
    if (startup_sets.stratmod.override_size)
      startup_sets.stratmod.scale = *startup_sets.stratmod.override_size/stratmod_size;

    stratmod->useOpenGLCoordinateSystem();
  }
  
  
  if (!startup_sets.visual.font_size)
    startup_sets.visual.font_size =
      startup_sets.visual.nominal_size +
        (QApplication::primaryScreen()->logicalDotsPerInch()/96. - 1.)*10.;
  
  FDWidgetImpl fd_widget;
  fd_widget.BeginInit();
   
  if (stratmod) {
    fd_widget.rrm_project_path = rrm_filename;
    fd_widget.SetStratigraphyModel(*stratmod);
    fd_widget.SetSurfacesActor(CreateSurfacesActor(*stratmod, startup_sets.stratmod.scale));
  }
  
  fd_widget.SetRenderingSettings(startup_sets);
  fd_widget.SetPetrophysics(petrophysics);
  fd_widget.SetDomainRange(0, petrophysics.domains.size() - 2);

  fd_widget.EndInit();  
  
  fd_widget.show();


  return QApplication::exec();
}


vtkSmartPointer<vtkPropAssembly> CreateSurfacesActor(SModeller& strat_model, dpl::vector3d scale) {
  auto surfaces_prop = vtkSmartPointer<vtkPropAssembly>::New();

  strat_model.useDefaultCoordinateSystem();

  double x, y, z;

  strat_model.getSize(x, y, z); 

  // auto count_surf = strat_model.getOrderedSurfacesIndices().size();
  // auto i = 0;

  auto r = 0.05;
  for (auto surface_idx : strat_model.getOrderedSurfacesIndices()) {
    // ++i;
    // if (i == 1 || i == count_surf)
    //   continue;

    vtkNew<vtkCellArray> vtk_indices;

    std::vector<double> coordinates;
    std::vector<size_t> indices;

    strat_model.getMesh(surface_idx, coordinates, indices);

    if (indices.empty())
      continue;

    vtkNew<vtkPolyData> poly_data;

    vtkNew<vtkPoints> vtk_points;
    vtk_points->SetNumberOfPoints(coordinates.size()/3);

    for (size_t i = 0, vertices = coordinates.size()/3; i < vertices; ++i) {
      // if (z < min_z) min_z = z;
      // if (z > max_z) max_z = z;
      vtk_points->SetPoint(i,
        scale.x()*coordinates[i*3 + 0],
        scale.y()*(y - coordinates[i*3 + 1]),
        scale.z()*coordinates[i*3 + 2]);
    }

    for (size_t i = 0, triangles = indices.size()/3; i < triangles; ++i) {
      vtk_indices->InsertNextCell(3);
      vtk_indices->InsertCellPoint(indices[i*3 + 0]);
      vtk_indices->InsertCellPoint(indices[i*3 + 1]);
      vtk_indices->InsertCellPoint(indices[i*3 + 2]);
    }

    poly_data->SetPoints(vtk_points);
    poly_data->SetPolys(vtk_indices);

    vtkNew<vtkPolyDataMapper> mapper;

    mapper->SetInputData(poly_data);

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);


    // int r, g, b;
    // controller->model.objects[surface_idx]->getColor(r, g, b);

    r += 0.1;
    if (r >= 1)
      r -= 1;
    auto color = QColor::fromHslF(r, 0.7, 0.5);

    actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

    // actor->GetProperty()->SetColor(r/255.0, g/255.0, b/255.0);
    actor->GetProperty()->SetAmbient(0.5);
    actor->GetProperty()->SetDiffuse(0.4);

    surfaces_prop->AddPart(actor);
    // vtkNew<vtkFeatureEdges> edges;
    //
    // edges->SetInputData(poly_data);
    // edges->BoundaryEdgesOn();
    // edges->FeatureEdgesOff();
    // edges->ManifoldEdgesOff();
    // edges->NonManifoldEdgesOff();
    //
    // edges->ColoringOff();
    //
    // vtkNew<vtkPolyDataMapper> outlineMapper;
    // outlineMapper->SetInputConnection(edges->GetOutputPort());
    //
    // vtkNew<vtkActor> outlineActor;
    // outlineActor->SetMapper(outlineMapper);
    // // outlineActor->GetProperty()->SetLineWidth(1.5);
    // outlineActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
    //
    // surfaces_prop->AddPart(outlineActor);
  }

  strat_model.useOpenGLCoordinateSystem();

  return surfaces_prop;
}


#ifdef _WIN32
#include "Windows.h"

INT WinMain(HINSTANCE, HINSTANCE, PSTR, INT) {
  main(__argc, __argv);
}
#endif
