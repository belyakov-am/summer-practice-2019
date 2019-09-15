#include <tree_item.h>
#include <tree_model.h>


TreeModel::TreeModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent) {
    QVector<QVariant> rootData;
    rootData << "Node";
    rootItem = new TreeItem(rootData);
    setupModelData(data.split('\n'), rootItem);
}


TreeModel::~TreeModel() {
    delete rootItem;
}


TreeItem *TreeModel::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        // internalPointer return void*, therefore cast
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item) {
            return item;
        }
    }

    // if there is no valid index or item, return root
    return rootItem;
}


QVariant TreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant();
    }

    TreeItem *item = getItem(index);
    return item->data(index.column());
}


Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}


QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->data(section);
    }

    return QVariant();
}


QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0) {
        return QModelIndex();
    }

    TreeItem *parentItem = getItem(parent);
    TreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}


QModelIndex TreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();
    if (parentItem == rootItem) {
        return QModelIndex();
    }
    return createIndex(parentItem->childNumber(), 0, parentItem);
}


int TreeModel::rowCount(const QModelIndex &parent) const {
    TreeItem *parentItem = getItem(parent);
    return parentItem->childCount();
}


int TreeModel::columnCount(const QModelIndex &parent) const {
    // do not use parent because all items have the same number of columns
    Q_UNUSED(parent);
    return rootItem->columnCount();
}


void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent) {
    QList<TreeItem*> parents;
    parents << parent;
    QList<int> indentations;
    indentations << 0;

    int num = 0;
    int pos = 0;
    // move to first char skipping whitespaces
    while (num < lines.count()) {
        pos = 0;
        while (pos < lines[num].length()) {
            if (lines[num].at(pos) != ' ') {
                break;
            }
            ++pos;
        }

        // remove whitespaces from current line
        // .mid(pos) returns string from `pos` till the end
        // .trimmed() removes whitespaces from start and end
        QString lineData = lines[num].mid(pos).trimmed();

        if (!lineData.isEmpty()) {
            // split by columns
            QStringList columnStrings = lineData.split(":::", QString::SkipEmptyParts);

            QVector <QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column) {
                columnData << columnStrings[column];
            }

            // if pos of current item greater then last indent -- this is new parent
            if (pos > indentations.last()) {
                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount() - 1);
                    indentations << pos;
                }
            } else {
                while (pos < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }
            parents.last()->appendChild(new TreeItem(columnData, parents.last()));
        }
        ++num;
    }
}


bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole) {
        return false;
    }

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result) {
        // sends signal to other nodes to indicate changes in current node
        emit dataChanged(index, index);
    }
    return result;
}


bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal) {
        return false;
    }

    bool result = rootItem->setData(section, value);

    if (result) {
        // sends signal to other nodes to indicate changes in current node
        emit headerDataChanged(orientation, section, section);
    }
    return result;
}


bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent) {
    bool success;
    // Qt function for overriding insertColumns function
    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();
    return success;
}


bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent) {
    TreeItem *parentItem = getItem(parent);
    bool success;
    // Qt function for overriding insertRows function
    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();
    return success;
}


bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent) {
    bool success;
    // Qt function for overriding removeColumns function
    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();
    if (rootItem->columnCount() == 0) removeRows(0, rowCount());
    return success;
}


bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent) {
    TreeItem *parentItem = getItem(parent);
    bool success = true;
    // Qt function for overriding removeRows function
    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();
    return success;
}
