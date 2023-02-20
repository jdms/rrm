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

#include <QValueAxis>
#include <QPushButton>
#include <QRadioButton>

namespace rrm::fd::component
{
  class SweepPlot
  {
    // ReSharper disable CppUninitializedNonStaticDataMember
    FDWidget* fd_widget_;

    QtCharts::QChart* sweep_chart_;
    QtCharts::QLineSeries* sweep_series_;
    QRadioButton* radio_pvi_;
    QtCharts::QChartView* sweep_chart_view_;
    // ReSharper restore CppUninitializedNonStaticDataMember

  public:
    auto* LineSeries() const {
      return sweep_series_;
    }

    bool CheckedPVI() const {
      return radio_pvi_->isChecked();
    }
    
    QWidget* Init(FDWidget* w/*, std::function<void()> reset_charts*/) {
      fd_widget_ = w;

      using namespace QtCharts;
      
      radio_pvi_ = new QRadioButton{"PVI"};
      radio_pvi_->setChecked(true);

      auto* radio_time = new QRadioButton{"Time"};
      radio_time->setChecked(false);

      auto* copy_button = new QPushButton{"Copy"};
      // copy_button->setFixedWidth(40);

      auto click = [this/*, reset_charts*/] {
        auto* axisx = static_cast<QValueAxis*>(sweep_chart_->axes(Qt::Horizontal)[0]);
        if (radio_pvi_->isChecked()) {
          axisx->setTitleText("Pore volumes injected");
          axisx->setRange(0, 2);
          axisx->setLabelFormat("%.2f");
        }
        else {
          axisx->setTitleText("Time, years");
          axisx->setRange(0, 20);
          axisx->setLabelFormat("%.0f");
        }


        Update();
      };

      QObject::connect(radio_pvi_, &QRadioButton::clicked, click);
      QObject::connect(radio_time, &QRadioButton::clicked, click);

      QObject::connect(copy_button, &QPushButton::clicked, [this] {
        std::stringstream ss;

        for (auto p : sweep_series_->points())
          ss << p.x() << '\t' << p.y() << '\n';

        QApplication::clipboard()->setText(QString::fromStdString(ss.str()));
      });

      sweep_chart_view_ = new QChartView;
      sweep_chart_view_->setRenderHint(QPainter::Antialiasing);
      sweep_chart_view_->setBackgroundBrush(Qt::GlobalColor::white);


      auto scaled_font = QApplication::font();
      scaled_font.setPointSize(8);

      QFont font = scaled_font;
      font.setBold(false);

      QBrush black_brush{Qt::black};

      sweep_series_ = new QLineSeries();

      sweep_chart_ = new QChart();
      sweep_chart_->legend()->hide();
      sweep_chart_->addSeries(sweep_series_);
      sweep_chart_->createDefaultAxes();

      sweep_chart_view_->setChart(sweep_chart_);

      fd_widget_->FlowDiagnosticsChanged += [this] { Update(); };

      

      auto* axis_x = static_cast<QValueAxis*>(sweep_chart_->axes(Qt::Horizontal)[0]);
      axis_x->setLabelsFont(scaled_font);
      axis_x->setLabelFormat("%.2f");
      axis_x->setTitleText("Pore volumes injected");
      axis_x->setTitleFont(font);
      axis_x->setRange(0, 2);
      axis_x->setLabelsBrush(black_brush);
      axis_x->setTitleBrush(black_brush);


      auto* axis_y = static_cast<QValueAxis*>(sweep_chart_->axes(Qt::Vertical)[0]);
      axis_y->setLabelsFont(scaled_font);
      axis_y->setLabelFormat("%.2f");
      axis_y->setTitleText("Recovery factor");
      axis_y->setTitleFont(font);
      axis_y->setRange(0, 1);
      axis_y->setLabelsBrush(black_brush);
      axis_y->setTitleBrush(black_brush);

      sweep_series_->setPen({sweep_series_->pen().color(), 1.75});

      using namespace core::QLayoutBuilder;
      using std::tuple;

      return widget{
        box(dir::vertical) <<
        spacing{6} | (
          box(dir::horizontal, 3) <<
          spacing{9} | radio_pvi_ | radio_time | copy_button | stretch{1}) |
        tuple{sweep_chart_view_, stretch{1}}
      };
    }


    void Update() const {
      const auto& flow = fd_widget_->GetFlowSummary();
      const auto x_coef = radio_pvi_->isChecked() ? 1.0 : flow.TimeCoef();

      constexpr int steps = 100;

      sweep_series_->clear();
      sweep_series_->append(0, 0);

      const auto cell_count = fd_widget_->Model().CellCount();

      sweep_series_->append(x_coef*flow.sweep[1].x(), flow.sweep[1].y());
      for (auto i = 0; i < steps; ++i) {
        const int j = (i + 1.0)/steps*(cell_count - 1);

        if (!std::isinf(flow.sweep[j + 1].x()))
          sweep_series_->append(x_coef*flow.sweep[j + 1].x(), flow.sweep[j + 1].y());
      }
    }
  };
}