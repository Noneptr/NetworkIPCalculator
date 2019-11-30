#include "networktreemodel.h"

QVector<QString> NetworkTreeModel::__signs__ = {"Adress: ", "BitMask: ", "Mask: ",
                                                "Wildcard: ", "Direct broadcast: ",
                                                "Host min: ", "Host max: ", "Hosts: "};


NetworkTreeModel::NetworkTreeModel(QObject *parent)
    :QStandardItemModel(parent)
{
}


void NetworkTreeModel::createNetworkItem(QStandardItem *parent, const NetworkInfo &net_info)
{
    QString data = "";
    data += __signs__[0] + net_info.network().toQString() + "\n" +
            __signs__[1] + QString::number(net_info.mask().countBits()) + "\n" +
            __signs__[2] + net_info.mask().mask().toQString() + "\n" +
            __signs__[3] + net_info.wildcard().toQString() + "\n" +
            __signs__[4] + net_info.directBroadcast().toQString() + "\n" +
            __signs__[5] + net_info.hostMin().toQString() + "\n" +
            __signs__[6] + net_info.hostMax().toQString() + "\n" +
            __signs__[7] + QString::number(net_info.mask().countHosts());
    QStandardItem *node = new QStandardItem(data);

    NetMask subMask = NetMask(net_info.mask().countBits() + 1);
    if (subMask.countHosts() >= 2)
    {
        node->appendRow(new QStandardItem("&"));
    }

    parent->appendRow(node);
}


void NetworkTreeModel::createNetworkRoot(const IPrecord &ip, const NetMask &mask)
{
    QStandardItem *parentItem = this->invisibleRootItem();
    createNetworkItem(parentItem, NetworkInfo(ip, mask));
}

void NetworkTreeModel::splitNetworkItem(const QModelIndex &parentIndex)
{
    QStandardItem * parentItem = static_cast<QStandardItem *>(parentIndex.internalPointer());
    QStandardItem *netItem = parentItem->child(parentIndex.row());
    if (netItem->rowCount() == 1)
    {
        QString data = netItem->text();
        if (data != "")
        {
            QStringList list = data.split("\n");
            IPrecord net_ip(list[0].split(" ")[1]);                                     // выделить из данных адрес сети
            NetMask net_mask(list[2].split(" ")[1]);                                    // выделить из данных маску сети
            NetworkInfo sub_net1(net_ip, NetMask(net_mask.countBits() + 1));
            NetworkInfo sub_net2(sub_net1.directBroadcast() + 1, NetMask(net_mask.countBits() + 1));

            createNetworkItem(netItem, sub_net1);
            createNetworkItem(netItem, sub_net2);
            netItem->removeRow(0);
        }
    }
}


void NetworkTreeModel::mergeNetworkItem(const QModelIndex &parentIndex)
{
    QStandardItem *parentItem = static_cast<QStandardItem *>(parentIndex.internalPointer());
    QStandardItem *netItem = parentItem->child(parentIndex.row());
    if (netItem->rowCount() == 2)
    {
        netItem->removeRows(0, netItem->rowCount());
        netItem->appendRow(new QStandardItem("&"));
    }
}


NetworkTreeModel::~NetworkTreeModel()
{
}
