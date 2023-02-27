/* This file is part of the stratmod library */
/* Copyright (C) 2017-2020, Julio Daniel Machado Silva. */

/* The stratmod library is free software: you can redistribute it and/or modify it */
/* under the terms of the GNU General Public License as published by the Free */
/* Software Foundation, either version 3 of the License, or (at your option) any */
/* later version. */

/* The stratmod library is distributed in the hope that it will be useful, but */
/* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or */
/* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more */
/* details. */

/* You should have received a copy of the GNU General Public License */
/* along with the stratmod library.  If not, see <https://www.gnu.org/licenses/>. */

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
