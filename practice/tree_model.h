#ifndef TREE_MODEL_H
#define TREE_MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <tree_item.h>


// inherit QAbstractItemModel in order to feed class to model for QTreeView
class TreeModel : public QAbstractItemModel {
    // always add QObject macro
    Q_OBJECT
public:
    explicit TreeModel(const QStringList &header, const QString &data, QObject *parent = nullptr);
    ~TreeModel();

    // returns data with given `index` and `role`
    QVariant data(const QModelIndex &index, int role) const override;

    // return flags for item with given `index` (flags in qt are simple enum)
    // reimplemented in order to make model editable
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // returns data stored in root
    // reimplemented in order to make model (headers) editable
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // gets current item index by row and column
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    // gets parent's index
    QModelIndex parent(const QModelIndex &index) const override;

    // returns number of rows for current item
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    // returns number of columns for current item
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // sets data `value` to node with index `index`
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    // sets headers data for current column
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;

    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;

    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

private:
    void setupModelData(const QStringList &lines, TreeItem *parent);

    // get item by index
    TreeItem *getItem(const QModelIndex &index) const;

    // root item of a tree
    TreeItem *rootItem;
};

#endif // TREE_MODEL_H
