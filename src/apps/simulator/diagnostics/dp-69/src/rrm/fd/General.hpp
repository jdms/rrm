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

#include "dpl/units.hpp"

#include "rrm/fd/VerticalWell.hpp"

#include <filesystem>
#include <map>
#include <optional>

#include <QColor>
#include <QVariant>

#include <vtkCamera.h>
#include <vtkLookupTable.h>
#include <vtkPropCollection.h>
#include <vtkPropAssembly.h>

#include <boost/algorithm/string.hpp>

#include <nlohmann/json.hpp>


namespace dpl
{
  template<typename... Args>
  class range_impl_ {};

  template<typename T>
  class range_impl_<vtkPropCollection, T>
  {
    vtkPropCollection* list_;

    class iter_
    {
      vtkPropCollection* list_;
      T* obj_;

    public:
      explicit iter_(vtkPropCollection* list = nullptr, T* obj = nullptr)
        : list_{list}, obj_{obj} {}

      T* operator*() { return obj_; }

      auto& operator++() {
        obj_ = static_cast<T*>(list_->GetNextProp());
        return *this;
      }

      friend bool operator!=(const iter_& lhs, const iter_& rhs) {
        return lhs.obj_ != rhs.obj_;
      }
    };
    
  public:
    explicit range_impl_(vtkPropCollection* collection)
      : list_(collection) {}

    auto begin() const {
      list_->InitTraversal();
      return iter_{list_, static_cast<T*>(list_->GetNextProp())};
    }

    auto end() const { return iter_{}; }
  };


  template<typename T>
  auto range(vtkPropAssembly* list) {
    return range_impl_<vtkPropCollection, T>(list->GetParts());
  }
}




class TimestepRecords
{
  std::vector<std::pair<std::chrono::system_clock::time_point, std::string>> timestamps_;

public:
  void Record(std::string_view desc = "") {
    timestamps_.emplace_back(std::chrono::system_clock::now(), desc);
  }


  explicit TimestepRecords(std::string_view category = "", bool record = true)
    : category(category) {
    if (record)
      Record();
  }

  std::string category;



  auto Timestamps() {
    return timestamps_.data();
  }

  auto operator[](int i) {
    return timestamps_[i];
  }

  auto& operator<<(std::string_view desc) {
    Record(desc);
    return *this;
  }

  auto& operator>>(std::ostream& str) {
    if (!category.empty())
      str << "[" << category << "]\n";

    for (auto i = 0; i < timestamps_.size() - 1; ++i) {
      if (!category.empty())
        str << "  ";
      str << timestamps_[i + 1].second << ": "
        << std::chrono::duration_cast<std::chrono::milliseconds>(
          timestamps_[i + 1].first - timestamps_[i].first).count() << "ms\n";
    }

    return str;
  }
};


struct ArithmeticAverage {  
  template<typename X, typename Y>
  constexpr auto operator()(X x, Y y) const {
    return (x + y)/2.;
  }
};

struct HarmonicAverage {  
  template<typename X, typename Y>
  constexpr auto operator()(X x, Y y) const {
    return 2./(1./x + 1./y);
  }
};

struct GeometricAverage {
  template<typename X, typename Y>
  constexpr auto operator()(X x, Y y) const {
    return std::sqrt(x*y);
  }    
};








// template<typename T>
// auto vtkReadOnlyArrayNew(T&& getter) {
//   return vtkReadOnlyArray<std::remove_reference_t<T>>::New(std::forward<T>(getter));
// }




template<typename T>
QVariant ItemToQVariant(T* ptr) {
  return QVariant::fromValue(reinterpret_cast<size_t>(ptr));
}
template<typename T>
auto ItemFromQVariant(const QVariant& v) {
  return reinterpret_cast<T*>(v.value<size_t>());
}













namespace rrm::fd
{
  /**
   * \brief type for 1d array index
   */
  using i1d = int;

  
  inline void PopulateLut(vtkLookupTable* lut, const std::vector<std::pair<vtkIdType, dpl::vector3d>>& entries) {      
    auto prev = entries.begin();      
    auto curr = prev;
    
    while (++curr != entries.end()) {
      double width = curr->first - prev->first;
      for (auto i = prev->first; i < curr->first; ++i) {        
        auto coef = (i - prev->first)/width;          
        auto interp = dpl::lerp(prev->second, curr->second, coef);
        lut->SetTableValue(i, interp.x(), interp.y(), interp.z());
      }
        
      prev = curr;
    }

    lut->SetTableValue(prev->first, prev->second.x(), prev->second.y(), prev->second.z());           
  }

