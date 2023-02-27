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

#include <QCheckBox>
#include <QPieSeries>


namespace rrm::fd::component
{
  template <typename Filter>
  QtCharts::QPieSeries* CreateDomainPieSeries(
    const RegularGrid& model, const Filter& filter, bool pore, bool hidden) {
    std::map<int, double> domain_to_volume;

    double other_volume = 0.0;
    
    auto cell_count = model.CellCount();
    for (auto idx = 0; idx < cell_count; ++idx) {
      auto value = pore ? model[POROSITY][idx] : 1.0;
      
      if (filter(idx))
        domain_to_volume[model.Domain(idx)] += value;
      else
        other_volume += value;
    }
    
    auto cell_volume = model.CellSize().prod();
    
    auto* series = new QtCharts::QPieSeries;

    for (auto [domain_idx, count] : domain_to_volume) {
      auto* slice = new core::QCustomPieSlice;
      auto& domain = model.Petrophysics().domains[domain_idx];
      slice->name = domain.name;
      slice->setValue(count*cell_volume);
      slice->setColor(domain.color);

      series->append(slice);
    }

    if (hidden && other_volume) {
      auto* slice = new core::QCustomPieSlice;
      slice->name = "<Hidden>";
      slice->setValue(other_volume*cell_volume);
      slice->setColor(Qt::white);
      
      series->append(slice);
    }

    return series;
  }


  class DomainPartitioning
  {
    // ReSharper disable CppUninitializedNonStaticDataMember
    FDWidget* fd_widget_;

    QtCharts::QChart* chart_;
    core::QExtendedPieChartView* view_;

    QCheckBox* percent_;
    QCheckBox* pore_;
    QCheckBox* hidden_;
    // ReSharper restore CppUninitializedNonStaticDataMember

  public:
    QWidget* Init(FDWidget* w) {
      fd_widget_ = w;

      view_ = new core::QExtendedPieChartView;
      chart_ = new QtCharts::QChart;

      view_->setChart(chart_);
      
      percent_ = new QCheckBox{"%"};
      percent_->setChecked(false);
      QObject::connect(percent_, &QCheckBox::clicked, [this](bool v) {
        view_->show_percentage = v;
        view_->viewport()->update();
      });
      
      pore_ = new QCheckBox{"Pore"};
      pore_->setToolTip("Pore volume");
      QObject::connect(pore_, &QCheckBox::clicked, [this](bool) { Update(); });
      
      hidden_ = new QCheckBox{"Hidden"};
      QObject::connect(hidden_, &QCheckBox::clicked, [this](bool) { Update(); });
      
      view_->show_percentage = percent_->isChecked();

      fd_widget_->VisibleVolumeChanged += [this] { Update(); };
      
      using namespace core::QLayoutBuilder;
      using std::tuple;

      return widget{
        box(dir::vertical) <<
        spacing{9} | (
          box(dir::horizontal, 3) <<
          spacing{9} | percent_ | pore_ | hidden_ | stretch{1}) |
        tuple{view_, stretch{1}}
      };
    }

    void Update() const {
      chart_->removeAllSeries();
      chart_->addSeries(CreateDomainPieSeries(
        fd_widget_->Model(), fd_widget_->CellFilter(), pore_->isChecked(), hidden_->isChecked()));
      
      view_->viewport()->update();
    }
  };
}
