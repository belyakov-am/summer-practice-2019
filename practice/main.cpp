#include <widget.h>
#include <QApplication>
#include <QTreeView>
#include <QAbstractItemModel>
#include <QStringList>
#include <QStringListModel>
#include <QListView>
#include <QFileSystemModel>
#include <QDir>
#include <QSplitter>


void writeVectorToFile(QVector<QString> test) {
    QString filename = "data.txt";
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << "container" << endl;

        for (auto&& str : test) {
            stream << " " << str << endl;
        }
    }
}


void processVector(std::vector<std::string> test) {
    QVector<QString> qt_test;
    for (size_t i = 0; i < test.size(); ++i) {
        qt_test.append(QString::fromStdString(test[i]));
    }
    writeVectorToFile(qt_test);
}


int main(int argc, char *argv[]) {
    std::vector<std::string> test = {"first line", "second line", "third line"};

    processVector(test);

    QApplication app(argc, argv);
    Widget w;
    w.show();
    return app.exec();
}
