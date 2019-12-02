#include "networktreemodel.h"

QVector<QString> NetworkTreeModel::__signs__ = {"Adress: ", "BitMask: ", "Mask: ",
                                                "Wildcard: ", "Direct broadcast: ",
                                                "Host min: ", "Host max: ", "Free hosts: ",
                                                "Busy hosts: "};

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
            __signs__[7] + QString::number(net_info.mask().countHosts() - net_info.busyHosts()) + "\n" +
            __signs__[8] + QString::number(net_info.busyHosts());
    return data;
}


NetworkInfo NetworkTreeModel::stringToNetInfo(const QString &data)
{
    QStringList list = data.split("\n");
    IPrecord net_ip(list[0].split(": ")[1]);                                     // выделить из данных адрес сети
    NetMask net_mask(list[2].split(": ")[1]);                                    // выделить из данных маску сети
    unsigned int busy_hosts = list[8].split(": ")[1].toUInt();                   // выделить из данных количество занятых хостов
    NetworkInfo net_info(net_ip, net_mask);
    net_info.setBusyHosts(busy_hosts);
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
            IPrecord mid_host = curr_info.directBroadcast() - curr_info.wildcard() / 2;
            if (net_info.directBroadcast() < mid_host)                      // если меньше среднего адреса сети узла
            {
                if (curr->rowCount() == 1)
                {
                    QString child0data = curr->child(0)->text();
                    if (child0data != __emptySign__)
                    {
                        NetworkInfo child_info = stringToNetInfo(child0data);
                        if (child_info.directBroadcast() < mid_host)        // если позиция левого занята
                        {
                            curr = curr->child(0);                          // то спускаемся к нему
                            continue;
                        }
                    }

                    createNetworkItem(curr, net_info);                      // создаём и присоединяем новый узел
                    if (child0data == __emptySign__)
                    {
                        curr->removeRow(0);                                 // удаления пустого символа
                    }
                    else                                                    // нарушение структуры т.к. правый узел стоит первым
                    {
                        curr->sortChildren(0);                              // восстановление структуры сыновей, путём перестановки их местами
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
                    QString child0data = curr->child(0)->text();
                    if (child0data != __emptySign__)
                    {
                        NetworkInfo child_info = stringToNetInfo(child0data);
                        if (child_info.directBroadcast() >= mid_host)       // если позиция правого занята
                        {
                            curr = curr->child(0);                          // то спускаемся к нему
                            continue;
                        }
                    }

                    createNetworkItem(curr, net_info);                       // создаём и присоединяем новый узел
                    if (curr->child(0)->text() == __emptySign__)
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


void NetworkTreeModel::expandAllExist()
{
    QStandardItem *parent = invisibleRootItem();
    if (parent->rowCount() > 0)
    {
        QStandardItem *root = parent->child(0);
        std::queue<QStandardItem *> nodes;
        nodes.push(root);
        QStandardItem *curr;
        while(!nodes.empty())                                           // обход дерева в ширину
        {
            curr = nodes.front();
            nodes.pop();

            if (curr->rowCount() > 1)                                   // если у узла два сына значит его необходимо раскрыть
            {
                emit needExpandItem(curr->index());
                nodes.push(curr->child(0));
                nodes.push(curr->child(1));
            }
        }
    }
}


void NetworkTreeModel::createNetworkItem(QStandardItem *parent, const NetworkInfo &net_info)
{
    QString data = netInfoToString(net_info);

    QStandardItem *node = new QStandardItem(data);

    NetMask subMask = NetMask(net_info.mask().countBits() + 1);
    if ((subMask.countHosts() >= 2) && (net_info.busyHosts() == 0))                     // проверка на то что может ли существовать следующий узел
    {
        node->appendRow(new QStandardItem(__emptySign__));                              // добавление пустого символа
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
    QStandardItem * parentItem = static_cast<QStandardItem *>(parentIndex.internalPointer());       // родитель разделяемого узла
    QStandardItem *netItem = parentItem->child(parentIndex.row());                                  // разделяемый узел
    if (netItem->rowCount() == 1)
    {
        QString data = netItem->text();
        NetworkInfo net = stringToNetInfo(data);
        NetworkInfo sub_net1(net.network(), NetMask(net.mask().countBits() + 1));                   // выделение подсетей
        NetworkInfo sub_net2(sub_net1.directBroadcast() + 1, NetMask(net.mask().countBits() + 1));

        createNetworkItem(netItem, sub_net1);                                                       // добавление подсетей в дерево
        createNetworkItem(netItem, sub_net2);
        netItem->removeRow(0);                                                                      // удаление пустого символа __emptySign__
    }
}


void NetworkTreeModel::mergeNetworkItem(const QModelIndex &parentIndex)
{
    QStandardItem *parentItem = static_cast<QStandardItem *>(parentIndex.internalPointer());        // родитель агрегируемого узла
    QStandardItem *netItem = parentItem->child(parentIndex.row());                                  // агрегируемый узел
    if (netItem->rowCount() == 2)
    {
        netItem->removeRows(0, netItem->rowCount());                                                // удаление дочерних узлов
        netItem->appendRow(new QStandardItem(__emptySign__));                                       // добавление пустого символа
    }
}


void NetworkTreeModel::setFilename(const QString &filename)
{
    __filename = filename;
}


void NetworkTreeModel::makeBusyNode(QStandardItem *node, unsigned int &busy_hosts)
{
    if (node && busy_hosts > 0)
    {
        QString data = node->text();
        if (data != __emptySign__)
        {
            NetworkInfo node_info = stringToNetInfo(data);

            if (node_info.busyHosts() == 0)
            {
                long long unsigned int e = 10 *
                static_cast<long long unsigned int>(pow(10, unsigned(QString::number(busy_hosts).count())));
                qDebug() << "e: " << e;
                qDebug() << "op1: " << log2(node_info.mask().countHosts());
                qDebug() << "op2: " << log2(busy_hosts);

                double val = round((log2(node_info.mask().countHosts())
                            - log2(busy_hosts)) * e) / e;

                qDebug() << "fact_val: " << log2(node_info.mask().countHosts())
                             - log2(busy_hosts);
                qDebug() << "round_val: " << val;
                qDebug() << endl;

                if (val >= 0)
                {
                    if (val <= 1)
                    {
                        node_info.setBusyHosts(busy_hosts);
                        node->setText(netInfoToString(node_info));
                        node->removeRows(0, node->rowCount());
                        busy_hosts = 0;
                    }
                    else
                    {
                        if (node->rowCount() == 1)
                        {
                            //qDebug() << node->parent()->text() << endl;
                            splitNetworkItem(node->index());
                        }

                        makeBusyNode(node->child(0), busy_hosts);
                        makeBusyNode(node->child(1), busy_hosts);
                    }
                }
            }
        }
    }
}


QString NetworkTreeModel::filename() const
{
    return __filename;
}

void NetworkTreeModel::writeNetworkInFile()
{
    FILE *file = nullptr;
    file = fopen((__filename + __file_extention__).toStdString().c_str(), "wb");
    if (file)
    {
        QStandardItem *parent = invisibleRootItem();
        if (parent->rowCount() > 0)
        {
            QStandardItem *root = parent->child(0);
            std::queue<QStandardItem *> nodes;
            nodes.push(root);
            QStandardItem *curr;
            while(!nodes.empty())                                                // обход дерева в ширину
            {
                curr = nodes.front();
                nodes.pop();

                if (curr->text() != __emptySign__)
                {
                    NetworkInfo net_info = stringToNetInfo(curr->text());

                    fwrite(&net_info, sizeof(net_info), 1, file);               // запись информации об узле в файл

                    for (int i = 0; i < curr->rowCount(); i++)
                    {
                        nodes.push(curr->child(i));
                    }
                }
            }
        }
        fclose(file);
    }
    else
    {
        throw __ERROR_WRITE_IN_BIN_FILE__;
    }
}


void NetworkTreeModel::readNetworkOfFile()
{
    FILE *file = nullptr;
    file = fopen((__filename + __file_extention__).toStdString().c_str(), "rb");
    if (file)
    {
        this->clear();
        setHorizontalHeaderLabels({QString("")});

        NetworkInfo net_info;
        while (fread(&net_info, sizeof(net_info), 1, file) == 1)        // чтение информации об узле из файла
        {
            insertIntoNetwork(net_info);                                // вставка узла в дерево
        }

        fclose(file);

        // раскрытие довольно дорогая операция
        // чтение происходит быстрее
        expandAllExist();                                               // раскрытие всех существующих узлов
    }
    else
    {
        throw __ERROR_READ_OF_BIN_FILE__;
    }
}


NetworkTreeModel::~NetworkTreeModel()
{
}
