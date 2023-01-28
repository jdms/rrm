/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2021
 * UofC - University of Calgary
 * HW - Heriot-Watt University
 *
 * This file is part of RRM Software.
 *
 * RRM is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with RRM.  If not, see <http://www.gnu.org/licenses/>,
 * or write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA  02110-1301  USA.
 */

/**
 * @file fd_interface.cpp
 * @author Julio Daniel Machado Silva
 * @author Dmytro Petrovskyy
 * @brief Implementation of interface between sketching and flow diagnostics gui
 */

#include "fd_interface.hpp"
/* #include "fd_interface_impl.hpp" */

#include <mutex>

#include <QGuiApplication>
#include <QScreen>
#include <QSurfaceFormat>

#include "rrm/fd/FDQVTKWidget.h"
#include "rrm/fd/FDWidgetImpl.h"
#include "fd_definitions.h"
#include "model/metadata_access.h"
#include "stratmod/smodeller.hpp"
#include "stratmod/sutilities.hpp"
#include "colorwrap.hpp"

/// Quick and dirty class to create colors on the fly using the Colorwrap class
/// (Colorwrap is a wraper for colorbrewer: https://colorbrewer2.org/)
class Colors {
    public:
        Colors(std::vector<int> colormap = Colorwrap::Spectral())
        {
            this->num_colors = colormap.size()/3;
            this->colormap = colormap;
        }

        QColor color(int i)
        {
            int r = 255, g = 0, b = 0;

            i = i % num_colors;
            if (invert_colors)
            {
                i = (num_colors -1) - i;
            }

            r = colormap[3*i + 0];
            g = colormap[3*i + 1];
            b = colormap[3*i + 2];

            return QColor(r, g, b);
        }

        void invert() { invert_colors = !invert_colors; }

    private:
        int num_colors{};
        std::vector<int> colormap{};
        bool invert_colors = false;
};


/////////////////////////////////////////////////////////////////////////////
//
// FlowDiagnosticsInterface : FD initialization and setup methods.
//
// With the possible exception of constructors and the destructor, the
// following methods are supposed to be maintained by whoever is responsible
// for the FD implementation, as they depend on details of the FD gui
// implementation.
//
//////////////////////////////////////////////////////////////////////////////

// class FlowDiagnosticsDefinitions is an interface with useful methods to
// get/set metadata in class stratmod::SModeller.
struct FlowDiagnosticsInterface::Impl : public /*rrm::fd::*/FlowDiagnosticsDefinitions
{
    // It is not good practice to use public and private in the Impl struct of
    // the PIMPL pattern as no one should have access to the struct
    // FlowDiagnosticsInterface::Impl (thus there is no need for data
    // encapsulation here)
    public:
        // using FDWidget = rrm::fd::FDWidget;
        using StartupSettings = rrm::fd::StartupSettings;
        using PetrophysicalSettings = rrm::fd::PetrophysicalSettings;
        using MMA = model::MetadataAccess;
        using FDD = /*rrm::fd::*/FlowDiagnosticsDefinitions;

        // using unique_ptr + mutex to prevent leaks and race conditions
        std::unique_ptr<rrm::fd::FDWidgetImpl> fd_window_ = nullptr;
        std::mutex m_{};
        inline static stratmod::SModeller* pmodel_ = nullptr;

        //
        // FlowDiagnosticsInterface::Impl specific methods
        //
        // Methods in uppercase belong to this implementation, and can't be
        // used outside of this Impl struct without significant reworking.
        //

        /// default contructor
        Impl() = default;

        /// default destructor 
        ///
        /// The @fd_window_ pointer is destroyed when the fd window is
        /// closed, see @CreateWindow().
        ~Impl() {
            CloseWindow();
        }

        /// initialize and show rrm::fd::FDWidget
        ///
        /// CreateWindow() is guarded by a lock on mutex @m_.
        ///
        /// IMPORTANT: upon instatiation of new FDWidget object, its
        /// `close_event_` member MUST be updated to set @fd_window_ to
        /// nullptr.
        #ifdef CreateWindow
        #undef CreateWindow
        #endif
        bool CreateWindow();

