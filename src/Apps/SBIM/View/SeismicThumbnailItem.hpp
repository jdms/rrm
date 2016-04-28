/*
 * SeismicThumbnailItem.hpp
 *
 *  Created on: Apr 27, 2016
 *      Author: felipe
 */

#ifndef _SBIM_VIEW_SEISMICTHUMBNAILITEM_HPP_
#define _SBIM_VIEW_SEISMICTHUMBNAILITEM_HPP_

#include <QtWidgets/QListWidgetItem>


/// Subclassing QListWidgetItem just to overwrite the operator<. The default one use data() as string.
/// The new version use data() as integer, so it can place the slices in the right crescent order.
namespace RRM
{

	class SeismicThumbnailItem : public QListWidgetItem
	{
		public:

			explicit SeismicThumbnailItem(QListWidget *view = Q_NULLPTR, int type = Type);
			explicit SeismicThumbnailItem(const QString &text, QListWidget *view = Q_NULLPTR, int type = Type);
			explicit SeismicThumbnailItem(const QIcon &icon, const QString &text,
			                             QListWidget *view = Q_NULLPTR, int type = Type);

			virtual ~SeismicThumbnailItem ( );

			virtual bool operator<(const QListWidgetItem &other) const;
	};

} /* namespace RRM */

#endif /* _SBIM_VIEW_SEISMICTHUMBNAILITEM_HPP_ */
