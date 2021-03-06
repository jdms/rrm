/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file objecttreeitem.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class ObjectTreeItem
 */


#include "objecttreeitem.h"

ObjectTreeItem::ObjectTreeItem()
{
    setFlags( flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable );
}

void ObjectTreeItem::setType( const Settings::Objects::ObjectType& type_ )
{
    type = type_;
}

Settings::Objects::ObjectType ObjectTreeItem::getType() const
{
    return type;
}


void ObjectTreeItem::setIndex( const std::size_t index_ )
{
    index = index_;
}

std::size_t ObjectTreeItem::getIndex() const
{
    return index;
}
