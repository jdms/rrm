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

#ifndef STRATMOD_SRC_SMODELLER2_ACCESS_HPP
#define STRATMOD_SRC_SMODELLER2_ACCESS_HPP

#include "detail/smodeller_impl.hpp"

#include "stratmod/smodeller.hpp"

namespace stratmod::detail {
    class SModellerAccess {
        public: 
            static SModellerImplementation* Get();
    };
} // stratmod::detail

#endif
