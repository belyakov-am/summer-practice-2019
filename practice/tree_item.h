#ifndef TREE_ITEM_H
#define TREE_ITEM_H

#include <QList>
#include <QVector>
#include <QVariant>
#include <QtAlgorithms>


class TreeItem {
public:
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parent = nullptr);
    ~TreeItem();

    void appendChild(TreeItem *child);

    // returns child with index `number`
    TreeItem *child(int number);

    // total amount of children
    int childCount() const;

    // total amount of columns for current node (row)
    int columnCount() const;

    // returns value of given column in current node (row)
    QVariant data(int column) const;

    // inserts `count` children (rows)
    bool insertChildren(int position, int count, int columns);

    // inserts `columns` columns at position `position` for current node and all it's children
    bool insertColumns(int position, int columns);

    // returns parent
    TreeItem *parent();

    // removes `count` children from position `position`
    bool removeChildren(int position, int count);

    // removes `columns` columns from position `position`
    bool removeColumns(int position, int columns);

    // returns index of current child in parent's children list
    int childNumber() const;

    // sets current row and columns with data `value`
    bool setData(int column, const QVariant &value);

private:
    // vector of all children
    QVector<TreeItem*> childItems_;

    // vector of columns data for current node (row)
    QVector<QVariant> itemData_;

    // parent
    TreeItem *parentItem_;
};


#endif // TREE_ITEM_H