        /// close FDWidget window
        void CloseWindow()
        {
            std::lock_guard<std::mutex> g(m_);
            if (fd_window_)
            {
                fd_window_->close();
            }
        }

        bool IsActive()
        {
            std::lock_guard<std::mutex> g(m_);
            if (fd_window_)
            {
                return true;
            }

            return false;
        }

        //
        // General helper functions
        //
        // These are marked static to guarantee they don't depend on members of
        // struct FlowDiagnosticsInterface::Impl.  Those can be useful outside
        // of the FlowDiagnosticsInterface::Impl struct (minimal rework may be
        // required).
        //

        /// get reference to model (make sure default coordinate system is in use?)
        static stratmod::SModeller& model() { 
            /* stratmod::SModeller::Instance().useDefaultCoordinateSystem(); */
            if (pmodel_)
            {
                return *pmodel_;
            }

            return stratmod::SModeller::Instance();
        }

        /// compute current number of surfaces in model
        static int numSurfaces() { return static_cast<int>(model().getSurfacesIndices().size()); }

        /// compute max number of regions in model
        static int numRegions() { return (numSurfaces() > 0) ? (numSurfaces() -1) : 0; }

        /// [WARNING]: this method depends on the model saving its "file name" in its data structure
        /// A proper long term solution should be considered
        static std::optional<std::string> getSavedModelFilename()
        {
            stratmod::SUtilities u(model());
            /* return u.getSavedFileName(); */
            return std::optional<std::string>();
        }

        /// initialize rendering settings
        static StartupSettings processStartupSettings();

        /// initialize petrophysical properties
        static PetrophysicalSettings processPetrophysicalSettings(const StartupSettings&);

        // Why the private qualifier?
    private:
        static vtkPropAssembly* createSurfacesActor(double& min_z, double& max_z);
};

