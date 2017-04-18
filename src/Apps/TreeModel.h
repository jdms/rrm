#ifndef TREEMODEL_H
#define TREEMODEL_H


#include <QAbstractItemModel>

class TreeModel: public QAbstractItemModel
{
    Q_OBJECT

    public:

        TreeModel( const QString& data_, QObject* parent_ );
        ~TreeModel();

        QVariant data( const QModelIndex& index_, int role_ ) const override;
        Qt::ItemFlags flags( const QModelIndex& index_ ) const override;
        QVariant headerData( int section_, Qt::Orientation orientation_,
                             int role_ = Qt::DisplayRole  ) const override;

//        QModelIndex index( int row_ , int column_ )
};

#endif // TREEMODEL_H
