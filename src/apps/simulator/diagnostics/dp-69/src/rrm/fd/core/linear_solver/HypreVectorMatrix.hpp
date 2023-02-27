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

#include <HYPRE.h>
#include <HYPRE_parcsr_ls.h>

#ifndef MPI_COMM_WORLD
#define MPI_COMM_WORLD 0
#endif

namespace linear_solver
{
  class HypreVector
  {
    HYPRE_IJVector v_ = nullptr;

  public:
    // HypreVector() = default;

    HypreVector(HYPRE_Int nrows/*, const HYPRE_Int* rows*/, const HYPRE_Real* values) {
      SetValues(nrows/*, rows*/, values);
    }

    HypreVector(HYPRE_Int nrows) {
      AllocateSpace(nrows);
    }

    ~HypreVector() {
      if (v_)
        HYPRE_IJVectorDestroy(v_);
    }

    void AllocateSpace(HYPRE_Int nrows) {
      HYPRE_IJVectorCreate(MPI_COMM_WORLD, 0, nrows - 1, &v_);
      HYPRE_IJVectorSetObjectType(v_, HYPRE_PARCSR);
      HYPRE_IJVectorInitialize(v_);
      
      HYPRE_IJVectorAssemble(v_);
    }

    void SetValues(HYPRE_Int nrows/*, const HYPRE_Int* rows*/, const HYPRE_Real* values) {
      HYPRE_IJVectorCreate(MPI_COMM_WORLD, 0, nrows - 1, &v_);
      HYPRE_IJVectorSetObjectType(v_, HYPRE_PARCSR);
      HYPRE_IJVectorInitialize(v_);

      //      HYPRE_IJVectorSetValues(_v, nrows, rows, values);
      for (HYPRE_Int i = 0; i < nrows; ++i)
        HYPRE_IJVectorSetValues(v_, 1, &i, values + i);
            
      HYPRE_IJVectorAssemble(v_);
    }

    HYPRE_ParVector GetParVector() const {      
      HYPRE_ParVector pointer;
      HYPRE_IJVectorGetObject(v_, reinterpret_cast<void**>(&pointer));
      return pointer;
    }

    void GetValues(HYPRE_Int nrows, const HYPRE_Int* rows, HYPRE_Real* values) const {
      HYPRE_IJVectorGetValues(v_, nrows, rows, values);
    }


    HypreVector(const HypreVector& other) = delete;
    HypreVector(HypreVector&& other) = delete;
    HypreVector& operator=(const HypreVector& other) = delete;
    HypreVector& operator=(HypreVector&& other) = delete;
  };
  
  class HypreMatrix
  { 
    HYPRE_IJMatrix m_ = nullptr;  

  public:   
    void SetValues(HYPRE_Int nrows, HYPRE_Int ncols, HYPRE_Int* ncols_per_row, const HYPRE_Int* cols_of_coefs, const HYPRE_Real* coefs) {
      HYPRE_IJMatrixCreate(MPI_COMM_WORLD, 0, nrows - 1, 0, ncols - 1, &m_);
      
      HYPRE_IJMatrixSetObjectType(m_, HYPRE_PARCSR);
      // HYPRE_IJMatrixSetRowSizes(A, ncols.data());    

      // std::vector<HYPRE_Int> zeroArray(nrows, 0);
      // HYPRE_IJMatrixSetDiagOffdSizes(m_, ncols_per_row, zeroArray.data());

      HYPRE_IJMatrixInitialize(m_);
      // HYPRE_IJMatrixSetValues(_m, nrows, ncols, rows, cols, values);
      
      for (HYPRE_Int i = 0, shift = 0; i < nrows; ++i) {
        HYPRE_IJMatrixSetValues(m_, 1, ncols_per_row + i, &i, cols_of_coefs + shift, coefs + shift);
        shift += ncols_per_row[i];
      }

      HYPRE_IJMatrixAssemble(m_);            
    }

    HYPRE_ParCSRMatrix GetParCSRMatrix() const {
      HYPRE_ParCSRMatrix csr;            
      HYPRE_IJMatrixGetObject(m_, reinterpret_cast<void**>(&csr));
      return csr;
    }

    HypreMatrix(HYPRE_Int nrows, HYPRE_Int ncols, HYPRE_Int* ncols_per_row, const HYPRE_Int* cols_of_coefs, const HYPRE_Real* coefs) {
      SetValues(nrows, ncols, ncols_per_row, cols_of_coefs, coefs);
    }

    // HypreMatrix() = default;

    ~HypreMatrix() {
      if (m_)
        HYPRE_IJMatrixDestroy(m_);
    }

    HypreMatrix(const HypreMatrix& other) = delete;
    HypreMatrix& operator=(const HypreMatrix& other) = delete;    
    HypreMatrix(HypreMatrix&& other) = delete;    
    HypreMatrix& operator=(HypreMatrix&& other) = delete;
  };
}