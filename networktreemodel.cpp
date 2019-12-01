#include "networktreemodel.h"

QVector<QString> NetworkTreeModel::__signs__ = {"Adress: ", "BitMask: ", "Mask: ",
                                                "Wildcard: ", "Direct broadcast: ",
                                                "Host min: ", "Host max: ", "Hosts: ", "Busy hosts: "};

QString NetworkTreeModel::__emptySign__ = "&";


NetworkTreeModel::NetworkTreeModel(QObject *parent)
    :QStandardItemModel(parent), __filename("new_net_tree")
{
}

NetworkTreeModel::NetworkTreeModel(const QString &filename, QObject *parent)
    :QStandardItemModel(parent), __filename(filename)
{
}


QString NetworkTreeModel::netInfoToString(const NetworkInfo &net_info)
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
    return data;
}


NetworkInfo NetworkTreeModel::stringToNetInfo(const QString &data)
{
    QStringList list = data.split("\n");
    IPrecord net_ip(list[0].split(" ")[1]);                                     // выделить из данных адрес сети
    NetMask net_mask(list[2].split(" ")[1]);                                    // выделить из данных маску сети
    NetworkInfo net_info(net_ip, net_mask);
    return net_info;
}


void NetworkTreeModel::insertIntoNetwork(const NetworkInfo &net_info)
{
    QStandardItem *parent = invisibleRootItem();
    if (parent->rowCount() == 0)
    {
        createNetworkItem(parent, net_info);
    }
    else
    {
        QStandardItem *root = parent->child(0);
        QStandardItem *curr = root;
        while (true)
        {
            NetworkInfo curr_info = stringToNetInfo(curr->text());          // получение сетевых данных о текущем узле
            IPrecord mid_host = curr_info.directBroadcast() - curr_info.directBroadcast() / 2;
            if (net_info.directBroadcast() < mid_host)                      // если меньше среднего адреса сети узла
            {
                if (curr->rowCount() == 1)
                {
                    createNetworkItem(curr, net_info);                      // создаём и присоединяем новый узел
                    if (curr->text() == __emptySign__)
                    {
                        curr->removeRow(0);                                 // удаления пустого символа
                    }
                    else                                                    // нарушение структуры т.к. правый узел стоит первым
                    {
                        QStandardItem *left = curr->child(1);               // восстановление структуры сыновей, путём перестановки их местами
                        QStandardItem *right = curr->child(0);
                        curr->insertRow(0, left);
                        curr->insertRow(1, right);
                    }
                    break;
                }
                else if (curr->rowCount() == 2)
                {
                    curr = curr->child(0);                                  // спуститься к левому сыну
                }
                else
                {
                    break;                                                  // достигнут самый нижний уровень дерева, выход из вставки
                }
            }
            else
            {
                if (curr->rowCount() == 1)
                {
                    createNetworkItem(curr, net_info);                       // создаём и присоединяем новый узел
                    if (curr->text() == __emptySign__)
                    {
                        curr->removeRow(0);                                  // удаление пустого символа
                    }
                    break;
                }
                else if (curr->rowCount() == 2)
                {
                    curr = curr->child(1);                                  // спуститься к правому сыну
                }
                else
                {
                    break;                                                  // достигнут самый нижний уровень дерева, выход из вставки
                }
            }
        }
    }
}


void NetworkTreeModel::createNetworkItem(QStandardItem *parent, const NetworkInfo &net_info)
{
    QString data = netInfoToString(net_info);

    QStandardItem *node = new QStandardItem(data);

    NetMask subMask = NetMask(net_info.mask().countBits() + 1);
    if (subMask.countHosts() >= 2)                                                      // проверка на то что может ли существовать следующий узел
    {
        node->appendRow(new QStandardItem(__emptySign__));
    }

    parent->appendRow(node);
}


void NetworkTreeModel::createNetworkRoot(const IPrecord &ip, const NetMask &mask)
{
    QStandardItem *parentItem = this->invisibleRootItem();
    createNetworkItem(parentItem, NetworkInfo(ip, mask));
}


void NetworkTreeModel::createNetworkRoot(const NetworkInfo &net_info)
{
    QStandardItem *parentItem = this->invisibleRootItem();
    createNetworkItem(parentItem, net_info);
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
            NetworkInfo net = stringToNetInfo(data);
            NetworkInfo sub_net1(net.network(), NetMask(net.mask().countBits() + 1));
            NetworkInfo sub_net2(sub_net1.directBroadcast() + 1, NetMask(net.mask().countBits() + 1));

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
        netItem->appendRow(new QStandardItem(__emptySign__));
    }
}


void NetworkTreeModel::setFilename(const QString &filename)
{
    __filename = filename;
}


QString NetworkTreeModel::filename() const
{
    return __filename;
}

void NetworkTreeModel::writeNetworkInFile()
{

}


void NetworkTreeModel::readNetworkOfFile()
{
}


NetworkTreeModel::~NetworkTreeModel()
{
}
