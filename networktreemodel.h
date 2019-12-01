#ifndef NETWORKTREEMODEL_H
#define NETWORKTREEMODEL_H

#include <QStandardItemModel>
#include "networkinfo.h"
#include <queue>
#include <QDebug>


enum NetworkTreeModelError {__ERROR_WRITE_IN_BIN_FILE__, __ERROR_READ_OF_BIN_FILE__};       // Исключения вызываемые классом

const QString __file_extention__ = ".ipcalc";

class NetworkTreeModel: public QStandardItemModel
{
    Q_OBJECT

private:
    QString __filename;

public:
    static QVector<QString> __signs__;                                                  // подписи при выводе
    static QString __emptySign__;
    static QString netInfoToString(const NetworkInfo &net_info);
    static NetworkInfo stringToNetInfo(const QString &data);

public:
    NetworkTreeModel(QObject *parent = nullptr);
    NetworkTreeModel(const QString &filename, QObject *parent = nullptr);
    virtual ~NetworkTreeModel();

public:
    void setFilename(const QString &filename);
    QString filename() const;

public slots:
    void insertIntoNetwork(const NetworkInfo &net_info);
    void createNetworkItem(QStandardItem *parent, const NetworkInfo &net_info);               // создать узел
    void createNetworkRoot(const IPrecord &ip, const NetMask &mask);                          // создать корневой узел сеть
    void createNetworkRoot(const NetworkInfo &net_info);                                      // создать корневой узел сеть
    void splitNetworkItem(const QModelIndex &parentIndex);                                    // разделить сеть на подсети
    void mergeNetworkItem(const QModelIndex &parentIndex);                                    // объединить подсети в сеть
    void expandAllExist();                                                                    // раскрыть все cуществующие p.s. довольно тяжёлая операция
    void writeNetworkInFile();                                                                // записать модель в файл
    void readNetworkOfFile();                                                                 // считать модель из файла

signals:
    void needExpandItem(const QModelIndex&);                                                  // сигнализирует представлению о необходимости
                                                                                              // раскрыть элемент с указанным индексом
};

#endif // NETWORKTREEMODEL_H
