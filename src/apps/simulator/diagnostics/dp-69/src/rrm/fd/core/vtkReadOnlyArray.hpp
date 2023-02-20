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
#include <vtkAOSDataArrayTemplate.h>
#include <vtkGenericDataArray.h>


namespace rrm::fd::core
{
  template<typename Functor>
  class vtkReadOnlyArray : public vtkGenericDataArray<vtkReadOnlyArray<Functor>, std::invoke_result_t<Functor, vtkIdType>>
  {
    using ValueType = std::invoke_result_t<Functor, vtkIdType>;
    using ThisType = vtkReadOnlyArray<Functor>;
    using BaseType = vtkGenericDataArray<ThisType, ValueType>;
    friend class vtkGenericDataArray<ThisType, ValueType>;
    
    std::unique_ptr<Functor> func_;
    
  protected:
    bool AllocateTuples(vtkIdType) { return true; }
    bool ReallocateTuples(vtkIdType) { return true; }
    vtkObjectBase* NewInstanceInternal() const override { return vtkAOSDataArrayTemplate<ValueType>::New(); }

  public:
    template<typename... Args>
    void ConstructFunctor(Args&&... args) {
      func_ = std::make_unique<Functor>(std::forward<Args>(args)...);
    }
    
    auto& operator()() {
      return *func_;
    }
    
    // static vtkStandardNewMacro(ThisType)
    static ThisType* New() { VTK_STANDARD_NEW_BODY(ThisType); }
    vtkAbstractTemplateTypeMacro(ThisType, BaseType)


    
    ValueType GetValue(vtkIdType) const { throw std::exception(); }
    void SetValue(vtkIdType, ValueType) { throw std::exception(); }
    void GetTypedTuple(vtkIdType, ValueType*) const { throw std::exception(); }
    void SetTypedTuple(vtkIdType, const ValueType*) { throw std::exception(); }
    void SetTypedComponent(vtkIdType, int, ValueType) { throw std::exception(); }

    
    
    ValueType GetTypedComponent(vtkIdType tupleIdx, int) const {
      return (*func_)(tupleIdx);
    }
  };

  template<typename Func>
  auto vtkReadOnlyArrayCreate(Func f) {
    auto result = vtkSmartPointer<vtkReadOnlyArray<Func>>::New();
    result->ConstructFunctor(f);
    return result;
  }
}