bool FlowDiagnosticsInterface::Impl::CreateWindow()
{
    std::lock_guard<std::mutex> g(m_);

    //
    // Return if FD window is already active or if there is no region to show
    //
    if (numRegions() < 1)
        return false;
    if (fd_window_) {
        fd_window_->activateWindow();
        return true;
    }

    //
    // Prepare rendering settings
    //
    StartupSettings rendering_settings = processStartupSettings();


    if (getSavedModelFilename()) {
      try {
        // TODO: Deprecated
        LoadSettings(rendering_settings.domains,
          nlohmann::json::parse(
            std::ifstream(std::filesystem::path(*getSavedModelFilename()).replace_extension("fdsettings.json"))));
      }
      catch (...) {}

      rendering_settings.TryParseConfig(
        std::filesystem::path(*getSavedModelFilename()).replace_extension("startup.json").string());
    }
  
  
    //
    // Prepare petrophysical properties
    //
    /* FDD::InterpretationHandle = MMA::InterpretationHandle; // make sure both handles point to the same interpretation */
    PetrophysicalSettings petrophysics = processPetrophysicalSettings(rendering_settings);

   
    /* TODO [WARNING]: This code does nothing */
    // TODO [Dima] - I and the runtime disagree :)
    /* Qt OpenGL profile set after main app is created is ignored */ 
    {
      auto format = rrm::fd::FDQVTKWidget::defaultFormat();
    
      #ifdef _WIN32
         format.setProfile(QSurfaceFormat::CompatibilityProfile);
      #else
         format.setProfile(QSurfaceFormat::CoreProfile);
      #endif
    
      QSurfaceFormat::setDefaultFormat(format);
    }

    {
      dpl::vector3d stratmod_size;
      model().getSize(stratmod_size.x(), stratmod_size.y(), stratmod_size.z());
      
      if (rendering_settings.stratmod.override_size)
        rendering_settings.stratmod.scale = (*rendering_settings.stratmod.override_size)/stratmod_size;

      // rendering_settings.visual.well_radius =
      //   sqrt(
      //     (stratmod_size.x()*rendering_settings.stratmod.scale.x())*
      //     (stratmod_size.y()*rendering_settings.stratmod.scale.y()))/250.;
    }

    if (!rendering_settings.visual.font_size)
      rendering_settings.visual.font_size =
        // /*1.5**/rendering_settings.visual.nominal_size*QApplication::primaryScreen()->logicalDotsPerInch()/96;
        rendering_settings.visual.nominal_size +
          (QApplication::primaryScreen()->logicalDotsPerInch()/**1.75*//96. - 1.)*10.;

  
    //
    // Create FDWidget window
    //
    auto NewFDWidget = [&]() /* -> std::unique_ptr<FDWidget> */ {
        if (fd_window_ == nullptr)
        {
            fd_window_ = std::make_unique<rrm::fd::FDWidgetImpl>();
            // not necessary as we're using a unique pointer now
            /* fd_window_->setAttribute(Qt::WA_DeleteOnClose); */
            fd_window_->close_event_ = [this]() { this->fd_window_ = nullptr; };

            /* TODO: Let's not forget these definitions; not sure if they were useful
             * in the integration of FD with the legacy prototype */

            // app_main_window->BeginInit();
            // app_main_window->hide();

            // fd_widget->setParent(this);
            // fd_widget->setWindowModality(Qt::WindowModal);
            // fd_widget->setWindowFlags(Qt::Window);

            // fd_widget->setWindowModality(Qt::WindowModal);
            // fd_widget->setWindowFlags(Qt::Dialog | Qt::Tool);
        }

        /* return std::move(fd_window_); */
    };
    /* fd_window_ = */ NewFDWidget();

    //
    // Set FD window properties
    //

    fd_window_->BeginInit();
    fd_window_->SetScale(
      dpl::vector3d{rendering_settings.stratmod.scale.max()}/rendering_settings.stratmod.scale);


    if (getSavedModelFilename())
      fd_window_->rrm_project_path = *getSavedModelFilename();

  
  
    fd_window_->SetStratigraphyModel(model());
    fd_window_->SetRenderingSettings(rendering_settings); 
    fd_window_->SetPetrophysics(petrophysics);
    fd_window_->SetDomainRange(0, petrophysics.domains.size() - 2);

    double min_z, max_z;
    auto* surfaces = createSurfacesActor(min_z, max_z);
    /* fd_window_->SetSurfacesActor(surfaces, min_z, max_z); */
    fd_window_->SetSurfacesActor(surfaces);
    fd_window_->EndInit();
    
    if (rendering_settings.visual.maximised)
      fd_window_->showMaximized();
    else
      fd_window_->showNormal();        

    return true;
}

FlowDiagnosticsInterface::Impl::StartupSettings FlowDiagnosticsInterface::Impl::processStartupSettings()
{
    StartupSettings rendering_settings{}; 

    /* using namespace rrm::fd; */
    using std::filesystem::path;

    auto load_json = [](path p) {
        return nlohmann::json::parse(std::ifstream(p));
    };        

    try {      
        LoadSettings(rendering_settings.domains, load_json("_config/fdsettings.json")); // TODO: Deprecated
    }
    catch (...) {}

    /* TODO: check if we're missing anything by not bringing the
     * TreeNamesToObjects method to this code. */
    // TreeNamesToObjects();

    rendering_settings.TryParseConfig();

    // [JD] I think I prefer the 'nominal_size' for the text
    // rendering_settings.visual.font_size = 1.5 * rendering_settings.visual.nominal_size
    //     * QGuiApplication::primaryScreen()->logicalDotsPerInch() / 96; 

    rendering_settings.font_path = rrm::fd::GetWindowsUnicodeArialFontPath();

    // {
    //     double x, y, z;
    //     model().getSize(x, y, z);
    //     rendering_settings.visual.well_radius = sqrt(x*y)/250.;
    // }

    return rendering_settings;
}

