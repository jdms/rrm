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

#include <QBoxLayout>

namespace rrm::fd::core::QLayoutBuilder
{
  // ReSharper disable CppNonExplicitConversionOperator
  struct spacing { int value; operator int() const { return value; } };
  struct stretch { int value; operator int() const { return value; } };

  enum class dir
  {
    horizontal,
    vertical
  };
  
  class box
  {
    QBoxLayout* qbox;

  public:
    box(dir d, int spacing = 0) {
      qbox = new QBoxLayout{d == dir::horizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom};
      qbox->setSpacing(spacing);
      qbox->setContentsMargins(0, 0, 0, 0);
    }

    // template<typename... Args>
    // box(QBoxLayout::Direction dir, int spacing = 0, Args&&... args);

    operator QBoxLayout*() const { return qbox; }
  };

  class widget
  {
    QWidget* qwidget;

  public:
    widget(QLayout* l) {
      qwidget = new QWidget{};
      qwidget->setContentsMargins(0, 0, 0, 0);
      qwidget->setLayout(l);
    }

    operator QWidget*() const { return qwidget; }
  };
  // ReSharper restore CppNonExplicitConversionOperator

  namespace resolve
  {
    inline void add(QBoxLayout* l, std::tuple<QWidget*, stretch> arg) {
      l->addWidget(std::get<QWidget*>(arg), std::get<stretch>(arg));
    }
    
    inline void add(QBoxLayout* l, QWidget* arg) { l->addWidget(arg); }
    inline void add(QBoxLayout* l, QLayout* arg) { l->addLayout(arg); }
    inline void add(QBoxLayout* l, QWidget& arg) { l->addWidget(&arg); }
    inline void add(QBoxLayout* l, QLayout& arg) { l->addLayout(&arg); }
    inline void add(QBoxLayout* l, stretch arg) { l->addStretch(arg); }
    inline void add(QBoxLayout* l, spacing arg) { l->addSpacing(arg); }
  }

  // template<typename... Args>
  // box::box(QBoxLayout::Direction dir, int spacing, Args&&... args) {
  //   qbox = new QBoxLayout{dir};
  //   qbox->setSpacing(spacing);
  //   qbox->setContentsMargins(0, 0, 0, 0);
  //
  //   dpl::apply([this](auto arg) { resolve::add(qbox, arg); }, std::forward<Args>(args)...);
  // }

  template <typename Item>
  box&& operator<<(box&& b, Item&& arg) {
    resolve::add(b, std::forward<Item>(arg));
    return std::move(b);
  }
  
  template <typename Item>
  box& operator<<(box& b, Item&& arg) {
    resolve::add(b, std::forward<Item>(arg));
    return b;
  }

  template <typename Item>
  box&& operator|(box&& b, Item&& arg) {
    resolve::add(b, std::forward<Item>(arg));
    return std::move(b);
  }
  
  template <typename Item>
  box& operator|(box& b, Item&& arg) {
    resolve::add(b, std::forward<Item>(arg));
    return b;
  }
  
  // template<typename... Rest>
  // void AddItem(QBoxLayout* l, QWidget& widget, Rest&&... args) {
  //   l->addWidget(&widget);
  //   QLayoutBuilder::AddItem(l, std::forward<Rest>(args)...);
  // }
  
  
  // template<typename... Args>
  // auto* box(QBoxLayout::Direction dir, int spacing = 0) {
  //   auto* bl = new QBoxLayout{dir};
  //   bl->setSpacing(spacing);
  //   bl->setContentsMargins(0, 0, 0, 0);
  //   // dpl::apply([bl](auto arg) { resolve::add(bl, arg); });
  //   return bl;
  // }
}