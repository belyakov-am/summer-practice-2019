#include <tree_item.h>


TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent) {
    parentItem_ = parent;
    itemData_ = data;
}


TreeItem::~TreeItem() {
    qDeleteAll(childItems_);
}


void TreeItem::appendChild(TreeItem *child) {
    childItems_.append(child);
}


QVariant TreeItem::data(int column) const {
    return itemData_.value(column);
}


TreeItem *TreeItem::parent() {
    return parentItem_;
}


TreeItem *TreeItem::child(int number) {
    if (number < 0 || number >= childItems_.size()) {
        return nullptr;
    }

    return childItems_.at(number);
}


int TreeItem::childCount() const {
    return childItems_.size();
}


int TreeItem::childNumber() const {
    if (parentItem_) {
        return parentItem_->childItems_.indexOf(const_cast<TreeItem*>(this));
    }

    return 0;
}


int TreeItem::columnCount() const {
    return itemData_.size();
}


bool TreeItem::setData(int column, const QVariant &value) {
    if (column < 0 || column >= itemData_.size()) {
        return false;
    }

    itemData_[column] = value;
    return true;
}


bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems_.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        TreeItem *item = new TreeItem(data, this);
        childItems_.insert(position, item);
    }

    return true;
}


bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems_.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems_.takeAt(position);

    return true;
}


bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData_.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData_.insert(position, QVariant());

    for (TreeItem *child : qAsConst(childItems_))
        child->insertColumns(position, columns);

    return true;
}


bool TreeItem::removeColumns(int position, int columns) {
    if (position < 0 || position + columns > itemData_.size()) {
        return false;
    }
    for (int column = 0; column < columns; ++column) {
        itemData_.removeAt(position);
    }
    foreach (TreeItem *child, childItems_) child->removeColumns(position, columns);
    return true;
}