FlowDiagnosticsInterface::Impl::PetrophysicalSettings FlowDiagnosticsInterface::Impl::processPetrophysicalSettings(const StartupSettings& rendering_settings)
{
    using namespace rrm::fd;

    PetrophysicalSettings petrophysics{};

    // Make sure that domains set by sketghing gui can be used for flow diagnostics
    MMA::enforceDomainsConsistency();

    // Using a colormap to provide default colors to domains if gui has not assigned any
    Colors domains_colormap(Colorwrap::Pastel1());

    for (auto& [domain_idx, domain] : MMA::domains())
    {
        for (auto& r : domain.regions)
        {
            petrophysics.region_to_domain[r.id()] = domain_idx;
        }

        auto& domain_settings = petrophysics.domains[domain_idx];

        // `getORset*` methods are defined on interface FlowDiagnosticsDefinitions
        domain_settings.name = FDD::getORsetName(domain, "Domain" + std::to_string(domain_idx));
        domain_settings.color = FDD::getORsetColor(domain, domains_colormap.color(domain_idx));
        domain_settings.perm = {
          units::MilliDarcy(FDD::getORsetPermXY(domain, 999.99)),
          units::MilliDarcy(FDD::getORsetPermXY(domain, 999.99)),
          units::MilliDarcy(FDD::getORsetPermZ(domain, 999.99))
        };
        // domain_settings.k_xy = units::MilliDarcy(FDD::getORsetPermXY(domain, 999.99));
        // domain_settings.k_z = units::MilliDarcy(FDD::getORsetPermZ(domain, 999.99));
        domain_settings.poro = FDD::getORsetPoro(domain, 0.2999);
        domain_settings.color.setAlpha(0);
    }

    // Using same default colormap as gui to assign colors to regions if user
    // has not assigned colors to them
    Colors regions_colormap(Colorwrap::Paired(12));

    // Make sure that regions set by sketghing gui can be used for flow diagnostics
    MMA::enforceRegionsConsistency();

    // TODO: check if everything is ok
    for (auto& [region_idx, region] : MMA::regions())
    {
        auto& region_settings = petrophysics.regions[region_idx];
        region_settings.name = FDD::getORsetName(region, "Region" + std::to_string(region_idx));
        region_settings.color = FDD::getORsetColor(region, regions_colormap.color(region_idx + 1)); // +1 to match colour in sketching gui

        // Won't save metadata for regions ignored by the user
        if (petrophysics.region_to_domain.find(region_idx) == petrophysics.region_to_domain.end())
        {
            auto domain_idx = petrophysics.domains.size();
            petrophysics.region_to_domain[region_idx] = domain_idx;

            auto& domain_settings = petrophysics.domains[domain_idx];

            domain_settings.name = region_settings.name;
            domain_settings.perm = units::MilliDarcy(999.99);
            // domain_settings.k_xy = units::MilliDarcy(999.99);
            // domain_settings.k_z = units::MilliDarcy(999.99);
            domain_settings.poro = 0.2999;
            domain_settings.color = region_settings.color;
        }
    } 

    using std::filesystem::path;
    auto load_json = [](path p) {
        return nlohmann::json::parse(std::ifstream(p));
    };        

    if (rendering_settings.stratmod.discretisation)
        model().changeDiscretization(rendering_settings.stratmod.discretisation->x(), rendering_settings.stratmod.discretisation->y());

    // TODO: [JD]
    // I'm not saving this now because it would create problems for the sketching gui.
    // Besides, in my code, the "OuterRegion" id is -1.
    auto outer_domain_idx = petrophysics.domains.size();
    auto& outer_settings = petrophysics.domains[outer_domain_idx];
    outer_settings.poro = 0.3;
    outer_settings.name = "<Outer>";
    outer_settings.perm = units::MilliDarcy(0.01);
    // outer_settings.k_xy = units::MilliDarcy(0.01);
    // outer_settings.k_z = units::MilliDarcy(0.01);
    outer_settings.color = QColor::fromRgbF(0.4, 0.4, 0.4);

    petrophysics.region_to_domain[petrophysics.region_to_domain.size()] = outer_domain_idx;

    for (auto& [_, ds] : petrophysics.domains) {
        auto original_name = ds.name;
        auto lookup_iter = rendering_settings.domains.find(boost::algorithm::to_lower_copy(original_name));
        if (lookup_iter != rendering_settings.domains.end()) {
            ds = lookup_iter->second;
            ds.name = original_name;
        }
    }

    return petrophysics;
}

