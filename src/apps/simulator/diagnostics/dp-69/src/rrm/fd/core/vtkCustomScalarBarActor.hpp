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

#include <vtkPolyData.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarActorInternal.h>
#include <vtkScalarsToColors.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>

namespace rrm::fd::core
{
  class vtkCustomScalarBarActor : public vtkScalarBarActor
  {
    std::function<std::string(double)> label_format_map_;
    
  public:
    const StartupSettings* rendering_settings;
    
    // static vtkStandardNewMacro(vtkCustomScalarBarActor)
    static vtkCustomScalarBarActor* New() { VTK_STANDARD_NEW_BODY(vtkCustomScalarBarActor); }
    vtkTypeMacro(vtkCustomScalarBarActor, vtkScalarBarActor)
    
    void SetLabelFormatMap(std::function<std::string(double)> map) {
      label_format_map_ = std::move(map);
    }

  protected:
    void ConfigureTitle() override {
      vtkScalarBarActor::ConfigureTitle();

      if (/*!this->LookupTable->GetIndexedLookup() && */this->GetOrientation() == VTK_ORIENT_HORIZONTAL) {
        auto& actor = this->TitleActor;
        auto* pos = actor->GetPosition();
        actor->SetPosition(pos[0],
          this->P->ScalarBarBox.Posn[1] + this->P->ScalarBarBox.Size[0] + rendering_settings->visual.font_size*0.3);

        actor->GetTextProperty()->SetVerticalJustificationToBottom();
      }
    }
    
    void ConfigureAnnotations() override {
      vtkScalarBarActor::ConfigureAnnotations();

      if (this->LookupTable->GetIndexedLookup()) {
        if (this->GetOrientation() == VTK_ORIENT_VERTICAL)
          for (auto& actor : this->P->AnnotationLabels) {
            auto* pos = actor->GetPosition();      
            actor->SetPosition(pos[0] + rendering_settings->visual.font_size*1.5, pos[1]);
            actor->GetTextProperty()->SetJustificationToLeft();
          }
        else {
          for (auto& actor : this->P->AnnotationLabels) {
            // auto* pos = actor->GetPosition();      
            // actor->SetPosition(pos[0], pos[1] - rendering_settings->visual.font_size*0.3);
            // actor->SetOrientation(90);
            // actor->GetTextProperty()->SetJustificationToRight();
            // actor->GetTextProperty()->SetVerticalJustificationToCentered(); // TODO: leading lines to Labels
            
            
            auto* pos = actor->GetPosition();
            actor->SetPosition(pos[0], this->P->ScalarBarBox.Posn[1] - rendering_settings->visual.font_size*0.4);
            
            actor->GetTextProperty()->SetJustificationToCentered();
            actor->GetTextProperty()->SetVerticalJustificationToTop(); 
          }

          this->P->AnnotationLeaders->Initialize();
        }
      }
    }
    
    void ConfigureTicks() override {
      vtkScalarBarActor::ConfigureTicks();

      if (!this->LookupTable->GetIndexedLookup() && this->GetOrientation() == VTK_ORIENT_HORIZONTAL)
        for (int i = 0; i < this->NumberOfLabels; ++i) {      
          auto& actor = this->P->TextActors[i];
          auto* pos = actor->GetPosition();
          
          actor->SetPosition(pos[0], this->P->ScalarBarBox.Posn[1] - rendering_settings->visual.font_size*0.4/*0.25*/);
          actor->GetTextProperty()->SetVerticalJustificationToTop();
        }
    }
    
    void LayoutTicks() override { 
      vtkScalarBarActor::LayoutTicks();

      if (!label_format_map_)
        return;

      auto* lut = this->LookupTable;
      
      if (lut->GetIndexedLookup()) { 
      }
      else {
        auto is_log_table = lut->UsingLogScale();
        auto* range = lut->GetRange();

        for (int i = 0; i < this->NumberOfLabels; ++i)
          this->P->TextActors[i]->SetInput(
            label_format_map_(dpl::lerp(is_log_table, range[0], range[1], i/(this->NumberOfLabels - 1.))).c_str());
      }
    }
  };
}
