/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef OBJECTTREEITEM_H
#define OBJECTTREEITEM_H


#include <QTreeWidgetItem>

#include "./core/definitions/constants.hpp"


class ObjectTreeItem: public QTreeWidgetItem
{
    public:


        ObjectTreeItem();

        void setType( const Settings::Objects::ObjectType& type_ );
        Settings::Objects::ObjectType getType() const;


        void setIndex( const std::size_t index_ );
        std::size_t getIndex() const;

        void setTreeIndex( const std::size_t index_ );
        std::size_t getTreeIndex() const;

        ObjectTreeItem* cloneItem() const;

    protected:


        std::size_t index;
        std::size_t tree_index;
        Settings::Objects::ObjectType type;


};

#endif // OBJECTTREEITEM_H
