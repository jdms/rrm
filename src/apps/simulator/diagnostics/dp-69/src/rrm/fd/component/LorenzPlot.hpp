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
#include <QLineSeries>

namespace rrm::fd::component
{
  class QLorenzChartView : public QtCharts::QChartView
  {
    Q_OBJECT

    QMenu context_menu_;

  public:
    QLorenzChartView(QWidget* const parent = nullptr) : QChartView(parent) {
      setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
      connect(this, &QTreeView::customContextMenuRequested, [this](const QPoint& pos) {
        context_menu_.popup(viewport()->mapToGlobal(pos));
      });

      connect(context_menu_.addAction("Copy"), &QAction::triggered, [this]() {
        std::stringstream ss;

        for (auto p : static_cast<QtCharts::QLineSeries*>(chart()->series()[0])->points())
          ss << p.x() << '\t' << p.y() << '\n';

        QApplication::clipboard()->setText(QString::fromStdString(ss.str()));
      });
    }

  protected:
    void paintEvent(QPaintEvent* event) override {
      QChartView::paintEvent(event);

      QPainter painter(this->viewport());

      auto size = QFontMetrics(font()).size(Qt::TextSingleLine, "Homogeneous");
      auto midpoint = chart()->mapToPosition({0.5, 0.5});

      auto rect = QRectF(midpoint.x() + 2, midpoint.y(), size.width(), size.height());

      painter.drawText(rect, "Homogeneous");
    }
  };
  

  class LorenzPlot
  {
    // ReSharper disable CppUninitializedNonStaticDataMember
    FDWidget* fd_widget_;

    QtCharts::QChart* chart_;
    QLorenzChartView* lorenz_chart_view_;
    QtCharts::QChart* lorenz_chart_;
    QtCharts::QLineSeries* lorenz_series_;
    // ReSharper restore CppUninitializedNonStaticDataMember

  public:
    auto* LineSeries() const {
      return lorenz_series_;
    }
    
    QWidget* Init(FDWidget* w) {
      fd_widget_ = w;

      using namespace QtCharts;

      auto scaled_font = QApplication::font();
      scaled_font.setPointSize(8);

      QFont font = scaled_font;
      font.setBold(false);
      
      QBrush black_brush{Qt::black};

      
      

      lorenz_chart_view_ = new QLorenzChartView;
      lorenz_chart_view_->setRenderHint(QPainter::Antialiasing);
      lorenz_chart_view_->setBackgroundBrush(Qt::GlobalColor::white);


      lorenz_series_ = new QLineSeries();
      lorenz_series_->append(0, 0);
      lorenz_series_->append(1.0, 1.0);


      auto* homo_line = new QLineSeries();

      QPen pen;
      pen.setDashPattern({9, 6});
      homo_line->setPen(pen);

      homo_line->append(0, 0);
      homo_line->append(1.0, 1.0);


      lorenz_chart_ = new QChart();
      lorenz_chart_->legend()->hide();
      lorenz_chart_->addSeries(lorenz_series_);
      lorenz_chart_->addSeries(homo_line);
      lorenz_chart_->createDefaultAxes();

      lorenz_chart_view_->setChart(lorenz_chart_);



      {
        auto* axisx = static_cast<QValueAxis*>(lorenz_chart_->axes(Qt::Horizontal)[0]);
        axisx->setLabelsFont(scaled_font);
        axisx->setLabelFormat("%.2f");
        axisx->setTitleText("Storage capacity");
        axisx->setTitleFont(font);
        axisx->setLabelsBrush(black_brush);
        axisx->setTitleBrush(black_brush);

        auto* axisy = static_cast<QValueAxis*>(lorenz_chart_->axes(Qt::Vertical)[0]);
        axisy->setLabelsFont(scaled_font);
        axisy->setLabelFormat("%.2f");
        axisy->setTitleText("Flow capacity");
        axisy->setTitleFont(font);
        axisy->setLabelsBrush(black_brush);
        axisy->setTitleBrush(black_brush);
      }

      
      lorenz_series_->setPen({lorenz_series_->pen().color(), 1.75});

      fd_widget_->FlowDiagnosticsChanged += [this] { Update(); };
      
      return lorenz_chart_view_;
    }

    void Update() const {
      const auto& lorenz = fd_widget_->GetFlowSummary().lorenz;

      constexpr int steps = 100;

      lorenz_series_->clear();
      lorenz_series_->append(0, 0);

      const auto cell_count = fd_widget_->Model().CellCount();
      
      for (auto i = 0; i < steps; ++i) {
        const int j = (i + 1.0)/steps*(cell_count - 1);
        lorenz_series_->append(lorenz[j + 1].x(), lorenz[j + 1].y());

      }
    }
  };
}