  inline void PopulateLutRedWhiteBlue(vtkLookupTable* lut, const int entries = 1000) {
    lut->SetNumberOfTableValues(entries);
    
    PopulateLut(lut, {
      {0, {0.231373, 0.298039, 0.752941}},
      {3*entries/10, {0.548188, 0.581521, 0.808972}}, //
      {entries/2, {0.865003, 0.865003, 0.865003}},
      {7*entries/10, {0.785443, 0.440345, 0.507012}}, //
      {entries - 1, {0.705882, 0.0156863, 0.14902}}
    });
  }




  #define DECLARE_PHYSICAL_UNIT(name, conv) \
      constexpr double name(double value = 1) { return value*conv; }  // NOLINT(bugprone-macro-parentheses)

  #define DECLARE_PHYSICAL_UNIT_WITH_PREFIX(name, conv) \
      DECLARE_PHYSICAL_UNIT(name, conv) \
      DECLARE_PHYSICAL_UNIT(Milli##name, name(1e-3)) \
      DECLARE_PHYSICAL_UNIT(Centi##name, name(1e-2))

  
  namespace units
  {
    // Volume, m^3
    DECLARE_PHYSICAL_UNIT_WITH_PREFIX(Barrel, 0.158987)
    
    // Permeability, m^2
    DECLARE_PHYSICAL_UNIT_WITH_PREFIX(Darcy, 9.869233e-13)

    // Viscosity, Pa*s
    DECLARE_PHYSICAL_UNIT_WITH_PREFIX(Poise, 0.1)

    // Pressure, Pa
    DECLARE_PHYSICAL_UNIT_WITH_PREFIX(Pascal, 1)
    DECLARE_PHYSICAL_UNIT(Bar, 1e5)

    // Time, s
    DECLARE_PHYSICAL_UNIT(Day, 60*60*24)
    DECLARE_PHYSICAL_UNIT(Year, 60*60*24*365)
    
    // Length, m
    DECLARE_PHYSICAL_UNIT_WITH_PREFIX(Metres, 1)
    DECLARE_PHYSICAL_UNIT(Feet, 0.3048)
  }



  struct FlowSummary
  {
    /**
     * \brief the origin (0, 0) + cell count values.
     */
    std::vector<dpl::vector2d> lorenz; // (storage capacity, flow capacity)
    std::vector<dpl::vector2d> sweep; // (pore volumes injected, sweep efficiency)

    double lorenz_coef;
    double pore_volume = 0.0;
    double pore_volume_excluding_outer = 0.0;
    
    double total_inj_flow_rate;

    dpl::vector3d upscaled_perm;

    std::unordered_map<VerticalWell*, WellSimulationResult> well_results;
    std::array<double, 6> face_flow_rate;


    
    double SweepAtPV(double pv) const {
      if (sweep.size() < 2)
        return 0;
      
      if (pv <= sweep.front().x())
        return sweep.front().y();

      if (sweep.back().x() <= pv)
        return sweep.back().y();

      return dpl::lerp_solve(sweep, pv);
    }

    double SweepAtYears(double years) const {
      return SweepAtPV(years/(TimeInPVI()/units::Year()));
    }

    double TimeInPVI() const {
      return pore_volume/total_inj_flow_rate;
    }

    double TimeCoef() const {
      return TimeInPVI()/units::Year();
    }
  };
  

  
  struct RegionSettings
  {
    std::string name;
    QColor color;

    
  };


  struct DomainSettings
  {
    std::string name;
    QColor color;

    double poro;
    dpl::vector3d perm;

    struct
    {
      dpl::vector3d p0;
      dpl::vector3d p1;

      bool poro_log = false;
      std::optional<double> poro1;
      
      bool perm_log[3] {false, false, false};
      std::optional<double> perm1[3] = {{}, {}, {}};
    } grad;
  };

  inline void from_json(const nlohmann::json& json, DomainSettings& p) {
    p.name = json.at("name");

    p.perm.x() = units::MilliDarcy(json.at(json.contains("k_x") ? "k_x" : "k_xy"));
    p.perm.y() = units::MilliDarcy(json.at(json.contains("k_y") ? "k_y" : "k_xy"));
    p.perm.z() = units::MilliDarcy(json.at("k_z"));
    p.poro = json.at("poro");

    if (json.contains("hsl_f")) {
      auto& c = json.at("hsl_f");
      p.color = QColor::fromHslF(c[0], c[1], c[2]);
    }
    else if (json.contains("rgb_f")) {
      auto& c = json.at("rgb_f");
      p.color = QColor::fromRgbF(c[0], c[1], c[2]);
    }
    else if (json.contains("rgb")) {
      auto& c = json.at("rgb");
      p.color = QColor::fromRgb(c[0], c[1], c[2]);
    }
  }

  inline void to_json(nlohmann::json& j, const DomainSettings& d) {
    j = nlohmann::json{
      {"name", d.name},
      {"k_x", d.perm.x()/units::MilliDarcy()},
      {"k_y", d.perm.y()/units::MilliDarcy()},
      {"k_z", d.perm.z()/units::MilliDarcy()},
      {"poro", d.poro},
      {"rgb", nlohmann::json::array({d.color.red(), d.color.green(), d.color.blue()})}
    };
  }

  struct PetrophysicalSettings
  {
    std::map<int, RegionSettings> regions;
    std::map<int, DomainSettings> domains;
    
    std::map<int, int> region_to_domain;

    // std::vector<double> imported_permeability;
    // std::vector<double> imported_porosity;
  };

  template<typename K, typename T>
  void try_get(const nlohmann::json& j, K name, T& ref) {
    try {      
      if (j.contains(name))
        j.at(name).get_to(ref);
    }
    catch (...) {}
  }

  template<typename K, typename T>
  void try_get(const nlohmann::json& j, K name, std::optional<T>& ref, double coef = 1.0) {
    try {             
      if (j.contains(name)) 
        ref = coef*j.at(name);
    }
    catch (...) {}
  }

  inline nlohmann::json parse_path(const nlohmann::json& j) {
    return j.is_string() ? nlohmann::json::parse(std::ifstream(j.get<std::string>())) : j;
  }

  struct CameraSettings
  {
    bool parallel = false;
    double parallel_scale = 1.0;
    dpl::vector3d position;
    dpl::vector3d focal_point;
    dpl::vector3d view_up;

    CameraSettings() = default;
    
    CameraSettings(vtkCamera* camera) {
      parallel = camera->GetParallelProjection();
      parallel_scale = camera->GetParallelScale();
      camera->GetPosition(position);
      camera->GetViewUp(view_up);
      camera->GetFocalPoint(focal_point);
    }

    void operator>>(vtkCamera* camera) {
      camera->SetParallelProjection(parallel);
      camera->SetParallelScale(parallel_scale);
      camera->SetPosition(position);
      camera->SetViewUp(view_up);
      camera->SetFocalPoint(focal_point);
    }

    // CameraSettings(const nlohmann::json& j) {        
    //   j.at("position").get_to(position);
    //   j.at("focal_point").get_to(focal_point);
    //   j.at("view_up").get_to(view_up);
    // }
    
    // operator nlohmann::json() const {
    //   return nlohmann::json{
    //     {"position", position},
    //     {"focal_point", focal_point},
    //     {"view_up", view_up}
    //   };
    // }
  };


  inline void to_json(nlohmann::json& j, const CameraSettings& p) {
    j = nlohmann::json{
      {"parallel", p.parallel},
      {"parallel_scale", p.parallel_scale},
      {"position", p.position},
      {"focal_point", p.focal_point},
      {"view_up", p.view_up}
    };
  }
  
  inline void from_json(const nlohmann::json& j, CameraSettings& p) {
    j.at("position").get_to(p.position);
    j.at("focal_point").get_to(p.focal_point);
    j.at("view_up").get_to(p.view_up);

    try_get(j, "parallel", p.parallel);
    try_get(j, "parallel_scale", p.parallel_scale);        
  }

  
  
  


  inline void LoadSettings(std::map<std::string, DomainSettings>& settings, const nlohmann::json& j) {
    for (auto& domain : j.is_object() ? j["domains"] : j) {
      std::string name = domain["name"];
      boost::algorithm::to_lower(name);
      settings[name] = domain;
    }  
  }
  
  struct StartupSettings
  {
    std::map<std::string, DomainSettings> domains;
    
    std::optional<nlohmann::json> wells;

    
    
    std::optional<CameraSettings> camera;

    dpl::units::pascal_second viscosity = {dpl::units::centipoise{1}};
    std::array<std::optional<dpl::units::pascal>, 6> bond_cond;

    struct {
      double delta_density = 1.0;
      double viscosity_defending = 1.0;
      double LH_ratio = 1.0;

      double coef() const {
        return delta_density*LH_ratio/viscosity_defending;
      }
    } gravity_number;
    
    struct {
      std::optional<dpl::vector3i> resolution;
      std::optional<dpl::vector3d> location;
      std::optional<dpl::vector3d> size;
      bool edges = false;
    } grid;
    
    bool publishing = false;
    

    struct {
      dpl::vector3d scale = 1.0;
      bool y_invert = false;
      std::optional<dpl::vector2i> discretisation;
      std::optional<dpl::vector3d> override_size;
    } stratmod;

    struct
    {
      /*constexpr static */double nominal_size = 16.;
      int font_size = 0;

      // double well_radius;
      
      struct
      {
        std::optional<dpl::units::year> tof_forward;
        std::optional<dpl::units::year> tof_backward;
        dpl::vector3i type{0};
        dpl::vector3d value{0};
      } cutoff;

    
     
      
      double ScaleFactor() { return font_size/nominal_size; }

      std::string display_property{};
      std::map<std::string, std::pair<dpl::vector2d, std::optional<bool>>> colour_ranges;

      struct
      {
        bool visible = true;
        std::string shape{}; // h_top_right, h_bottom_left
        dpl::vector2d scale{1};
      } scalar_bar;
      
      struct
      {
        std::optional<dpl::vector3d> min;
        std::optional<dpl::vector3d> max;
      } axes;

      std::optional<dpl::vector2i> viewport;
      bool maximised = false;

      std::optional<dpl::vector3d> exaggeration;
    } visual;
    

    
    
    
    
    std::filesystem::path font_path;




    void TryParseConfig(std::filesystem::path p) {
      TryParseConfig(p.string());
    }

    void TryParseConfig(std::string filename = "_config/startup.json") {
      // TimestepRecords timesteps{"JSON parsing"};

      
      auto current_path = std::filesystem::current_path();

      using namespace dpl::units;
      
      try {
        auto json = nlohmann::json::parse(std::ifstream(filename));

        auto path = std::filesystem::path(filename);
        
        if (path.has_parent_path())
          std::filesystem::current_path(path.parent_path());

        try {
          TryParseConfig(json.at("base").get<std::string>());
        } catch (...) {}



        try { gravity_number.delta_density = json.at("/gravity_number/delta_density"_json_pointer); } catch (...) {}
        try { gravity_number.viscosity_defending = json.at("/gravity_number/viscosity_defending"_json_pointer); } catch (...) {}
        try { gravity_number.LH_ratio = json.at("/gravity_number/LH_ratio"_json_pointer); } catch (...) {}
        
        try { visual.exaggeration = json.at("/visual/exaggeration"_json_pointer); } catch (...) {}
        
        // try { visual.well_radius = json.at("/visual/well_radius"_json_pointer); } catch (...) {}
        try { visual.font_size = json.at("/visual/font_size"_json_pointer); } catch (...) {}
        
        try { visual.cutoff.type = json.at("/visual/cutoff/type"_json_pointer); } catch (...) {}
        try { visual.cutoff.value = json.at("/visual/cutoff/value"_json_pointer); } catch (...) {}
        try {
          auto& x = json.at("/visual/cutoff/tof_forward"_json_pointer);
          
          if (x.is_null())
            visual.cutoff.tof_forward.reset();
          else
            visual.cutoff.tof_forward = x.get<year>();
        } catch (...) {}
        
        try {
          auto& x = json.at("/visual/cutoff/tof_backward"_json_pointer);

          if (x.is_null())
            visual.cutoff.tof_backward.reset();
          else
            visual.cutoff.tof_backward = x.get<year>();
        } catch (...) {}
        
        try { visual.display_property = json.at("/visual/display_property"_json_pointer); } catch (...) {}

        try { visual.axes.min = json.at("/visual/axes/min"_json_pointer); } catch (...) {}
        try { visual.axes.max = json.at("/visual/axes/max"_json_pointer); } catch (...) {}

        try {
          for (auto& x : json.at("/visual/colour_range"_json_pointer).items()) 
            try {
              std::pair<dpl::vector2d, std::optional<bool>> result;
              
              if (x.value().is_array())
                x.value().get_to(result.first);
              else {
                x.value().at("value").get_to(result.first);
                result.second = x.value().at("log");
              }

              visual.colour_ranges[x.key()] = result;
            }
            catch (...) {}
          
        }
        catch (...) {}
        
        try { visual.scalar_bar.visible = json.at("/visual/colour_bar/visible"_json_pointer); } catch (...) {}
        try { visual.scalar_bar.shape = json.at("/visual/colour_bar/shape"_json_pointer); } catch (...) {}
        try { visual.scalar_bar.scale = json.at("/visual/colour_bar/scale"_json_pointer); } catch (...) {}

        try { visual.maximised = json.at("/visual/window/maximised"_json_pointer); } catch (...) {}
        try { visual.viewport = json.at("/visual/window/viewport"_json_pointer); } catch (...) {}

        
        try { stratmod.discretisation = json.at("/stratmod/discretisation"_json_pointer); } catch (...) {}       
        try { stratmod.scale = json.at("/stratmod/scale"_json_pointer); } catch (...) {}
        try { stratmod.override_size = json.at("/stratmod/override_size"_json_pointer); } catch (...) {}
        try { stratmod.y_invert = json.at("/stratmod/y_invert"_json_pointer); } catch (...) {}

        try { camera = parse_path(json.at("camera")); } catch (...) {}
        try { camera = parse_path(json.at("/visual/camera"_json_pointer)); } catch (...) {}
         
        
        try { publishing = json.at("publishing"); } catch (...) {}
        
        try { wells = parse_path(json.at("wells")); } catch (...) {}        
        
        try {
          auto& bc = json.at("boundary_conditions");
          
          for (int i = 0; i < 6; ++i)
            if (!bc[i].is_null())
              bond_cond[i] =  bc[i].get<pascal>();// pascal{bc[i].get<psi>()};
          
        } catch (...) {}

        try {
          viscosity = json.at("viscosity").get<pascal_second>();
        } catch(...) {}

        // try_get(j, "viscosity", viscosity, units::CentiPoise());        

        try { grid.size = json.at("/grid/size"_json_pointer); } catch (...) {}
        try { grid.location = json.at("/grid/location"_json_pointer); } catch (...) {}
        try { grid.resolution = json.at("/grid/resolution"_json_pointer); } catch (...) {}
        try { grid.edges = json.at("/grid/edges"_json_pointer); } catch (...) {}

        try {
          LoadSettings(domains, parse_path(json.at("domains")));                    
        } catch (...) {}
      }
      catch (...) {}

      std::filesystem::current_path(current_path);

      // timesteps << "finished";

      // timesteps >> std::cout;
    }
  };
  





  










  

  
  
  



  inline std::filesystem::path GetWindowsUnicodeArialFontPath() {
    #ifdef _WIN32
      for(auto& p : std::filesystem::directory_iterator("C:\\Windows\\WinSxS"))
        if (p.is_directory() && boost::starts_with(p.path().filename().string(), "amd64_microsoft-windows-font-truetype-arial"))
          return p.path() / "arial.ttf";

      return R"(C:\Windows\WinSxS\amd64_microsoft-windows-font-truetype-arial_31bf3856ad364e35_10.0.17134.1_none_5803fc87168579d6\arial.ttf)";
    #else
      return "";
    #endif
    
    
  }

  
}


