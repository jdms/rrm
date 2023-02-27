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

#include "rrm/fd/FDWidget.h"
#include "rrm/fd/core/QLayoutBuilder.hpp"

#include <QPieSeries>

namespace rrm::fd::component
{
  inline QtCharts::QPieSeries* CreateTracerPieSeries(
    FDWidget* w, bool is_producers) {
    
    double* tracer_ptr;
    vtkLookupTable* lut;

    if (is_producers) {
      tracer_ptr = &w->Model()[TRACER_BACKWARD][0];
      lut = w->CellInfo(TRACER_BACKWARD).Lut();
    }
    else {
      tracer_ptr = &w->Model()[TRACER_FORWARD][0];
      lut = w->CellInfo(TRACER_FORWARD).Lut();
    }
    
    std::vector<VerticalWell*> wells;
    for (auto& well : w->Wells())
      if (w->GetFlowSummary().well_results[&well].IsProducer() == is_producers)
        wells.push_back(&well);


    int tracer_count = wells.size(); //    floor(lut->GetTableRange()[1]);
    std::vector<int> hist(tracer_count);

    auto cell_count = w->Model().CellCount();
    for (auto idx = 0; idx < cell_count; ++idx) {
      auto tracer_idx = round(tracer_ptr[idx]) - 1;

      if (w->CellFilter()(idx) && 0 <= tracer_idx && tracer_idx < tracer_count)
        ++hist[tracer_idx];
    }

    auto* series = new QtCharts::QPieSeries;

    auto tracer_value = 0;
    for (auto freq : hist) {
      if (freq > 0) {
        double color[3];

        lut->GetColor(tracer_value + 1, color);

        auto* slice = new core::QCustomPieSlice;

        slice->name = wells[tracer_value]->name;
        slice->setValue(freq);
        slice->setColor(QColor::fromRgbF(color[0], color[1], color[2]));

        series->append(slice);
      }

      ++tracer_value;
    }

    return series;
  }
  
  class WellPartitioning
  {
    // ReSharper disable CppUninitializedNonStaticDataMember
    FDWidget* fd_widget_;

    QRadioButton* inj_;
    QRadioButton* prod_;
    core::QExtendedPieChartView* tracer_pie_view_;
    QtCharts::QChart* tracer_pie_chart_;
    
    // ReSharper restore CppUninitializedNonStaticDataMember

  public:
    QWidget* Init(FDWidget* w) {
      fd_widget_ = w;

      using namespace QtCharts;

      tracer_pie_view_ = new core::QExtendedPieChartView;
      tracer_pie_view_->show_percentage = true;
      
      tracer_pie_chart_ = new QChart;
      tracer_pie_view_->setChart(tracer_pie_chart_);
      
      prod_ = new QRadioButton{"Producers"};
      prod_->setChecked(true);

      inj_ = new QRadioButton{"Injectors"};
      inj_->setChecked(false);

      QObject::connect(prod_, &QRadioButton::clicked, [this](bool) { Update(); });
      QObject::connect(inj_, &QRadioButton::clicked, [this](bool) { Update(); });
      
      fd_widget_->WellNameChanged += [this] { Repaint(); };
      fd_widget_->VisibleVolumeChanged += [this] { Update(); };
      
      using namespace core::QLayoutBuilder;
      using std::tuple;
      
      return widget{
        box(dir::vertical) <<
        spacing{6} | (
          box(dir::horizontal) <<
          spacing{9} | prod_ | inj_ | stretch{1}) |
        tuple{tracer_pie_view_, stretch{1}}};
    }

    auto* PieSeries() {
      return static_cast<QtCharts::QPieSeries*>(tracer_pie_chart_->series()[0]);
    }
    
    bool Producers() {
      return prod_->isChecked();
    }
    
    void Update() const {
      tracer_pie_chart_->removeAllSeries();
      tracer_pie_chart_->addSeries(CreateTracerPieSeries(fd_widget_, prod_->isChecked()));

      Repaint();
    }
    
    void Repaint() const {
      tracer_pie_view_->viewport()->update();
    }
  };
}
