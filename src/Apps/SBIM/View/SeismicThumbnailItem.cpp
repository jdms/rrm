/*
 * SeismicThumbnailItem.cpp
 *
 *  Created on: Apr 27, 2016
 *      Author: felipe
 */

#include "SeismicThumbnailItem.hpp"

namespace RRM
{

	SeismicThumbnailItem::SeismicThumbnailItem(QListWidget *view , int type) : QListWidgetItem(view,type)
	{

	}

	SeismicThumbnailItem::SeismicThumbnailItem(const QString &text, QListWidget *view, int type) : QListWidgetItem(text,view,type)
	{

	}

	SeismicThumbnailItem::SeismicThumbnailItem(const QIcon &icon, const QString &text,
	                             QListWidget *view, int type): QListWidgetItem(icon,text,view,type)
	{

	}

	SeismicThumbnailItem::~SeismicThumbnailItem ( )
	{
	}

	bool SeismicThumbnailItem::operator< ( const QListWidgetItem &other ) const
	{
		QVariant v_other = other.data ( Qt::UserRole );
		QVariant v_this = this->data ( Qt::UserRole );

		int index_other = v_other.value<int> ( );
		int index_this = v_this.value<int> ( );

		return ( index_this < index_other );
	}


} /* namespace RRM */
