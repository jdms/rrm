/********************************************************************************/
/*                                                                              */
/* This file is part of the "Stratigraphy Modeller Library" (StratModLib)       */
/* Copyright (C) 2017-2022, Julio Daniel Machado Silva.                         */
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



#ifndef STRATMOD_MODEL_SURFACE
#define STRATMOD_MODEL_SURFACE

#include <map>
#include <memory>
#include <string>
#include <optional>

#include "stratmod/misc/stratmod_primitives.hpp"
#include "stratmod/metadata.hpp"

#include "Eigen/Dense"

namespace stratmod {

class ModelSurface {
    public:

    private:
        struct ModelSurfaceImpl;
        std::unique_ptr<ModelSurfaceImpl> pimpl_;
};

} // namespace stratmod

#endif
