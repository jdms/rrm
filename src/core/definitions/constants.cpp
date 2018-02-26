/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */


/*
 * Constants.cpp
 *
 *  Created on: Aug 23, 2015
 *      Author: felipe
 */

#include "constants.hpp"

namespace Math {

/// TODO inveser sqrt inverse 3

const double Math::Constants::Pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164;
const double Math::Constants::HalfPi = Math::Constants::Pi / 2.0;

const double Math::Constants::SqrtTwo = 1.4142135623730950488016887242096980785696718753769480731766797379907324784621;
const double Math::Constants::InverseSqrtTwo = 0.70710678118654752440084436210484903928483593756084;

const double Math::Constants::SqrtThree = 1.7320508075688772935274463415058723669428052538103806280558069794519330169088;
const double Math::Constants::InverseSqrtThree = 0.57735026918962576450;
const double Math::Constants::InverseThree = 0.333333333333333333333333333333333333333333333333;

const double Math::Constants::Degree2Radian = Math::Constants::Pi / 180.0;
const double Math::Constants::Radian2Degree = 180.0 / Math::Constants::Pi;

const double Math::Constants::Infinty = std::numeric_limits<double>::max();
const double Math::Constants::Epsilon = std::numeric_limits<double>::epsilon();

const std::size_t Math::Constants::MaxSize_t = std::numeric_limits<std::size_t>::max();
const std::size_t Math::Constants::MinSize_t = std::numeric_limits<std::size_t>::min();

}


namespace Settings {

    const double Volume::VOLUME_WIDTH = 500;
    const double Volume::VOLUME_HEIGHT = 500;
    const double Volume::VOLUME_LENGTH = 500;

    const double Volume::VOLUME_ORIGINX = 0;
    const double Volume::VOLUME_ORIGINY = 0;
    const double Volume::VOLUME_ORIGINZ = 0;

    const std::string Volume::DEFAULT_VOLUME_NAME = "Volume ";



    const CrossSection::CrossSectionDirections CrossSection::DEFAULT_CSECTION_DIRECTION = CrossSection::CrossSectionDirections::Z;
    const CrossSection::CrossSectionDirections CrossSection::DEFAULT_TOPVIEW_DIRECTION = CrossSection::CrossSectionDirections::Y;

    const double CrossSection::INITIAL_CSECTIONX_POSITION = Volume::VOLUME_ORIGINX;
    const double CrossSection::INITIAL_CSECTIONY_POSITION = Volume::VOLUME_HEIGHT;
    const double CrossSection::INITIAL_CSECTIONZ_POSITION = Volume::VOLUME_ORIGINZ;

    const std::size_t CrossSection::CSECTION_DISCRETIZATION = 10;
    const std::string CrossSection::DEFAULT_CSECTION_NAME = "CrossSection ";



    const Stratigraphy::StratigraphicRules Stratigraphy::DEFAULT_STRAT_RULES = Stratigraphy::StratigraphicRules::TRUNCATE;
    const std::string Stratigraphy::DEFAULT_STRAT_NAME = "Surface ";
    const MeshResolution Stratigraphy::DEFAULT_MESH_RESOLUTION = MeshResolution::REGULAR;




    const std::string Regions::DEFAULT_REGION_NAME = "Region ";




    const Well::WellType Well::DEFAULT_WELL_TYPE = Well::WellType::NONE;




    const Objects::ObjectType Objects::DEFAULT_OBJECT_TYPE = Objects::ObjectType::STRATIGRAPHY;
    const Objects::BounderingRegion Objects::DEFAULT_BOUNDERING_REGION = Objects::BounderingRegion::NONE;
    const std::size_t Objects::MAX_CSECTION_NUMBER_FOR_CHANNEL = 1;



    const double Application::APP_WIDTH = 1024;
    const double Application::APP_HEIGHT = 800;

    const double Application::APP_ORIGIN_X = 10;
    const double Application::APP_ORIGIN_Y = 10;

    const double Application::APP_WIDTH_SCALE = 0.8;
    const double Application::APP_HEIGHT_SCALE = 0.8;

    const bool Application::DEFAULT_TOPVIEW_VISIBILITY = true;
    const bool Application::DEFAULT_CSECTION_VISIBILITY = true;
    const bool Application::DEFAULT_SIMULATOR_VISIBILITY = false;
    const bool Application::DEFAULT_SIDEBAR_VISIBILITY = true;

    const std::string Application::SHADERS_DIRECTORY = "./shaders";
    const std::string Application::IO_DIRECTORY = "./files";
    const std::string Application::SCREENSHOT_DIRECTORY = "./screenshots";
    const std::string Application::OUTPUT_COMPUTATIONS = "./output_files";




    const int OpenGL::DEPTH_BUFFER = 16;
    const int OpenGL::SAMPLES = 16;

}

