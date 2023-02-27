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

#include <QApplication>
#include <QChartView>
#include <QClipboard>
#include <QMenu>
#include <QPieSeries>
#include <QTreeView>

#include <boost/format.hpp>
#include <boost/math/constants/constants.hpp>

#include <sstream>

namespace rrm::fd::core
{
  class QCustomPieSlice : public QtCharts::QPieSlice
  {
    Q_OBJECT

  public:
    std::string name;
    
    QCustomPieSlice(QObject* const parent = nullptr) : QPieSlice(parent) {}
  };
  
  class QExtendedPieChartView : public QtCharts::QChartView
  {
    Q_OBJECT

    QMenu context_menu_;
    
    static void DrawText(QPainter* painter, QPointF p, const QString& text) {
      // NOLINTBEGIN(clang-diagnostic-enum-enum-conversion)
      auto size = QFontMetrics(painter->font()).size(Qt::TextDontClip | Qt::AlignHCenter, text);

      QRectF rect({p.x() - size.width()/2., p.y() - size.height()/2.}, size);

      painter->drawText(rect, Qt::TextDontClip | Qt::AlignHCenter, text);
      // NOLINTEND(clang-diagnostic-enum-enum-conversion)
    }
    
  public:
    bool show_percentage = true;
    
    QExtendedPieChartView(QWidget* const parent = nullptr) : QChartView(parent) {
      setBackgroundBrush(Qt::GlobalColor::white);


      setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
      connect(this, &QTreeView::customContextMenuRequested, [this](const QPoint& pos) {
        context_menu_.popup(viewport()->mapToGlobal(pos));
      });

      connect(context_menu_.addAction("Copy"), &QAction::triggered, [this] {
        std::stringstream ss;

        auto* series = static_cast<QtCharts::QPieSeries*>(chart()->series()[0]);
        
        for (auto* x : series->slices()) {
          auto* slice = static_cast<QCustomPieSlice*>(x);
          
          if (show_percentage)
            ss << boost::format("%s\t%.3f\n") % slice->name % slice->percentage();
          else
            ss << boost::format("%s\t%.9e\n") % slice->name % slice->value();
        }

        QApplication::clipboard()->setText(QString::fromStdString(ss.str()));
      });

      connect(context_menu_.addAction("Copy values"), &QAction::triggered, [this] {
        std::stringstream ss;

        auto* series = static_cast<QtCharts::QPieSeries*>(chart()->series()[0]);
        
        for (auto* x : series->slices()) {
          auto* slice = static_cast<QCustomPieSlice*>(x);
          
          if (show_percentage)
            ss << boost::format("%.3f\n") % slice->percentage();
          else
            ss << boost::format("%.9e\n") % slice->value();
        }

        QApplication::clipboard()->setText(QString::fromStdString(ss.str()));
      });

      
    }

  protected:
    void paintEvent(QPaintEvent* event) override {
      QPainter painter(viewport());
      painter.setRenderHint(QPainter::RenderHint::Antialiasing);
      
      painter.fillRect(0, 0, width(), height(), backgroundBrush());

      if (!this->chart()->series().count())
        return;

      auto side = std::min(width(), height())*0.9;

      QRectF rect(-side/2., -side/2., side, side);

      auto* pie_series = static_cast<QtCharts::QPieSeries*>(this->chart()->series()[0]);

      auto current_a = 0;

      // painter.setPen({Qt::white, 3.75});

      painter.translate(width()/2., height()/2.);


      auto f = font();
      // f.setPointSizeF(f.pointSize() + 1);
      f.setBold(true);
      painter.setFont(f);
      
      QFontMetrics metric(font());

      for (auto* slice : pie_series->slices()) {
        constexpr auto full_circle = 5760;
        
        painter.setPen({::Qt::white, 2.0});

        int span = slice->percentage()*full_circle;  // NOLINT(clang-diagnostic-float-conversion)

        painter.setBrush(slice->color());
        painter.drawPie(rect, current_a, current_a + span > full_circle ? full_circle - current_a : span);
        // auto text = slice->label();
        
        QString text;

        if (show_percentage)
          text = QString::fromStdString(
            (boost::format("%s\n%.1f%%")
              % static_cast<QCustomPieSlice*>(slice)->name//static_cast<VerticalWell*>(static_cast<QCustomPieSlice*>(slice)->data)->name
              % (100.0*slice->percentage())).str());
        else {
          // auto value = slice->value();
          // int exponent = std::log10<int>(value);
          //
          // text = QString::fromStdString(
          //   (boost::format("%s\n%.1fe%i")
          //     % static_cast<QCustomPieSlice*>(slice)->name//static_cast<VerticalWell*>(static_cast<QCustomPieSlice*>(slice)->data)->name
          //     % (value/std::pow(10., exponent))
          //     % exponent).str());
          
          
          text = QString::fromStdString(
            (boost::format("%s\n%.1e")
              % static_cast<QCustomPieSlice*>(slice)->name//static_cast<VerticalWell*>(static_cast<QCustomPieSlice*>(slice)->data)->name
              % slice->value()).str());
        }
          
       
        auto size = metric.size(Qt::TextDontClip | Qt::AlignHCenter, text);  // NOLINT(clang-diagnostic-enum-enum-conversion)
        // auto label_radius = std::max(size.width(), size.height())/2.;

        constexpr auto pi2 = 2*boost::math::constants::pi<double>();
        auto angle = (current_a + span/2.)/5760.*pi2;
        
        auto x = cos(angle)*(side*0.275);
        auto y = -sin(angle)*(side*0.275);

        QRectF text_rect ({x - size.width()/2., y - size.height()/2.}, size);
        
        painter.setPen(slice->color().lightnessF() < 0.5 ? Qt::white : Qt::black); 

        painter.drawText(text_rect, Qt::TextDontClip | Qt::AlignHCenter, text);  // NOLINT(clang-diagnostic-enum-enum-conversion)
        
        current_a += span;
      }
    }
  };
}