vtkPropAssembly* FlowDiagnosticsInterface::Impl::createSurfacesActor(double& min_z, double& max_z)
{
    auto* surfaces_prop = vtkPropAssembly::New();

    double x, y, z;
    model().getSize(x, y, z); 

    min_z = std::numeric_limits<double>::max();
    max_z = -std::numeric_limits<double>::max();

    // Using a colormap to provide default colors to surfaces before the sketching
    // gui is able to provide them
    Colors colormap_surfaces(Colorwrap::Paired(12));

    for (auto surface_idx : model().getOrderedSurfacesIndices()) {
        vtkNew<vtkCellArray> vtk_indices;

        std::vector<double> coordinates;
        std::vector<size_t> indices;

        model().getMesh(surface_idx, coordinates, indices);

        if (indices.size() == 0)
            continue;

        vtkNew<vtkPolyData> poly_data;

        vtkNew<vtkPoints> vtk_points;
        vtk_points->SetNumberOfPoints(coordinates.size() / 3);

        for (size_t i = 0, vertices = coordinates.size()/3; i < vertices; ++i) {
            z = coordinates[i*3 + 2];
            if (z < min_z) min_z = z;
            if (z > max_z) max_z = z;
            /* vtk_points->SetPoint(i, coordinates[i*3 + 0], y - coordinates[i*3 + 1], z); */
            vtk_points->SetPoint(i, coordinates[i*3 + 0], coordinates[i*3 + 1], z);
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

        // TODO: check code below, and see if anything is missing

        // Get surface metadata from model() and use its color if defined,
        // use default color otherwise. See also @MMA::getSurfaceMetadata
        double r = 1., g = 0., b = 0.; // qreal r, g, b;
        colormap_surfaces.color(surface_idx).getRgbF(&r, &g, &b);
        {
            stratmod::SurfaceMetadata data;
            bool success = model().getSurfaceMetadata(surface_idx, data);
            if (success && data.color_rgb)
            {
                FDD::color(*data.color_rgb).getRgbF(&r, &g, &b);
            }
        }
        actor->GetProperty()->SetColor(r, g, b);
        actor->GetProperty()->SetAmbient(0.5);
        actor->GetProperty()->SetDiffuse(0.4);

        surfaces_prop->AddPart(actor);

        // TODO: uncoment anything?
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

    return surfaces_prop;
}

//////////////////////////////////////////////////////////////////////////////
// Maintanence of constructors and destructor is shared by SBIM gui and FD gui,
// update them as required for any new data structured added to class
// FlowDiagnosticsInterface.
//
// Also, see @FlowDiagnosticsInterface::Impl section.
//
//////////////////////////////////////////////////////////////////////////////

FlowDiagnosticsInterface::FlowDiagnosticsInterface()
{
    pimpl_ = std::make_unique<FlowDiagnosticsInterface::Impl>();
}

FlowDiagnosticsInterface::~FlowDiagnosticsInterface() = default;
FlowDiagnosticsInterface::FlowDiagnosticsInterface(FlowDiagnosticsInterface&&) = default;
FlowDiagnosticsInterface& FlowDiagnosticsInterface::operator=(FlowDiagnosticsInterface&&) = default;

bool FlowDiagnosticsInterface::isAvailable()
{
    return (pimpl_->numRegions() > 0);
}

bool FlowDiagnosticsInterface::isActive()
{
    return pimpl_->IsActive();
}

bool FlowDiagnosticsInterface::createWindow()
{
    bool success = pimpl_->CreateWindow();
    stratmod::SModeller::Instance().useDefaultCoordinateSystem();

    return success;
}

void FlowDiagnosticsInterface::closeWindow()
{
    pimpl_->CloseWindow();
    stratmod::SModeller::Instance().useDefaultCoordinateSystem();
}

void FlowDiagnosticsInterface::setModel(stratmod::SModeller* pmodel)
{
    pimpl_->pmodel_ = pmodel;
    model::MetadataAccess::pModel(pmodel);
    FlowDiagnosticsDefinitions::pModel(pmodel);
}

/////////////////////////////////////////////////////////////////////////////
//
// FlowDiagnosticsInterface methods : shared methods
//
// These are meant to share state between main SBIM gui and FD gui and should
// be maintained by both parties.
//
//////////////////////////////////////////////////////////////////////////////

