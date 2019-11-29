#ifndef NETWORKTREEMODEL_H
#define NETWORKTREEMODEL_H

#include <QStandardItemModel>
#include "networkinfo.h"
#include <QDebug>


class NetworkTreeModel: public QStandardItemModel
{
    Q_OBJECT

public:
    static QVector<QString> __signs__;                                                  // подписи при выводе

public:
    NetworkTreeModel(QObject *parent = nullptr);
    virtual ~NetworkTreeModel();

public slots:
    void createNetworkItem(QStandardItem *parent, const NetworkInfo &net_info);         // создать узел
    void createNetworkRoot(const IPrecord &ip, const NetMask &mask);                    // создать корневой узел сеть
    void splitNetworkItem(const QModelIndex &index);                                    // разделить сеть на подсети
//    void mergeNetworkItem(const QModelIndex &index);                                    // объединить подсети в сеть
};

#endif // NETWORKTREEMODEL_H
