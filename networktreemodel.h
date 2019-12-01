#ifndef NETWORKTREEMODEL_H
#define NETWORKTREEMODEL_H

#include <QStandardItemModel>
#include "networkinfo.h"
#include <QDebug>


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
    void createNetworkItem(QStandardItem *parent, const NetworkInfo &net_info);               // создать узел
    void createNetworkRoot(const IPrecord &ip, const NetMask &mask);                          // создать корневой узел сеть
    void splitNetworkItem(const QModelIndex &parentIndex);                                    // разделить сеть на подсети
    void mergeNetworkItem(const QModelIndex &parentIndex);                                    // объединить подсети в сеть
    void writeNetworkInFile();
    void readNetworkOfFile();
};

#endif // NETWORKTREEMODEL_H
