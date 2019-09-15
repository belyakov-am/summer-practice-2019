#include "widget.h"
#include "tree_model.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    QFile file("data.txt");
    file.open(QIODevice::ReadOnly);

    TreeModel *model = new TreeModel(file.readAll());
    file.close();
    ui->treeView->setModel(model);
    for (int column = 0; column < model->columnCount(); ++column) {
        ui->treeView->resizeColumnToContents(column);
    }

    connect(ui->treeView->selectionModel(),SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
            this, SLOT(updateActions(const QItemSelection&,const QItemSelection&)));
    connect(ui->insertRowAction,SIGNAL(clicked()),this,SLOT(insertRow()));
    connect(ui->removeRowAction,SIGNAL(clicked()),this,SLOT(removeRow()));

    updateActions();
}


void Widget::insertRow() {
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();
    if (!model->insertRow(index.row()+1, index.parent())) {
        return;
    }
    updateActions();
    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        QModelIndex child = model->index(index.row()+1, column, index.parent());
        model->setData(child, QVariant("Data"), Qt::EditRole);
    }
}


void Widget::removeRow() {
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();
    if (model->removeRow(index.row(), index.parent())) {
        updateActions();
    }
}


void Widget::updateActions(const QItemSelection &selected,const QItemSelection &deselected) {
    updateActions();
}


void Widget::updateActions() {
    bool hasSelection = !ui->treeView->selectionModel()->selection().isEmpty();
    ui->removeRowAction->setEnabled(hasSelection);

    bool hasCurrent = ui->treeView->selectionModel()->currentIndex().isValid();
    ui->insertRowAction->setEnabled(hasCurrent);

    if (hasCurrent) {
        ui->treeView->closePersistentEditor(ui->treeView->selectionModel()->currentIndex());

        int row = ui->treeView->selectionModel()->currentIndex().row();
        int column = ui->treeView->selectionModel()->currentIndex().column();

        if (ui->treeView->selectionModel()->currentIndex().parent().isValid()) {
            this->setWindowTitle(tr("(row,col)=(%1,%2)").arg(row).arg(column));
        } else {
            this->setWindowTitle(tr("(row,col)=(%1,%2) ROOT").arg(row).arg(column));
        }
    }
}
